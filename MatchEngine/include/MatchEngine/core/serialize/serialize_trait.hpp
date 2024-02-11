#pragma once

namespace MatchEngine {
    // 序列化接口
    template <class T>
    struct SerializeTrait {
        SerializeTrait() = delete;

        static void serialize(class SerializeStream &ss, const T &rhs);
        static void deserialize(class DeserializeStream &ds, T &rhs);
    };
}
