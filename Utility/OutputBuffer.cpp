#include "OutputBuffer.h"

namespace Utility {

OutputBuffer& operator<<(OutputBuffer& stream, int i) {
    std::int32_t val = static_cast<std::int32_t>(i);
    stream.append(sizeof(val), reinterpret_cast<char*>(&val));
    return stream;
}

OutputBuffer& operator<<(OutputBuffer& stream, const std::string& s) {
    stream << static_cast<int>(s.size());
    stream.append(s.size(), s.data());
    return stream;
}

OutputBuffer& operator<<(OutputBuffer& stream, const RawData& s) {
    stream << static_cast<int>(s.size());
    stream.append(s.size(), s.data());
    return stream;
}

}