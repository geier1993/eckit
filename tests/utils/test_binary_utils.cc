/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <cstring>
#include <iostream>
#include <memory>
#include <set>

#include "eckit/utils/BinaryUtils.h"

#include "eckit/testing/Test.h"

using namespace std;
using namespace eckit;
using namespace eckit::testing;


namespace eckit {
namespace test {

//----------------------------------------------------------------------------------------------------------------------

CASE("Test nextPowerOf2 and bitwidth") {
    {
        const auto testNP2AndBitWidth = [](std::size_t i, std::size_t np2, std::size_t bw) {
            EXPECT_EQUAL(nextPowOf2(i), np2);
            EXPECT_EQUAL(bitWidth(i), bw);
        };

        // Test something explicit
        testNP2AndBitWidth(0, 0, 0);
        testNP2AndBitWidth(1, 1, 1);
        testNP2AndBitWidth(2, 2, 2);
        testNP2AndBitWidth(3, 4, 2);
        testNP2AndBitWidth(4, 4, 3);
        testNP2AndBitWidth(5, 8, 3);
        testNP2AndBitWidth(6, 8, 3);
        testNP2AndBitWidth(7, 8, 3);
        testNP2AndBitWidth(8, 8, 4);
        testNP2AndBitWidth(9, 16, 4);
        testNP2AndBitWidth(10, 16, 4);
        testNP2AndBitWidth(11, 16, 4);
        testNP2AndBitWidth(12, 16, 4);
        testNP2AndBitWidth(13, 16, 4);
        testNP2AndBitWidth(14, 16, 4);
        testNP2AndBitWidth(15, 16, 4);
        testNP2AndBitWidth(16, 16, 5);
        testNP2AndBitWidth(17, 32, 5);
        testNP2AndBitWidth(18, 32, 5);
        testNP2AndBitWidth(30, 32, 5);
        testNP2AndBitWidth(31, 32, 5);
        testNP2AndBitWidth(32, 32, 6);
        testNP2AndBitWidth(33, 64, 6);
        testNP2AndBitWidth(34, 64, 6);
        testNP2AndBitWidth(62, 64, 6);
        testNP2AndBitWidth(63, 64, 6);
        testNP2AndBitWidth(64, 64, 7);
        testNP2AndBitWidth(65, 128, 7);
        testNP2AndBitWidth(66, 128, 7);
        testNP2AndBitWidth(126, 128, 7);
        testNP2AndBitWidth(127, 128, 7);
        testNP2AndBitWidth(128, 128, 8);

        // Test whole integer range for a "small" integer to test limit behaviour
        using INTT = uint16_t; // testing a whole uint32_t will take too long
        for (INTT s = 0; s < (sizeof(INTT) * CHAR_BIT); ++s) {
            INTT s1 = (1UL << s);
            INTT s2 = (1UL << (s+1));
            for (INTT i = s1; i < s2; ++i) {
                testNP2AndBitWidth(i, (i == s1) ? s1 : s2, s+1);
            }
        }
    }
}


//----------------------------------------------------------------------------------------------------------------------

}  // end namespace test
}  // end namespace eckit

int main(int argc, char* argv[]) {
    return run_tests(argc, argv);
}
