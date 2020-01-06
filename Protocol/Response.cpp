#include "Response.h"

namespace Protocol {

std::string Response::errorMessage() const {
    switch (resultId()) {
    case ResultId::InvalidClass:  return "Invalid class";
    case ResultId::InvalidObject: return "Invalid object";
    case ResultId::InvalidMember: return "Invalid method or attribute";
    case ResultId::InvalidData:   return "Invalid value";
    default: return "Unknown error";
    }
}

Utility::OutputBuffer& operator<<(Utility::OutputBuffer& stream, const Response& response) {
    return stream << static_cast<int>(response.m_result) << response.m_data;
}

Utility::InputBuffer& operator>>(Utility::InputBuffer& stream, Response& response) {
    int result;
    stream >> result >> response.m_data;
    response.m_result = static_cast<ResultId>(result);
    return stream;
}

}