#include "LZWCodec.h"

namespace LZW {
    std::ostream &operator<<(std::ostream &stream, const std::vector<unsigned char> &vect) {
        stream.write(reinterpret_cast<const char *>(vect.begin().base()), vect.size() * sizeof(unsigned char));
        return stream;
    }
}