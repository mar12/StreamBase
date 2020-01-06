/**
 * Representation of the data contained in the response
 *
 * Reponse uses the following format:
 *
 * +-----------+------+
 * | Result ID | Data |
 * +-----------+------+
 *
 * Depending on the action requested, Data field can be not used in the particular response.
 * The exact binary representations of the fields are governed by the type of the field,
 * as well as by the type serialization operator implementation for `Utility::InputBuffer`
 * and `Utility::OutputBuffer` classes.
 *
 * The length of the response must not exceed the `maxLength()`
 *
 */
#ifndef PROTOCOL_RESPONSE_H_
#define PROTOCOL_RESPONSE_H_
#include "ResultId.h"
#include "Utility/RawData.h"
#include "Utility/InputBuffer.h"
#include "Utility/OutputBuffer.h"

namespace Protocol {

class Response {
public:
    Response() {}

    Response(ResultId result, const Utility::RawData& data = Utility::RawData()) : m_result(result), m_data(data) {}

    ResultId resultId() const { return m_result; }

    std::string errorMessage() const;

    const Utility::RawData& data() const { return m_data; }

    static constexpr std::size_t maxLength() { return 4096; }

private:
    ResultId m_result;
    Utility::RawData m_data;

    friend Utility::OutputBuffer& operator<<(Utility::OutputBuffer& stream, const Response& response);
    friend Utility::InputBuffer& operator>>(Utility::InputBuffer& stream, Response& response);
};

}

#endif // PROTOCOL_RESPONSE_H_