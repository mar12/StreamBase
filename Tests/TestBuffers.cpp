#include "TestBuffers.h"
#include <iostream>
#include <cstring>
#include "Utility/InputBuffer.h"
#include "Utility/OutputBuffer.h"

namespace Tests {

bool testBuffers() {

    using namespace Utility;

    /// Create output buffer
    OutputBuffer out;

    /// Output data to output buffer
    int i1 = 123;
    std::string s1 = "Hello, World!";
    char raw_data[] = { 'a', 'b', 'c' };
    RawData d1(sizeof(raw_data), raw_data);
    out << i1 << s1 << d1;

    /// Convert output buffer to input buffer
    InputBuffer in(out.size(), out.data());

    /// Read data from input buffer
    int i2 = 0;
    std::string s2 = "";
    RawData d2;
    in >> i2 >> s2 >> d2;

    /// Compare readen data with written data
    bool success = true;
    if (i1 != i2) {
        std::cerr << __func__ << " - error: ints don't match" << std::endl;
        success = false;
    }
    if (s1 != s2) {
        std::cerr << __func__ << " - error: strings don't match" << std::endl;
        success = false;
    }

    if (d1.size() != d2.size() || (std::memcmp(d1.data(), d2.data(), d1.size()) != 0)) {
        std::cerr << __func__ << " - error: RawDatas don't match" << std::endl;
        success = false;
    }

    return success;

}

}