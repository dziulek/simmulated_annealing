#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP






class Customer{

    public:

        int id;
        float x; //x coordinate
        float y; //y coordinate
        float q; //
        float e; //time of window opening
        float l; //time of window closing
        float d; 

        Customer(float x, float y, float q, float e, float l, float d, float waiting_time) : x(x), y(y), q(q), e(e), l(l), d(d){};
        static inline float dist(const Customer &c1,const Customer &c2);

};

#endif