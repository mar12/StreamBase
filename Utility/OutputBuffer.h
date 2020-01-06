/**
 * Buffer for the storage of the data to be written
 *
 * Allows its serialization and stores the data in the `std::vector` container
 */
#ifndef UTILITY_OUTPUTBUFFER_H_
#define UTILITY_OUTPUTBUFFER_H_
#include <cstdint>
#include <vector>
#include <string>
#include "RawData.h"

namespace Utility {

class OutputBuffer {
public:
    const char* data() const noexcept {
        return m_data.data();
    }

    std::size_t size() const noexcept {
        return m_data.size();
    }

    void append(std::size_t length, const char* data) {
        m_data.insert(m_data.end(), data, data + length);
    }

private:
    std::vector<char> m_data;

};

OutputBuffer& operator<<(OutputBuffer& stream, int i);
OutputBuffer& operator<<(OutputBuffer& stream, const std::string& s);
OutputBuffer& operator<<(OutputBuffer& stream, const RawData& s);

}

#endif // UTILITY_OUTPUTBUFFER_H_