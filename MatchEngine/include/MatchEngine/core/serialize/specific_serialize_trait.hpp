#pragma once

#include <MatchEngine/core/serialize/serialize_stream.hpp>
#include <MatchEngine/core/serialize/deserialize_stream.hpp>
#include <string>
#include <vector>
#include <map>

namespace MatchEngine {
    // STL序列化与反序列化

    template <>
    struct SerializeTrait<std::string> {
        static void serialize(SerializeStream &ss, const std::string &str) {
            ss.write(str.data(), str.length());
            ss << str.length();
        }

        static void deserialize(DeserializeStream &ds, std::string &str) {
            std::string::size_type length;
            ds >> length;
            str.resize(length);
            ds.read(str.data(), length);
        }
    };

    template <class T>
    struct SerializeTrait<std::vector<T>> {
        static void serialize(SerializeStream &ss, const std::vector<T> &vector) {
            auto capacity = vector.capacity();
            auto size = vector.size();
            for (size_t i = 0; i < size; i ++) {
                ss << vector[size - i - 1];
            }
            ss << capacity << size;
        }

        static void deserialize(DeserializeStream &ds, std::vector<T> &vector) {
            size_t capacity, size;
            ds >> size >> capacity;
            vector.reserve(capacity);
            vector.resize(size);
            for (size_t i = 0; i < size; i ++) {
                ds >> vector[i];
            }
        }
    };

    template <class K, class V>
    struct SerializeTrait<std::pair<K, V>> {
        static void serialize(SerializeStream &ss, const std::pair<K, V> &pair) {
            ss << pair.first << pair.second;
        }

        static void deserialize(DeserializeStream &ds, std::pair<K, V> &pair) {
            ds >> pair.second >> pair.first;
        }
    };

    template <class K, class V>
    struct SerializeTrait<std::map<K, V>> {
        static void serialize(SerializeStream &ss, const std::map<K, V> &map) {
            auto size = map.size();
            for (auto &pair : map) {
                ss << pair;
            }
            ss << size;
        }

        static void deserialize(DeserializeStream &ds, std::map<K, V> &map) {
            size_t size;
            ds >> size;
            std::pair<K, V> pair;
            for (size_t i = 0; i < size; i ++) {
                ds >> pair;
                map.insert(std::move(pair));
            }
        }
    };
}
