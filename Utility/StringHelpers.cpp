#include "StringHelpers.h"
#include <windows.h>

namespace Utility {

std::vector<std::string> split(const std::string& s, char seperator) {
    std::vector<std::string> result;
    std::size_t prev_pos = 0;
    std::size_t pos = 0;
    while ((pos = s.find(seperator, pos)) != std::string::npos) {
        std::string substring(s.substr(prev_pos, pos - prev_pos));
        result.push_back(substring);
        prev_pos = ++pos;
    }
    result.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
    return result;
}


std::wstring convertUtf8StringToWString(const std::string& s) {
    /// Get the output string length
    std::size_t dst_length = static_cast<std::size_t>(MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<DWORD>(s.length()), 0, 0));
    /// Allocate temporary buffer for the output string
    std::vector<wchar_t> buf(dst_length);
    /// Do the actual conversion
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), static_cast<DWORD>(s.length()), buf.data(), static_cast<DWORD>(dst_length));
    /// Return the `wstring`
    return std::wstring(buf.cbegin(), buf.cend());
}

}