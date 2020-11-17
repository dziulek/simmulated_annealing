#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <chrono>

using namespace std;

typedef vector<vector<pair<int, pair<double, double> > > > VVIFF;
typedef vector<pair<int,pair<double,double> > > VIFF;
typedef vector<int> VI;
typedef pair<int,pair<double,double> > IFF;
typedef vector<vector<int> > VVI;

#define	MP make_pair
#define ST first
#define ND second
#define FOR(x,b,e) for(int x=b;i<=(e);++x)
#define FORD(x,b,e) for(int x=b;i>=(e);--x)
#define REP(x,n) for(int x=0;x<(n);++x)
#define SIZE(x) ((int)(x).size())
#define	ALL(c) (c).begin(),(c).end()

struct customer {
	int id;
	double x;
	double y;
	double q;
	double e;
	double l;
	double d;
	double waiting_time;
};
//stale
vector<customer> customer_list;
vector<int> remaining_capacity;
vector<bool> remaining_cus;
VVIFF initial_solution;
VVIFF current_solution;
VVIFF best_solution;
VVI tabu_list;
int capacity,vehicles;
int random_moves_begin=10;
int MAXVALUE=1000000000;
int tabu_period=customer_list.size()/10;
//parametry sterujace heurystyka
float Temperature;
float INITEMP;
float RATIO;
float epoch_len;
float TIME;
float PINIT;
float MINPERCENT;
double beta1=100000, beta2=10, beta3=1;
double _MAX=1000000000;
double epsilon=0.0000000001;
double k1=0.3,k2=0.3;
//initial solution methods
double dist(int &i, int &j) {
	double x = customer_list[i].x - customer_list[j].x, y = customer_list[i].y - customer_list[j].y;
	return sqrt(x*x + y * y);
}
double b_new(int &prec, int &succ, double &begin_time_prec) {
	return max(customer_list[succ].e, begin_time_prec + customer_list[prec].d + dist(succ, prec));
}
double b_waiting(int &prec, int &succ, double &begin_time_prec) {
	return begin_time_prec + customer_list[prec].d + dist(succ, prec);
}
bool check_possible(){
    double begin_time=0;
	for (int i = 1; i < customer_list.size(); i++) {
		if (customer_list[i].q > capacity)return 0;
	}
	for (int i = 1; i < customer_list.size(); i++) {
		double b = b_new(customer_list[0].id, i,begin_time);
		if (b > customer_list[i].l)return 0;
		if (b_new(i, customer_list[0].id, b) > customer_list[0].l)return 0;
	}
	return 1;
}
//a) insertion heuristic
bool check_feasible(int &route_number, int &pre, int &suc, int &ins_customer, int indeks_ins,VVIFF &routes) {
	double b_ins = b_new(pre, ins_customer, routes[route_number][indeks_ins - 1].second.first);
	if (b_ins > customer_list[ins_customer].l)return 0;

	double PF = b_new(ins_customer, suc, b_ins) - routes[route_number][indeks_ins].second.first;
	for (int i = indeks_ins; i < routes[route_number].size(); i++) {
		if (PF + routes[route_number][i].second.first > customer_list[routes[route_number][i].first].l)return 0;
		if (i != routes[route_number].size() - 1)PF = max(double(0), PF - customer_list[routes[route_number][i + 1].first].waiting_time);
	}
	return 1;
}
bool range_feasible(VVIFF &solution,int &route_number,int first_indeks,int indeks_last,double &pushforward_out,double &pushforward_in){//sprawdza tylko bez wstawiania, potrzebne do funkcji delete_insert
    if(abs(pushforward_in)<epsilon){
        pushforward_out=0;return 1;
    }
    else if(pushforward_in>0){
        double PF=pushforward_in;
        for(int i=first_indeks;i<indeks_last-1;i++){
            if(solution[route_number][i].ND.ST+PF>customer_list[solution[route_number][i].ST].l)return 0;
            PF=max(double(0),PF-customer_list[solution[route_number][i+1].ST].waiting_time);
        }
        if(solution[route_number][indeks_last-1].ND.ST+PF>customer_list[solution[route_number][indeks_last-1].ST].l)return 0;
        pushforward_out=PF;
        return 1;
    }
    else {
        double PB=pushforward_in,zapas;
        for(int i=first_indeks+1;i<indeks_last;i++){
            zapas=solution[route_number][i].ND.ST-customer_list[solution[route_number][i].ST].e;
            if(zapas+PB<=epsilon)PB=-zapas;
            if(abs(PB)<epsilon){
                pushforward_out=0;return 1;
            }
        }
        pushforward_out=PB;//cout<<PB<<endl;
        return 1;
    }
    return 0;
}
void exe_range_feasible(VVIFF &solution, int &route_number,int first_indeks,int indeks_last,double &pushforward_out, double &pushforward_in,double &diff){
    if(abs(pushforward_in)<epsilon){
        for(int i=first_indeks;i<indeks_last;i++){
            solution[route_number][i].ND.ND+=diff;
        }
        pushforward_out=0;
    }
    else if(pushforward_in>0){//to znaczy ze lecimy do przodu
        double PF=pushforward_in,PF_1=pushforward_in;
        for(int i=first_indeks;i<indeks_last-1;i++){
            //aktualizujemy dane
            solution[route_number][i].ND.ST+=PF;//aktualizacja czasu rozpoczecia pracy u danego klienta

            //if(customer_list[solution[route_number][i].ST].l<solution[route_number][i].ND.ST)cout<<"r: "<<first_indeks<<" "<<solution[route_number][i].ST<<endl;

            solution[route_number][i].ND.ND+=diff;
            PF_1=PF;
            PF=max(double(0),PF-customer_list[solution[route_number][i+1].ST].waiting_time);
            customer_list[solution[route_number][i+1].ST].waiting_time=max(double(0),customer_list[solution[route_number][i+1].ST].waiting_time-PF_1);
        }
        solution[route_number][indeks_last-1].ND.ST+=PF_1;
        solution[route_number][indeks_last-1].ND.ND+=diff;
        pushforward_out=PF_1;
    }
    else {
        //wykonujemy ruchy wstecz
        double PB=pushforward_in,zapas,PB_1=pushforward_in;
        for(int i=first_indeks;i<indeks_last-1;i++){
            solution[route_number][i].ND.ST+=PB;
            solution[route_number][i].ND.ND+=diff;

            //if(solution[route_number][i].ND.ST<customer_list[solution[route_number][i].ST].e)cout<<"blad";

            PB_1=PB;
            zapas=solution[route_number][i+1].ND.ST-customer_list[solution[route_number][i+1].ST].e;
            //aktualizacja waiting time
            if(customer_list[solution[route_number][i+1].ST].waiting_time>epsilon){
                customer_list[solution[route_number][i+1].ST].waiting_time+=(-PB);
            }
            else if(solution[route_number][i+1].ST!=0&&PB+zapas<epsilon)customer_list[solution[route_number][i+1].ST].waiting_time=-(PB+zapas);
            //aktualizacja PB
            if(zapas+PB<=epsilon)PB=-zapas;
        }
        solution[route_number][indeks_last-1].ND.ST+=PB_1;
        solution[route_number][indeks_last-1].ND.ND+=diff;
        pushforward_out=PB_1;
    }
}
void licz_na_trasie(VVIFF &solution,int r){
    double time=0,t_prev=0;
    cout<<time<<" ";
    for(int i=1;i<solution[r].size();i++){
        time=b_new(solution[r][i-1].ST,solution[r][i].ST,t_prev);cout<<time<<" ";
        t_prev=time;
    }
}
void licz_waiting(VVIFF &solution,int r){
    double time=0,t_prev=0;
    cout<<time<<" ";
    for(int i=1;i<solution[r].size();i++){
        time=b_new(solution[r][i-1].ST,solution[r][i].ST,t_prev);
        cout<<fixed<<setprecision(2)<<time-b_waiting(solution[r][i-1].ST,solution[r][i].ST,t_prev)<<" ";
        t_prev=time;
    }
}
void show_solution(VVIFF &solution,fstream &wy){
    wy<<solution[solution.size()-1][0].ND.ND<<" "<<fixed<<setprecision(5)<<solution[solution.size()-1][1].ND.ND<<endl;
    for(int i=0;i<solution.size()-1;i++){
        for(int j=0;j<solution[i].size();j++){
            wy<<solution[i][j].ST<<" ";
        }wy<<endl;
    }
}
void show_waiting(VVIFF &solution){
    for(int i=0;i<solution.size()-1;i++){
        for(int j=0;j<solution[i].size();j++){
            cout<<fixed<<setprecision(2)<<customer_list[solution[i][j].ST].waiting_time<<" ";
        }cout<<endl;
    }
}
vector<pair<int, pair<int, double> > > c1and2(VVIFF &routes,double &alfa_1, double &alfa_2, double &mikro, int route_number, double &lambda, int &rem_demand) {
	double d_0u;
	vector<pair<int, pair<int, double> > > choose_optimum;
	vector<pair<int, pair< double, pair<double, int> > > > scores;
	double c11, c12;
	int pos_ins;
	for (int i = 1; i < customer_list.size(); i++) {
		if (remaining_cus[i] == 1 && rem_demand - customer_list[i].q >= 0) {
			double minimum = _MAX;
			for (int j = 0; j < routes[route_number].size() - 1; j++) {
				if (check_feasible(route_number, routes[route_number][j].first, routes[route_number][j + 1].first, i, j + 1,routes) == 1) {
					c11 = dist(routes[route_number][j].first, i) + dist(i, routes[route_number][j + 1].first) - mikro * dist(routes[route_number][j].first, routes[route_number][j + 1].first);
					c12 = b_new(i, routes[route_number][j + 1].first, routes[route_number][j + 1].second.first) - routes[route_number][j + 1].second.first;
					if (minimum > alfa_1*c11 + alfa_2 * c12) {
						minimum = alfa_1 * c11 + alfa_2 * c12;
						d_0u = routes[route_number][j].second.second + dist(routes[route_number][j].first, i);
						pos_ins = j;
					}
				}
			}
			if (minimum != _MAX)scores.push_back(make_pair(i, make_pair(minimum, make_pair(d_0u, pos_ins))));
		}
	}
	if (scores.size() == 0)return choose_optimum;
	choose_optimum.resize(scores.size());
	for (int i = 0; i < scores.size(); i++) {
		choose_optimum[i] = make_pair(scores[i].first, make_pair(scores[i].second.second.second, lambda*scores[i].second.second.first - scores[i].second.first));
	}
	return choose_optimum;
}
int seed_customer(bool mode) {
	int id;
	if (mode == 0) {
		double d_max = 0;
		for (int i = 1; i < remaining_cus.size(); i++) {
			if (remaining_cus[i] == 1) {
				if (dist(customer_list[0].id, i) >= d_max) {
					d_max = dist(customer_list[0].id, i); id = i;
				}
			}
		}
	}
	else {
		int _min = _MAX;
		for (int i = 1; i < remaining_cus.size(); i++) {
			if (remaining_cus[i] == 1) {
				if (customer_list[i].l <= _min) {
					id = i; _min = customer_list[i].l;
				}
			}
		}
	}
	if (id == 0) {
		for (int i = 1; i < remaining_cus.size(); i++)
			if (remaining_cus[i] == 1) {
				remaining_cus[id] = 0; return i;
			}
		return -1;
	}
	remaining_cus[id] = 0;
	return id;
}
void insert_customer(int indeks_prec, int &id, int route_number,VVIFF &routes) {
	double b_ins = b_new(routes[route_number][indeks_prec].first, id, routes[route_number][indeks_prec].second.first);

	double a = dist(routes[route_number][indeks_prec].first, id), b = dist(routes[route_number][indeks_prec + 1].first, id);
	double d = routes[route_number][indeks_prec + 1].second.second - routes[route_number][indeks_prec].second.second;
	//aktualizacja dla wstawianego
	routes[route_number].insert(routes[route_number].begin() + indeks_prec + 1, make_pair(id, make_pair(b_ins, routes[route_number][indeks_prec].second.second + a)));
	customer_list[id].waiting_time = max(double(0), b_ins - b_waiting(routes[route_number][indeks_prec].first, id, routes[route_number][indeks_prec].second.first));

	double begin_time_new, diff = a + b - d;
	double PF,h,hh,PF_1;
	h=b_new(id,routes[route_number][indeks_prec+2].ST,b_ins);
	hh=routes[route_number][indeks_prec+2].ND.ST;
	PF=h-hh;
    customer_list[routes[route_number][indeks_prec+2].ST].waiting_time=max(double(0),h-b_waiting(routes[route_number][indeks_prec+1].ST,routes[route_number][indeks_prec+2].first,b_ins));

	for (int i = indeks_prec + 2; i < routes[route_number].size()-1; i++) {
		routes[route_number][i].second.first +=PF;
		routes[route_number][i].second.second += diff;
        PF_1=PF;
		PF=max(double(0),PF-customer_list[routes[route_number][i+1].ST].waiting_time);
		customer_list[routes[route_number][i+1].ST].waiting_time=max(double(0),-PF_1+customer_list[routes[route_number][i+1].ST].waiting_time);
	}
	routes[route_number][routes[route_number].size()-1].ND.ST+=PF;
	routes[route_number][routes[route_number].size()-1].ND.ND+=diff;
	remaining_cus[id] = 0;
}
VIFF init_route() {
	VIFF depots;
	depots.push_back(make_pair(0, make_pair(0, 0)));
	depots.push_back(make_pair(0, make_pair(0, 0)));
	return depots;
}
bool sorting_crit(pair<int, pair<int, double> > &a, pair<int, pair<int, double> > &b) {
	return a.second.second < b.second.second;
}
//rozne warianty optimum
//teoretycznie najlepsze
//czesciowo randomizowany
//calkowicie randomizowany
//teoretycznie najslabszy
int optimum(vector<pair<int, pair<int, double> > > &choose_optimum, int tryb) {
	sort(choose_optimum.begin(), choose_optimum.end(), sorting_crit);
	srand(time(0));
	if (tryb == 0) {
		return choose_optimum.size() - 1;
	}
	if (tryb == 1) {
		int s = rand() % choose_optimum.size();
		return s;
	}
	if (tryb == 2) {
		int width = choose_optimum.size() / 3;
		if (width == 0)return choose_optimum.size() - 1;
		else {
			int s = (choose_optimum.size() - width) + rand() % width;
			return s;
		}
	}
	return 0;
}
void initial_algorithm(double mikro, double lambda, double alfa_1, double alfa_2, int tryb, bool mode,VVIFF &routes) {
	remaining_cus.resize(customer_list.size());
	for(int i=0;i<remaining_cus.size();i++)remaining_cus[i]=1;
	remaining_cus[0]=0;
	int rem_cus = remaining_cus.size() - 1, rem_demand;
	vector<pair<int, pair< double, double> > > tour;
	vector<pair<int, pair<int, double> > > objective_function;
	while (rem_cus > 0) {
		tour = init_route();
		routes.push_back(tour);
		rem_demand = capacity;
		int seed = seed_customer(mode);
		insert_customer(customer_list[0].id, seed, routes.size() - 1,routes); rem_cus--; rem_demand -= customer_list[seed].q;
		objective_function = c1and2(routes,alfa_1, alfa_2, mikro, routes.size() - 1, lambda, rem_demand);
		while (objective_function.size() > 0) {
			int candidate_indeks = optimum(objective_function, tryb);
			insert_customer(objective_function[candidate_indeks].second.first, objective_function[candidate_indeks].first, routes.size() - 1,routes);//pomocy
			rem_cus--;
			rem_demand -= customer_list[objective_function[candidate_indeks].first].q;
			/////
			objective_function = c1and2(routes,alfa_1, alfa_2, mikro, routes.size() - 1, lambda, rem_demand);
		}
	}
}
//funkcje do metaheurystyki
double objective_function_annealing(VVIFF &solution) {
    return (solution[solution.size()-1][0].ND.ND)*beta1+(solution[solution.size()-1][1].ND.ND)*beta2+(solution[solution.size()-1][2].ND.ND)*beta3;
}
double delta_cost(VVIFF &current_solution){
    int a=current_solution.size()-1;
    return beta1*current_solution[a][0].ND.ST+beta2*current_solution[a][1].ND.ST+beta3*current_solution[a][2].ND.ST;
}
double probability(double &delta) {
	return exp(-delta / Temperature);
}
/////////////////////////////////////////////////////////////////////////////implemented
//ruchy w sasiedztwie
bool delete_insert(VVIFF &solution,int &a_route,int &b_route,int &a,int &b){//a i b to indeksy,a indeks usuwany, b-wstawiany
    //sprawdzmy czy pojemnosc jest dozwolona
    if(solution[a_route][a].ST==0)return 0;
    if(customer_list[solution[a_route][a].ST].q>remaining_capacity[b_route])return 0;
    //a_route i b_route sa rozne
    //najpierw usunmy a
    double pushforward_ina,pushforward_outa;
    pushforward_ina=b_new(solution[a_route][a-1].ST,solution[a_route][a+1].ST,solution[a_route][a-1].ND.ST)-solution[a_route][a+1].ND.ST;


    if(range_feasible(solution,a_route,a+1,solution[a_route].size(),pushforward_outa,pushforward_ina)){
            //teraz wstawiamy do trasy b
            double pushforward_inb,pushforward_outb,help;
            //nowy czas wstawianego klienta a
            help=b_new(solution[b_route][b-1].ST,solution[a_route][a].ST,solution[b_route][b-1].ND.ST);

            //sprawdzic czy mozliwe
            if(help>customer_list[solution[a_route][a].ST].l)return 0;
            pushforward_inb=b_new(solution[a_route][a].ST,solution[b_route][b].ST,help)-solution[b_route][b].ND.ST;

            if(range_feasible(solution,b_route,b,solution[b_route].size(),pushforward_outb,pushforward_inb)){
            // to znaczy ze jest mozliwe
            // czyli trzeba zaktualizowac
            //najpierw calkowity koszt
            solution[solution.size()-1][1].ND.ST=(pushforward_outa+pushforward_outb);
            //teraz dystans
            double db,da=-(solution[a_route][a+1].ND.ND-solution[a_route][a-1].ND.ND)+dist(solution[a_route][a-1].ST,solution[a_route][a+1].ST);
            db=-(solution[b_route][b-1].ND.ND-solution[b_route][b].ND.ND);
            db+=(dist(solution[b_route][b-1].ST,solution[a_route][a].ST)+dist(solution[a_route][a].ST,solution[b_route][b].ST));
            solution[solution.size()-1][2].ND.ST=(db+da);//jesli mamy dodatnie to mamy gorszy wynik;
            if(solution[a_route].size()==3){
                solution[solution.size()-1][0].ND.ST=-1;
            }
            return 1;
        }
        return 0;
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void exe_delete_insert(VVIFF &solution,int &a_route,int &b_route,int &a,int &b){//a i b to indeksy,a indeks usuwany, b-wstawiany
    //najpierw wstawmy a do trasy b
    IFF aa=solution[a_route][a];
    solution[b_route].insert(solution[b_route].begin()+b,aa);//wstawienie
    //aktualizacja wstawionego klienta
    solution[b_route][b].ND.ST=b_new(solution[b_route][b-1].ST,aa.ST,solution[b_route][b-1].ND.ST);//czas rozpoczecia pracy
    solution[b_route][b].ND.ND=solution[b_route][b-1].ND.ND+dist(solution[b_route][b-1].ST,aa.ST);//dystans
    customer_list[aa.ST].waiting_time=max(double(0),solution[b_route][b].ND.ST-b_waiting(solution[b_route][b-1].ST,aa.ST,solution[b_route][b-1].ND.ST));

    double pushforward_intb,pushforward_outb,h;
    h=b_new(solution[b_route][b].ST,solution[b_route][b+1].ST,solution[b_route][b].ND.ST);
    pushforward_intb=h-solution[b_route][b+1].ND.ST;
    //aktualizacja b+1 waiting time
    if(solution[b_route][b+1].ST!=0)//jesli jest rozny od magazynu
    customer_list[solution[b_route][b+1].ST].waiting_time=max(double(0),h-b_waiting(solution[b_route][b].ST,solution[b_route][b+1].ST,solution[b_route][b].ND.ST));
    //roznica odleglosci
    double db=dist(solution[b_route][b-1].ST,aa.first)+dist(aa.first,solution[b_route][b+1].ST);
    db-=(solution[b_route][b+1].ND.ND-solution[b_route][b-1].ND.ND);

    exe_range_feasible(solution,b_route,b+1,solution[b_route].size(),pushforward_outb,pushforward_intb,db);

    //teraz usuwamy
    solution[a_route].erase(solution[a_route].begin()+a);
    double pushforward_inta, pushforward_outa;
    h=b_new(solution[a_route][a-1].ST,solution[a_route][a].ST,solution[a_route][a-1].ND.ST);
    pushforward_inta=h-solution[a_route][a].ND.ST;
    double da=dist(solution[a_route][a-1].ST,solution[a_route][a].ST);
    da-=(solution[a_route][a].ND.ND-solution[a_route][a-1].ND.ND);
    //aktualizacja waiting time
    if(solution[a_route][a].ST!=0)//jesli jest rozny od magazynu
    customer_list[solution[a_route][a].ST].waiting_time=max(double(0),h-b_waiting(solution[a_route][a-1].ST,solution[a_route][a].ST,solution[a_route][a-1].ND.ST));

    exe_range_feasible(solution,a_route,a,solution[a_route].size(),pushforward_outa,pushforward_inta,da);

    //nalezy zaktualizowac dane nowego rozwiazania
    solution[solution.size()-1][1].ND.ND+=(pushforward_outa+pushforward_outb);//cout<<"oush: "<<(pushforward_outa+pushforward_outb)<<endl;
    solution[solution.size()-1][2].ND.ND+=(da+db);

    remaining_capacity[a_route]+=customer_list[solution[b_route][b].ST].q;
    remaining_capacity[b_route]-=customer_list[solution[b_route][b].ST].q;

    if(solution[a_route].size()==2){
        solution[solution.size()-1][0].ND.ND--;
        solution.erase(solution.begin()+a_route);
        remaining_capacity.erase(remaining_capacity.begin()+a_route);
    }
    //aktualizacja pojemnosci na trasach

    //zerujemy
    solution[solution.size()-1][0].ND.ST=0;
    solution[solution.size()-1][1].ND.ST=0;
    solution[solution.size()-1][2].ND.ST=0;
    //oby dzialalo
}
////////////////////////////////////////////////////////////////////////////////////////////////////implemented
bool one_to_one(VVIFF &solution,int &a_route,int &b_route,int &a,int &b){
    if(solution[a_route][a].ST==0||solution[b_route][b].ST==0)return 0;

    //a_route jest rozne od b_route
    //sprawdzamy pozostala pojemnosc
    if(customer_list[solution[a_route][a].ST].q>remaining_capacity[b_route]+customer_list[solution[b_route][b].ST].q)return 0;
    if(customer_list[solution[b_route][b].ST].q>remaining_capacity[a_route]+customer_list[solution[a_route][a].ST].q)return 0;
    //najpierw a
    double helpa,helpb;
    helpa=b_new(solution[b_route][b-1].ST,solution[a_route][a].ST,solution[b_route][b-1].ND.ST);
    if(helpa>customer_list[solution[a_route][a].ST].l)return 0;

    helpb=b_new(solution[a_route][a-1].ST,solution[b_route][b].ST,solution[a_route][a-1].ND.ST);
    if(helpb>customer_list[solution[b_route][b].ST].l)return 0;

    double pushforward_inb=b_new(solution[a_route][a].ST,solution[b_route][b+1].ST,helpa)-solution[b_route][b+1].ND.ST;
    double pushforward_ina=b_new(solution[b_route][b].ST,solution[a_route][a+1].ST,helpb)-solution[a_route][a+1].ND.ST;
    double pushforward_outa,pushforward_outb;
    if(range_feasible(solution,a_route,a+1,solution[a_route].size(),pushforward_outa,pushforward_ina)){
        if(range_feasible(solution,b_route,b+1,solution[b_route].size(),pushforward_outb,pushforward_inb)){
            //jesli jest mozliwe to nalezy zaktualizowac
            //przechowujemy delte
            solution[solution.size()-1][1].ND.ST=pushforward_outa+pushforward_outb;
            double da,db;
            da=dist(solution[a_route][a-1].ST,solution[b_route][b].ST)+dist(solution[b_route][b].ST,solution[a_route][a+1].ST);
            da-=(solution[a_route][a+1].ND.ND-solution[a_route][a-1].ND.ND);
            db=dist(solution[b_route][b-1].ST,solution[a_route][a].ST)+dist(solution[a_route][a].ST,solution[b_route][b+1].ST);
            db-=(solution[b_route][b+1].ND.ND-solution[b_route][b-1].ND.ND);
            solution[solution.size()-1][2].ND.ST=da+db;
            return 1;
        }
        return 0;
    }
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void exe_one_to_one(VVIFF &solution,int &a_route,int &b_route,int &a,int &b){
    // jest mozliwa zamiana
    IFF aa,bb;
    aa=solution[a_route][a],bb=solution[b_route][b];
    double pushforward_ina,pushforward_outa,pushforward_inb,pushforward_outb;
    double ha,hb;
    ha=b_new(solution[a_route][a-1].ST,bb.first,solution[a_route][a-1].ND.ST);
    hb=b_new(solution[b_route][b-1].ST,aa.first,solution[b_route][b-1].ND.ST);
    //usuwamy klientow a i b
    solution[a_route].erase(solution[a_route].begin()+a);
    solution[b_route].erase(solution[b_route].begin()+b);
    //wstawiamy
    solution[a_route].insert(solution[a_route].begin()+a,bb);
    solution[b_route].insert(solution[b_route].begin()+b,aa);
    //aktualizujemy aa i bb
    //najpierw czasy
    solution[a_route][a].ND.ST=ha;
    solution[b_route][b].ND.ST=hb;
    //odleglosci
    solution[a_route][a].ND.ND=solution[a_route][a-1].ND.ND+dist(solution[a_route][a-1].ST,bb.ST);
    solution[b_route][b].ND.ND=solution[b_route][b-1].ND.ND+dist(solution[b_route][b-1].ST,aa.ST);
    //waiting time
    customer_list[solution[a_route][a].ST].waiting_time=max(double(0),ha-b_waiting(solution[a_route][a-1].ST,bb.ST,solution[a_route][a-1].ND.ST));
    customer_list[solution[b_route][b].ST].waiting_time=max(double(0),hb-b_waiting(solution[b_route][b-1].ST,aa.ST,solution[b_route][b-1].ND.ST));
    //aktualizacja waiting time dla nastepnikow
    ha=b_new(bb.ST,solution[a_route][a+1].ST,solution[a_route][a].ND.ST);
    hb=b_new(aa.ST,solution[b_route][b+1].ST,solution[b_route][b].ND.ST);

    if(solution[a_route][a+1].ST!=0){
        customer_list[solution[a_route][a+1].ST].waiting_time=max(double(0),ha-b_waiting(bb.ST,solution[a_route][a+1].ST,solution[a_route][a].ND.ST));
    }
    if(solution[b_route][b+1].ST!=0){
        customer_list[solution[b_route][b+1].ST].waiting_time=max(double(0),hb-b_waiting(aa.ST,solution[b_route][b+1].ST,solution[b_route][b].ND.ST));
    }
    pushforward_ina=ha-solution[a_route][a+1].ND.ST;
    pushforward_inb=hb-solution[b_route][b+1].ND.ST;
    //zmiana odleglosci
    double da,db;
    da=dist(solution[a_route][a-1].ST,solution[a_route][a].ST)+dist(solution[a_route][a].ST,solution[a_route][a+1].ST);
    da-=(solution[a_route][a+1].ND.ND-solution[a_route][a-1].ND.ND);
    db=dist(solution[b_route][b-1].ST,solution[b_route][b].ST)+dist(solution[b_route][b].ST,solution[b_route][b+1].ST);
    db-=(solution[b_route][b+1].ND.ND-solution[b_route][b-1].ND.ND);
    //aktualizujemy
    exe_range_feasible(solution,a_route,a+1,solution[a_route].size(),pushforward_outa,pushforward_ina,da);
    exe_range_feasible(solution,b_route,b+1,solution[b_route].size(),pushforward_outb,pushforward_inb,db);
    // mozna zaktualizowac dane nowego rozwiazania
    solution[solution.size()-1][1].ND.ND+=(pushforward_outa+pushforward_outb);
    solution[solution.size()-1][2].ND.ND+=(da+db);
    //zerujemy
    solution[solution.size()-1][1].ND.ST=0;
    solution[solution.size()-1][2].ND.ST=0;
    solution[solution.size()-1][0].ND.ST=0;

    //aktualizacja remaining capacity
    remaining_capacity[a_route]+=(customer_list[aa.ST].q-customer_list[bb.ST].q);
    remaining_capacity[b_route]+=(customer_list[bb.ST].q-customer_list[aa.ST].q);
}
void set_params(double &cost_inc){
    PINIT=0.95;
    Temperature=(-cost_inc)/log(PINIT);
    INITEMP=Temperature;
    epoch_len=10000000;
    RATIO=0.95;
    MINPERCENT=0.01;
    TIME=2*customer_list.size();
}
bool if_accepted(double p){
    if(((double)rand()/RAND_MAX)<p)return 1;
    return 0;
}
void set_tabu_list(VVIFF &solution){
    tabu_list.resize(customer_list.size());
    for(int i=0;i<tabu_list.size();i++)tabu_list[i].resize(solution.size()-1);
    for(int i=0;i<tabu_list.size();i++){
        for(int j=0;j<tabu_list[j].size();j++){
            tabu_list[i][j]=0;
        }
    }
}
void actualize_tabu_list(VVIFF &solution,int &time){
    for(int i=0;i<tabu_list.size();i++){
        for(int j=0;j<tabu_list[i].size();j++){
            tabu_list[i][j]=max(0,tabu_list[i][j]-time);
        }
    }
    time=0;
}
bool tabu_move(VVIFF &solution,int &move_number,int &a_route,int &b_route,int &a,int &b,int &__time){
    if(move_number==0){
        if(tabu_list[solution[a_route][a].ST][b_route]>__time||tabu_list[solution[b_route][b].ST][a_route]>__time)return 1;
        return 0;
    }
    else {
        if(tabu_list[solution[a_route][a].ST][b_route]>__time)return 1;
        return 0;
    }
    return 0;
}
bool next_move(VVIFF &solution,int &move_number,int &a_route,int &b_route,int &a,int &b,int &time){
                move_number=rand()%2;
                double d,p;
            //przegladamy wszystkie pary tras
            // stosujemy strategie pierwszego ulepszenia
            for(int _i=current_solution.size()-2;_i>=1;_i--){//trasa a
                for(int __i=0;__i<_i;__i++){//trasa b
                        for(int _j=1;_j<current_solution[_i].size();_j++){//klient a
                            for(int __j=1;__j<current_solution[__i].size();__j++){//klient b
                                a_route=_i;
                                b_route=__i;
                                a=_j;
                                b=__j;
                                if(move_number==0){
                                    if(one_to_one(current_solution,a_route,b_route,a,b)){
                                        if(tabu_move(current_solution,move_number,a_route,b_route,a,b,time)){
                                            d=delta_cost(current_solution);
                                            if(d<=epsilon){
                                                    tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                    tabu_list[solution[b_route][b].ST][b_route]=time+tabu_period;
                                                    return 1;
                                            }
                                            //w przeciwnym wypadku mamy
                                            p=probability(d)*(1-k1)*(1-k2);
                                            if(if_accepted(p)){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                tabu_list[solution[b_route][b].ST][b_route]=time+tabu_period;
                                                return 1;
                                            }
                                        }
                                        else{
                                            d=delta_cost(current_solution);
                                            if(d<=epsilon){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                tabu_list[solution[b_route][b].ST][b_route]=time+tabu_period;
                                                return 1;
                                            }
                                            p=probability(d);
                                            if(if_accepted(p)){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                tabu_list[solution[b_route][b].ST][b_route]=time+tabu_period;
                                                return 1;
                                            }
                                        }
                                    }
                                }
                                else{
                                    if(delete_insert(current_solution,a_route,b_route,a,b)){
                                        if(tabu_move(current_solution,move_number,a_route,b_route,a,b,time)){
                                            d=delta_cost(current_solution);
                                            if(d<=epsilon){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                return 1;
                                            }
                                            //w przeciwnym wypadku mamy
                                            p=probability(d)*(1-k1)*(1-k2);
                                            if(if_accepted(p)){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                return 1;
                                            }
                                        }
                                        else{
                                            d=delta_cost(current_solution);
                                            if(d<=epsilon){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                return 1;
                                            }
                                            p=probability(d);
                                            if(if_accepted(p)){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                return 1;
                                            }
                                        }
                                    }
                                    else if(delete_insert(current_solution,b_route,a_route,b,a)){
                                        a_route=__i;
                                        b_route=_i;
                                        a=__j;
                                        b=_j;
                                        if(tabu_move(current_solution,move_number,a_route,b_route,a,b,time)){
                                            d=delta_cost(current_solution);
                                            if(d<=epsilon){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                return 1;
                                            }
                                            //w przeciwnym wypadku mamy
                                            p=probability(d)*(1-k1)*(1-k2);
                                            if(if_accepted(p)){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                return 1;
                                            }
                                        }
                                        else{
                                            d=delta_cost(current_solution);
                                            if(d<=epsilon){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                return 1;
                                            }
                                            p=probability(d);
                                            if(if_accepted(p)){
                                                tabu_list[solution[a_route][a].ST][a_route]=time+tabu_period;
                                                return 1;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            return 0;
}
void add_score_to_solution(VVIFF &solution){
    VIFF last;
    double cost=0,d=0;
    for(int i=0;i<solution.size();i++){
        cost+=solution[i][solution[i].size()-1].ND.ST;
        d+=solution[i][solution[i].size()-1].ND.ND;
    }
    last.push_back(make_pair(0,make_pair(double(0),solution.size())));
    last.push_back(make_pair(0,make_pair(double(0),cost)));
    last.push_back(make_pair(0,make_pair(double(0),d)));
    solution.push_back(last);
    //uzupelnienie vectora pozostalej pojemnosci
    remaining_capacity.clear();
    remaining_capacity.resize(solution.size()-1);
    int sum;
    for(int i=0;i<solution.size()-1;i++){
            sum=0;
        for(int j=1;j<solution[i].size()-1;j++){
            sum+=customer_list[solution[i][j].ST].q;
        }
        remaining_capacity[i]=capacity-sum;
    }
}
void aktualizuj_trasy(VVIFF &solution){
    for(int i=0;i<solution.size()-1;i++){
        for(int j=1;j<solution[i].size();j++){
            solution[i][j].ND.ST=b_new(solution[i][j-1].ST,solution[i][j].ST,solution[i][j-1].ND.ST);
            if(j<solution[i].size()-1)customer_list[solution[i][j].ST].waiting_time=max(double(0),solution[i][j].ND.ST-b_waiting(solution[i][j-1].ST,solution[i][j].ST,solution[i][j-1].ND.ST));
            solution[i][j].ND.ND+=dist(solution[i][j-1].ST,solution[i][j].ST);
        }
    }
    solution.erase(solution.begin()+solution.size());
    add_score_to_solution(solution);
}
bool check_equal(VVIFF &current_solution,VVIFF &solution,fstream &awaryjne){
    for(int i=0;i<solution.size()-1;i++){
        for(int j=1;j<solution[i].size();j++){
            solution[i][j].ND.ST=b_new(solution[i][j-1].ST,solution[i][j].ST,solution[i][j-1].ND.ST);
            if(abs(current_solution[i][j].ND.ST-solution[i][j].ND.ST)>epsilon){
                awaryjne<<"czas przybycia dla klienta nr: "<<i<<" "<<current_solution[i].size()<<" "<<current_solution[i][j].ND.ST-solution[i][j].ND.ST<<endl;
            }
            double w;
            if(j<solution[i].size()-1)w=max(double(0),solution[i][j].ND.ST-b_waiting(solution[i][j-1].ST,solution[i][j].ST,solution[i][j-1].ND.ST));
            if(abs(customer_list[current_solution[i][j].ST].waiting_time-w)>epsilon){
                //awaryjne<<"waiting_time u klienta nr: "<<current_solution[i][j].ST<<endl;
            }
            solution[i][j].ND.ND+=dist(solution[i][j-1].ST,solution[i][j].ST);
            if(abs(current_solution[i][j].ND.ND-solution[i][j].ND.ND)>epsilon){
                //awaryjne<<"odleglosc od magazynu dla klienta nr: "<<current_solution[i][j].ST<<current_solution[i][j].ND.ND-solution[i][j].ND.ND<<endl;
            }
        }
    }
   return 1;
}
void simulated_annealing(fstream &awaryjne){
    srand(time(0));
    initial_algorithm(1,2,0,1,3,1,initial_solution);
    add_score_to_solution(initial_solution);
   // add_score_to_solution(initial_solution);
    best_solution=initial_solution;
    current_solution=initial_solution;

    //awaryjne<<initial_solution.size()-1<<" "<<initial_solution[initial_solution.size()-1][1].ND.ND<<" ";
    show_solution(best_solution,awaryjne);
    //wykonujemy losowe ruchy
    int module,move_number,a_route,b_route,a,b;//zmienne do wykonywania ruchow
    double cost_inc=0;//do liczenia sredniej
    double P,delta;//prawdopodobienstwo i delta
    int t_t=0,h_h=0;

    while(t_t<random_moves_begin&&current_solution.size()>2){
            h_h++;
        //losujemy trasy
        move_number=rand()%2;
        a_route=rand()%(current_solution.size()-1);
        b_route=rand()%(current_solution.size()-1);

        if(current_solution.size()-1>1)
        while(a_route==b_route)b_route=rand()%(current_solution.size()-1);
        else break;
        //losowanie klientow
        a=rand()%(current_solution[a_route].size()-2)+1;
        b=rand()%(current_solution[b_route].size()-2)+1;
        if(move_number==0){
            if(one_to_one(current_solution,a_route,b_route,a,b)){
                    t_t++;//ruch zaakceptowany
                delta=delta_cost(current_solution);
                cost_inc+=abs(delta);
                    exe_one_to_one(current_solution,a_route,b_route,a,b);
            }
        }
        else{
            if(delete_insert(current_solution,a_route,b_route,a,b)){
                    t_t++;
                delta=delta_cost(current_solution);
                cost_inc+=abs(delta);
                    exe_delete_insert(current_solution,a_route,b_route,a,b);
                }
        }
        if(h_h>1000){
            h_h=0;t_t++;
        }
    }
    if(abs(cost_inc)<epsilon)cost_inc=1.02;
    cost_inc/=random_moves_begin;
    //trzeba ustawic zmienne
    set_params(cost_inc);

    chrono::time_point<chrono::system_clock> start,_end;
    start=chrono::system_clock::now();
    _end=chrono::system_clock::now();
    chrono::duration<double> elapsed_time;
    elapsed_time=_end-start;
    int __counter=0,rejected,opt_constant,__time=0;
    double FINDIVISOR=50000;
    set_tabu_list(current_solution);
    double seconds=0;
    //cout<<current_solution.size()<<endl;
    while(Temperature>=(INITEMP/FINDIVISOR)&&current_solution.size()>2){
            __counter=0;rejected=-1;opt_constant=0;
        while(opt_constant<TIME&&__counter*MINPERCENT<=(__counter-rejected)&&current_solution.size()>2&&elapsed_time.count()<2800){
            if(next_move(current_solution,move_number,a_route,b_route,a,b,__time)){
                if(move_number==0){
                    exe_one_to_one(current_solution,a_route,b_route,a,b);
                }
                else exe_delete_insert(current_solution,a_route,b_route,a,b);

                    if(objective_function_annealing(current_solution)<objective_function_annealing(best_solution)){
                        best_solution=current_solution;
                        opt_constant=0;
                    }
                    else opt_constant++;
            }
            else {rejected++;opt_constant++;}
            __counter++;

            _end=chrono::system_clock::now();
            elapsed_time=_end-start;
            __time++;
        }
        cout<<"T: "<<Temperature<<" "<<best_solution.size()-1<<" "<<fixed<<setprecision(2)<<best_solution[best_solution.size()-1][1].ND.ND<<" "<<fixed<<setprecision(2)<<current_solution[current_solution.size()-1][1].ND.ND<<endl;
        //zmiana temperatury
        Temperature*=RATIO;
        if(__time>MAXVALUE)actualize_tabu_list(current_solution,__time);
    }
}
int main(int argc,char* argv[])
{
    srand(time(0));
    fstream we,wy,wyjscie_awaryjne;
     double x_,y_,num_customers;
     double id_,q_,e_,d_,l_;
    string napis,name_of_problem;
    float table_best[6];
    if(argc==2){
               we.open(argv[1],ios::in);
                if(!we.good()){cout<<"blad otwarcia pliku "<<endl;}
                else{
                    we>>name_of_problem;
                    wy.open("wynik.txt",ios::out);
                    //wyjscie_awaryjne.open("awaria.txt",ios::out);
                    we>>napis>>napis>>napis;
                    we>>vehicles>>capacity;
                    we>>napis;
                    for(int j=0;j<11;j++)we>>napis;
                    customer c;
                    customer_list.clear();
                    while(!we.eof()){
                        we>>id_>>x_>>y_>>q_>>e_>>l_>>d_;
                        c.id=id_;
                        c.x=x_;
                        c.y=y_;
                        c.q=q_;
                        c.e=e_;
                        c.d=d_;
                        c.l=l_;
                        c.waiting_time=0;
                        customer_list.push_back(c);
                    }
                    //ostatni moze byc powielony zatem
                    if(customer_list[customer_list.size()-1].id==customer_list[customer_list.size()-2].id)
                    customer_list.pop_back();
                    //sprawdzenie dopuszczalnosci
                    if(check_possible()==0){
                        wy<<"-1"<<endl;
                        return 0;
                    }
                    //wywolujemy metaheurystyke
                    simulated_annealing(wyjscie_awaryjne);

                        wy<<best_solution[best_solution.size()-1][0].ND.ND<<" ";
                        wy<<fixed<<setprecision(5)<<best_solution[best_solution.size()-1][1].ND.ND<<endl;
                        for(int i=0;i<best_solution.size()-1;i++){
                            for(int j=1;j<best_solution[i].size()-1;j++){
                                wy<<best_solution[i][j].first<<" ";//cout<<best_solution[i][j].ND.ST<<" ";
                            }
                            wy<<endl;
                        }
                        wy.close();
                }
                we.close();
    }
    else cout<<"<nazwa_programu> <dane>"<<endl;
	return 0;
}
