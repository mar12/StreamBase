/**
 * Buffer for the storage of the readen data
 *
 * Stores data in the `std::vector` container and allows its deserialization
 *
 */
#ifndef UTILITY_INPUTBUFFER_H_
#define UTILITY_INPUTBUFFER_H_
#include <cstdint>
#include <vector>
#include <string>
#include "RawData.h"

namespace Utility {

class InputBuffer {
public:
    InputBuffer() : m_data() {}

    InputBuffer(std::size_t size, const char* data) :
        m_data(data, data + size), m_offset(0) {
    }

    const char* dataLeft() const {
        return &m_data.data()[m_offset];
    }

    std::size_t sizeLeft() const {
        return m_data.size() - m_offset;
    }

    void advance(std::size_t n) {
        m_offset += n;
    }

    void append(std::size_t size, const char* data) {
        m_data.insert(m_data.end(), data, data + size);
    }

private:
    std::vector<char> m_data;
    std::size_t m_offset = 0;

};

InputBuffer& operator>>(InputBuffer& stream, int& i);
InputBuffer& operator>>(InputBuffer& stream, std::string& s);
InputBuffer& operator>>(InputBuffer& stream, RawData& s);

}

#endif // UTILITY_INPUTBUFFER_H_