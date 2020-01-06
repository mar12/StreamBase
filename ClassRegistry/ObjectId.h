/**
 * ID for objects created by the client
 *
 * Unlike C++ objects identifiers, user can use any particular string for ID,
 * e.g. starting with numbers, containing spaces or any characters.
 * However, some limitations are due to the client command-line interface
 */
#ifndef CLASSREGISTRY_OBJECTID_H_
#define CLASSREGISTRY_OBJECTID_H_
#include <string>

namespace ClassRegistry {

using ObjectId = std::string;

}

#endif //  CLASSREGISTRY_OBJECTID_H_