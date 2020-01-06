/**
 * Helper functions for `std::string` and `std::wstring` manipulations
 *
 * Provided due to a lack of exact functionality in the standard C++ library.
 * ...hope they'll add it soon  :-)
 *
 * See the details for each function below.
 *
 */
#ifndef UTILITY_STRINGHELPERS_H_
#define UTILITY_STRINGHELPERS_H_
#include <string>
#include <vector>

namespace Utility {

/**
 * Separate one string into many at given separators
 */
std::vector<std::string> split(const std::string& s, char seperator);

/**
 * Convert an UTF-8 encoded `std::string` to UTF-16 encoded `std::wstring`
 * using the WinAPI `MultiByteToWideChar` function
 */
std::wstring convertUtf8StringToWString(const std::string& s);

}

#endif // UTILITY_STRINGHELPERS_H_