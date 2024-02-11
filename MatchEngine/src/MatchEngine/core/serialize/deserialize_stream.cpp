#include <MatchEngine/core/serialize/deserialize_stream.hpp>
#include <MatchEngine/core/serialize/serialize_stream.hpp>
#include <memory.h>

namespace MatchEngine {
    DeserializeStream::DeserializeStream(SerializeStream &&ss) {
        data = std::move(ss.data);
        ptr = data.size();
    }

    DeserializeStream::~DeserializeStream() {
        data.clear();
        ptr = 0;
    }

    void DeserializeStream::read(void *buffer, size_t length) {
        ptr -= length;
        memcpy(buffer, &data.data()[ptr], length);
    }
}
