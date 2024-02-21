#pragma once

#include <MatchEngine/core/serialize/serialize_trait.hpp>
#include <MatchEngine/core/base/macro.hpp>
#include <cstdint>
#include <vector>

namespace MatchEngine {
    // 反序列化流
    class DeserializeStream {
        NoCopyMoveConstruction(DeserializeStream)
    public:
        DeserializeStream(SerializeStream &&);
        ~DeserializeStream();

        template <class T>
        std::enable_if_t<std::is_arithmetic_v<T>, T> read() {
            ptr -= sizeof(T);
            return *reinterpret_cast<T *>(&data[ptr]);
        }

        void read(void *buffer, size_t length);
    private:
        size_t ptr;
        std::vector<uint8_t> data;
    };

    template <class T>
    std::enable_if_t<std::is_constructible_v<T>, DeserializeStream> &operator>>(DeserializeStream &lhs, T &rhs) {
        SerializeTrait<T>::deserialize(lhs, rhs);
        return lhs;
    };

    template <class T>
    std::enable_if_t<std::is_constructible_v<T>, DeserializeStream> &operator>>(DeserializeStream &lhs, T *rhs) {
        SerializeTrait<T>::deserialize(lhs, *rhs);
        return lhs;
    };

    // 基础类型反序列化
    template <class T>
    void SerializeTrait<T>::deserialize(DeserializeStream &ds, T &rhs) {
        rhs = ds.read<T>();
    }
}
