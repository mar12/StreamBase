#include "InputBuffer.h"

namespace Utility {

InputBuffer& operator>>(InputBuffer& stream, int& i) {
    std::int32_t val;
    val = *reinterpret_cast<const std::int32_t*>(stream.dataLeft());
    i = val;
    stream.advance(sizeof(val));
    return stream;
}

InputBuffer& operator>>(InputBuffer& stream, std::string& s) {
    int length;
    stream >> length;
    s = std::string(stream.dataLeft(), stream.dataLeft() + length);
    stream.advance(length);
    return stream;
}

InputBuffer& operator>>(InputBuffer& stream, RawData& s) {
    int length;
    stream >> length;
    s = RawData(length, stream.dataLeft());
    stream.advance(length);
    return stream;
}

}