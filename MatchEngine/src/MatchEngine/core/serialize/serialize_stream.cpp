#include <MatchEngine/core/serialize/serialize_stream.hpp>
#include <memory.h>

namespace MatchEngine {
    SerializeStream::SerializeStream() {
        data.reserve(32);
    }

    SerializeStream::~SerializeStream() {
        data.clear();
    }
    
    void *SerializeStream::getSaftPtr(size_t write_size) {
        while (data.size() + write_size > data.capacity()) {
            data.reserve(static_cast<size_t>(data.capacity() * 1.5));
        }
        data.resize(data.size() + write_size);
        return &data.data()[data.size() - write_size];
    }

    void SerializeStream::write(const void *buffer, size_t length) {
        memcpy(getSaftPtr(length), buffer, length);
    }
}
