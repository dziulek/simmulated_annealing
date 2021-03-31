#include "gtest/gtest.h"
#include "../src/main/solution.hpp"

class CRPTW_SolutionFixture : public ::testing::Test{

public:

    CRPTW_SolutionFixture(){
        pi = new ProviderInfo{0, 0, 0, 0, 0};
        solution = new CRPTW_Solution(*pi);
        
    }
    ~CRPTW_SolutionFixture(){
        delete solution;
        delete pi;
    }

    CRPTW_Solution * solution;
    ProviderInfo * pi;
    
};

TEST_F(CRPTW_SolutionFixture, copy_constructor){

    CRPTW_Solution temp_solution =  *solution;

    temp_solution.addRoute();
    ASSERT_EQ(solution->getNOfRoutes(), 0);
    ASSERT_EQ(temp_solution.getNOfRoutes(), 1);
    //to do check if temp_solution is deep copy of solution
}

TEST_F(CRPTW_SolutionFixture, add_delete_route){

    solution->addRoute();
    
    ASSERT_EQ(solution->getNOfRoutes(), 1);
}