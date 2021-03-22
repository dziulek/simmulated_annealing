#include "gtest/gtest.h"
#include "../src/customer.hpp"
#include "../src/route.hpp"

TEST(customer, customer_distance_Test){
    Customer c1(1, 0, 0, 10, 10, 10, 10);
    Customer c2(2, 1, 1, 10, 10, 10, 10);

    ASSERT_FLOAT_EQ(Customer::dist(c1, c2), sqrt(2));
}


int main(int argc, char * argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}