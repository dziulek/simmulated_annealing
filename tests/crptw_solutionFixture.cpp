#include "gtest/gtest.h"
#include "solution.hpp"

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

TEST_F(CRPTW_SolutionFixture, add_delete_route){

    solution->addRoute();
    
    ASSERT_EQ(solution->getNOfRoutes(), 1);
}