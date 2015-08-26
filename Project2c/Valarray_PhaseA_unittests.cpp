/*
 * Valarray_PhaseA_unittests.cpp
 * EPL - Spring 2015
 */

#include <chrono>
#include <complex>
#include <cstdint>
#include <future>
#include <iostream>
#include <stdexcept>

//#include "valbackup.h"
#include "Valarray.h"
#include "gtest/gtest.h"

using std::cout;
using std::endl;
using std::string;
using std::complex;

using namespace epl;

template <typename X, typename Y>
bool match(X x, Y y) {
    double d = x - y;
    if (d < 0) { d = -d; }
    return d < 1.0e-9; // not really machine epsilon, but close enough
}

/*********************************************************************/
// Phase A Tests
/*********************************************************************/

#if defined(PHASE_A0_1) | defined(PHASE_A)
TEST(PhaseA, BracketOp) {
    valarray<complex<double>> x(10);
    for (int i = 0; i < 10; ++i)
        x[i] = complex<double>(i, i);
    
    
    valarray<complex<double>> y = x + x;

    
    for (int i = 0; i < 10; ++i) {
        complex<double> ans(i, i);
        ans += ans;
        EXPECT_TRUE(match(y[i].real(), ans.real()));
    }

    x = y;
    int i = 0;
    for (auto yi : x) {
        complex<double> ans(i, i);
        ++i;
        ans += ans;
        EXPECT_TRUE(match(yi.real(), ans.real()));
    }
}
#endif

#if defined(PHASE_A0_0) | defined(PHASE_A)
TEST(PhaseA, Operators) {
    valarray<int> x(10);
    valarray<int> y(20);
    x = -((2 * (1 + x - y + 1)) / 1);

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(-4, x[i]);
    }
}
#endif

#if defined(PHASE_A1_0) | defined(PHASE_A)
TEST(PhaseA1, Complex) {
    valarray<complex<float>> x(10);
    valarray<double> y = { 1.5, 2.5, 3.5 };

    for (int i = 0; i < 10; i++)
        x[i] = complex<float>(0.0, 0.0);

    valarray<complex<double>> z = y + x;
    valarray<complex<double>> r = y + y + z;
    EXPECT_EQ(r[0], complex<double>(4.5, 0));
    EXPECT_EQ(r[1], complex<double>(7.5, 0));
    EXPECT_EQ(r[2], complex<double>(10.5, 0));
}
#endif

#if defined(PHASE_A1_1) | defined(PHASE_A)
TEST(PhaseA1, OSTREAM) {
//    valarray<complex<float>> x(10);
//    valarray<double> y = { 1.5, 2.5, 3.5 };
//    std::complex<int> a {2,2};
    
    valarray <double> v1(10);
    valarray <double> v2(10);
    //, v2, v3, v4;
    valarray<double> x= v1+v2;

    //valarray<complex<double>> xx=x+y;
    cout<<x<<endl;
    //cout<<y<<endl;
    //cout<<a;
    //cout<<"a+a="<<xx;
}
#endif

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto out = RUN_ALL_TESTS();

#ifdef _MSC_VER
    system("pause");
#endif

    return out;
}
