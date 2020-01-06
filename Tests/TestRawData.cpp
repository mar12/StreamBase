#include "TestRawData.h"
#include <limits>
#include <iostream>
#include "../Utility/RawData.h"

namespace Tests {

using Utility::RawData;

static bool testRawDataInt() {
    bool success = true;
    /// Positive test cases
    int test_values[] = { 0, -1, 1, -7, 12, 321, std::numeric_limits<int>::min(), std::numeric_limits<int>::max() };
    for (int i : test_values) {
        RawData data = RawData::fromValue(i);
        int i2 = 0xEE;
        if (!data.toValue(i2)) {
            std::cerr << __func__ << " - error: not able to convert value " << i << std::endl;
            success = false;
        } else if (i2 != i) {
            std::cerr << __func__ << " - error: value " << i << "converted to " << i2 << std::endl;
            success = false;
        }
    }
    /// Negative test case
    std::string not_a_number = "Hello";
    RawData data = RawData::fromValue(not_a_number);
    int i;
    if (data.toValue(i) == true) {
        std::cerr << __func__ << " - error: data " << not_a_number << " successfully converted to " << i << std::endl;
        success = false;
    }
    return success;
}

static bool testRawDataString() {
    bool success = true;
    std::string test_values[] = { "", "Hello, World!!!", "\naB\tc\0D e\r\0" };
    for (const std::string& test_string: test_values) {
        RawData data = RawData::fromValue(test_string);
        std::string s;
        if (!data.toValue(s)) {
            success = false;
            std::cerr << __func__ << " - error: function `RawData.toValue` returned false" << std::endl;
        } else if (s != test_string) {
            std::cerr << __func__ << " - error: string " << test_string << " converted to " << s << std::endl;
            success = false;
        }
    }
    return success;
}

bool testRawData() {
    bool success = true;
    if (!testRawDataString()) {
        success = false;
    }
    if (!testRawDataInt()) {
        success = false;
    }
    return success;
}

}