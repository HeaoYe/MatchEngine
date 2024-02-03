#pragma once

#include <MatchEngine/core/serialize/serialize_trait.hpp>
#include <MatchEngine/core/base/macro.hpp>
#include <cstdint>
#include <vector>

namespace MatchEngine {
    // 序列化流
    class SerializeStream {
        NoCopyMoveConstruction(SerializeStream)
        friend class DeserializeStream;
    public:
        SerializeStream();
        ~SerializeStream();

        template <class T>
        std::enable_if_t<std::is_arithmetic_v<T>, void> write(T value) {
            *reinterpret_cast<T *>(getSaftPtr(sizeof(T))) = value;
        }

        void write(const void *buffer, size_t length);
    private:
        void *getSaftPtr(size_t write_size);
    private:
        std::vector<uint8_t> data;
    };

    template <class T>
    std::enable_if_t<std::is_arithmetic_v<T>, SerializeStream> &operator<<(SerializeStream &lhs, T rhs) {
        SerializeTrait<T>::serialize(lhs, rhs);
        return lhs;
    };

    template <class T>
    std::enable_if_t<(!std::is_arithmetic_v<T>) && std::is_constructible_v<SerializeTrait<T>>, SerializeStream> &operator<<(SerializeStream &lhs, const T &rhs) {
        SerializeTrait<T>::serialize(lhs, rhs);
        return lhs;
    };

    // 基础类型序列化
    template <class T>
    void SerializeTrait<T>::serialize(SerializeStream &ss, const T &rhs) {
        ss.write(rhs);
    }
}
