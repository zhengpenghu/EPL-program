/*
 * Vector_PhaseC_unittests.cpp
 * EE380L - Spring 2015
 *
 * Tests for Vector_PhaseC are organized into three sections: PhaseC, PhaseC1, and
 * PhaseC2, corresponding to the C, C*, and C** requirements respectively. These tests
 * are independent, so you may comment out the ones that you are not utilizing, and
 * Google Test will run accordingly.
 *
 * These tests are not complete. Write additional tests on your own to test
 * the rest of the functionality of your program. The tests used to grade your
 * project will be more robust than those included in this file.
 */

#include <cstdint>
#include <stdexcept>
#include "gtest/gtest.h"
#include "Vector.h"

using epl::vector;

/*****************************************************************************************/
// Phase C Tests
/*****************************************************************************************/
#if defined(PHASE_C0_0) | defined(PHASE_C)
TEST (PhaseC, direct_init_list) {
    vector<int> w{ 0, 1, 2 };
    EXPECT_EQ(3, w.size());
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(i, w[i]);
    }
}
#endif

#if defined(PHASE_C0_1) | defined(PHASE_C)
TEST (PhaseC, begin_vector) {
    vector<int> x{ 1, 2, 3 };
    auto it = begin(x);
    const auto& it2 = end(x);
    EXPECT_EQ(1, *it);
}
#endif

/*****************************************************************************************/
// Phase C* Tests
/*****************************************************************************************/
#if defined(PHASE_C1_0) | defined(PHASE_C)
TEST (PhaseC1, range_for_loop) {
    vector<int32_t> x(10);

    int32_t k = 0;
    for (auto& v : x) {
        v = k++;
    }

    const vector<int32_t>& y = x;
    int32_t s = 0;
    for (const auto& v : y) {
        s += v;
    }

    EXPECT_EQ(45, s);
}
#endif

#if defined(PHASE_C1_1) | defined(PHASE_C)
TEST(PhaseC1, invalid_iterator) {
    vector<uint32_t> x(10);
    auto it = x.begin();
    x.pop_front();
    try {
        *it;
        FAIL();
    } catch (epl::invalid_iterator exc) {
        SUCCEED();
    } catch (...) {
        FAIL();
    }
}
#endif

/*****************************************************************************************/
// Phase C** Tests
/*****************************************************************************************/
#if defined(PHASE_C2_0) | defined(PHASE_C)
TEST (PhaseC2, ItrExceptSevere) {
    vector<int> x(1);
    auto itr = x.begin();
    x.pop_back();
    try{
        int a;
        a = *itr;
        FAIL();
        *itr = a;
    } catch (epl::invalid_iterator ii) {
        EXPECT_EQ(epl::invalid_iterator::SEVERE, ii.level);
    } catch (...) {
        FAIL();
    }
}
#endif

#if defined(PHASE_C2_1) | defined(PHASE_C)
TEST (PhaseC2, ItrExceptModerate) {
    vector<int> x(3), y{1,2,3};
    auto xi = x.begin();
    x = y;
    try {
        *xi = 5;
        FAIL();
    } catch (epl::invalid_iterator ex) {
        EXPECT_EQ(epl::invalid_iterator::MODERATE, ex.level);
    } catch (...) {
        FAIL();
    }
}
#endif

#if defined(PHASE_C2_2) | defined(PHASE_C)
TEST (PhaseC2, ItrExceptMild) {
    vector<int> x(3);
    auto itr = x.begin();
    x.pop_back();
    try {
        volatile int a;
        a = *itr;
        FAIL();
        *itr = a;
    } catch (epl::invalid_iterator ii) {
        EXPECT_EQ(epl::invalid_iterator::MILD, ii.level);
    } catch (...) {
        FAIL();
    }
}
#endif
