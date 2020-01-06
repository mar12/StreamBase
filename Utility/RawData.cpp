#include "RawData.h"
#include <string>
#include <stdexcept>

namespace Utility {


template<> RawData RawData::fromValue<int>(const int& value) {
    return fromValue(std::to_string(value));
}

template<> bool RawData::toValue<int>(int& value) const {
    try {
        std::string s;
        if (!toValue<std::string>(s)) {
            return false;
        }
        value = std::stoi(s);
    }
    catch (const std::invalid_argument&) {
        return false;
    }
    catch (const std::out_of_range&) {
        return false;
    }
    return true;
}


template<> RawData RawData::fromValue<std::string>(const std::string& value) {
    RawData result;
    result.m_data.reserve(value.size());
    result.m_data.assign(value.cbegin(), value.cend());
    return result;
}

template<> bool RawData::toValue<std::string>(std::string& value) const {
    value.clear();
    value.reserve(m_data.size());
    value.assign(m_data.cbegin(), m_data.cend());
    return true;
}

};