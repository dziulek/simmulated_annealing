#include "../src/customer.hpp"
#include <gtest/gtest.h>
#include <cmath>

TEST(customer, customer_distance_Test){
    Customer c1(0, 0, 10, 10, 10, 10, 10);
    Customer c2(1, 1, 10, 10, 10, 10, 10);

    ASSERT_FLOAT_EQ(Customer::dist(c1, c2), sqrt(2));
}