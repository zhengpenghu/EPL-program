/*
 * valarray_PhaseB_unittests.cpp
 * EPL - Spring 2015
 */

#include <chrono>
#include <complex>
#include <cstdint>
#include <future>
#include <iostream>
#include <stdexcept>

#include "InstanceCounter.h"
#include "Valarray.h"

#include "gtest/gtest.h"

using std::cout;
using std::endl;
using std::string;
using std::complex;

int InstanceCounter::counter = 0;

using namespace epl;

template <typename X, typename Y>
bool match(X x, Y y) {
    double d = x - y;
    if (d < 0) { d = -d; }
    return d < 1.0e-9; // not really machine epsilon, but close enough
}
/*********************************************************************/
// Phase B Tests
/*********************************************************************/
#if defined(PHASE_B1_0) | defined(PHASE_B)
TEST(PhaseB1, Sqrt) {
    valarray<int> v1(10);
    valarray<int> v4 = 4 + v1;

    valarray<float> v5 = v4.sqrt();
    for (uint64_t i = 0; i<10; i++) {
        EXPECT_EQ(2.0, v5[i]);
    }
}
#endif

#if defined(PHASE_B1_1) | defined(PHASE_B)
TEST(PhaseB1, Apply) {
    valarray<int> v1(10);
    valarray<int> v4 = 4 + v1;

    valarray<int> v7 = v4.apply(std::negate<int>());

    EXPECT_EQ(10, v7.size());

    for (uint64_t i = 0; i<10; i++) {
        EXPECT_EQ(-4, v7[i]);
    }
}
#endif

#if defined(PHASE_B1_2) | defined(PHASE_B)
TEST(PhaseB1, Accumulate) {
    valarray<int> v1{1, 2, 3, 4, 5};
    int sum = v1.accumulate(std::plus<int>());
    int product = v1.accumulate(std::multiplies<int>());
    EXPECT_EQ(15, sum);
    EXPECT_EQ(120, product);
}
#endif

#if defined(PHASE_B1_3) | defined(PHASE_B)
TEST(PhaseB1, Lazy) {
    // lazy evaluation test
    valarray <double> v1(10), v2(10), v3(10), v4;
      //auto x= v1+v2;
auto x2=v1-1;
    auto x3= v1*1;
    auto x4= v1/1;
    
    auto x5= 1+v1;
//    auto x6= 1-v1;
//    auto x7= 1*v1;
//    auto x8= 1/v1;
    cout<<"here";
    cout<<x3+x4+x5-x4/x2/2*3-4<<endl;
    Foo x{1,2};
    
    std::cout<<(Foo)x;
    //valarray<char> vvv;
    for (int i = 0; i<10; ++i) {
        v1.push_back(1.0);
        v2.push_back(1.0);
        v3.push_back(1.0);
        v4.push_back(1.0);
    }
    int cnt = InstanceCounter::counter;
    v1 + v2 - (v3 * v4);
    EXPECT_EQ(cnt, InstanceCounter::counter);

    valarray<double> ans(10);
    ans = v1 + v2 - (v3*v4);
    EXPECT_TRUE(match(ans[3], (v1[3] + v2[3] - (v3[3] * v4[3]))));
}
#endif
