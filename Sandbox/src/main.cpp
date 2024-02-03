#include <MatchEngine/MatchEngine.hpp>
#include <iostream>

// 自定义数据类型
struct MyData {
    std::string name;
    int age;
};

// 为自定义数据实现序列化接口
template <>
struct MatchEngine::SerializeTrait<MyData> {
    static void serialize(SerializeStream &ss, const MyData &data) {
        ss << data.name << data.age;
    }

    static void deserialize(DeserializeStream &ds, MyData &data) {
        ds >> data.age >> data.name;
    }
};

int main() {
    MatchEngine::MatchEngine engine;

    // 引擎初始化入口
    if (!engine.initialize()) {
        std::cout << "Failed initialize engine." << std::endl;
        return -1;
    }

    // 底层都是 std::vector<uint8_t> data;

    // 未来会加入存储到文件的功能
    // 序列化流
    MatchEngine::SerializeStream ss;
    int a = 1;
    float b = 1.4f;
    MyData data;
    data.name = "QWERTYUIOPPASDFGHJKLZXCVBNMQWERTYUIOPPASDFGHJKLZXCVBNMQWERTYUIOPPASDFGHJKLZXCVBNM";
    data.age = 99999999;

    // 复合数据的序列化与反序列化
    std::vector<std::vector<std::map<int, MyData>>> test_data, test_data_output;
    for (int i = 0; i < 3; i ++) {
        test_data.emplace_back();
        for (int j = 0; j < 2; j ++) {
            test_data[i].emplace_back();
            for (int k = 0; k < 3; k ++) {
                test_data[i][j].insert({ k * k, MyData { .name = std::to_string(i) + std::to_string(j) + std::to_string(k), .age = i * j * k } });
            }
        }
    }
    for (int i = 0; i < test_data.size(); i ++) {
        for (int j = 0; j < test_data[i].size(); j ++) {
            for (auto &[key, value] : test_data[i][j]) {
                MCH_CLIENT_INFO("原始数据: i{} j{} k{}: name {} age {}", i, j, key, value.name, value.age)
            }
        }
    }
    /*
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i0 j0 k0: name 000 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i0 j0 k1: name 001 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i0 j0 k4: name 002 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i0 j1 k0: name 010 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i0 j1 k1: name 011 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i0 j1 k4: name 012 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i1 j0 k0: name 100 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i1 j0 k1: name 101 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i1 j0 k4: name 102 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i1 j1 k0: name 110 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i1 j1 k1: name 111 age 1
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i1 j1 k4: name 112 age 2
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i2 j0 k0: name 200 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i2 j0 k1: name 201 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i2 j0 k4: name 202 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i2 j1 k0: name 210 age 0
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i2 j1 k1: name 211 age 2
    [2024-02-04 02:22:33.138] [MatchEngine Client] [info] 原始数据: i2 j1 k4: name 212 age 4
*/
/*
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i0 j0 k0: name 000 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i0 j0 k1: name 001 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i0 j0 k4: name 002 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i0 j1 k0: name 010 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i0 j1 k1: name 011 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i0 j1 k4: name 012 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i1 j0 k0: name 100 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i1 j0 k1: name 101 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i1 j0 k4: name 102 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i1 j1 k0: name 110 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i1 j1 k1: name 111 age 1
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i1 j1 k4: name 112 age 2
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i2 j0 k0: name 200 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i2 j0 k1: name 201 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i2 j0 k4: name 202 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i2 j1 k0: name 210 age 0
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i2 j1 k1: name 211 age 2
    [2024-02-04 02:23:32.624] [MatchEngine Client] [info] 反序列化数据: i2 j1 k4: name 212 age 4
*/


    ss << a << b << data;
    ss << test_data;
    MCH_CLIENT_INFO("序列化: a: {}, b: {}", a, b)


    // 未来会加入从文件加载的功能
    // 反序列化流
    MatchEngine::DeserializeStream ds(std::move(ss));
    int a_output;
    float b_output;
    MyData data_output;

    ds >> test_data_output;

    for (int i = 0; i < test_data_output.size(); i ++) {
        for (int j = 0; j < test_data_output[i].size(); j ++) {
            for (auto &[key, value] : test_data_output[i][j]) {
                MCH_CLIENT_INFO("反序列化数据: i{} j{} k{}: name {} age {}", i, j, key, value.name, value.age)
            }
        }
    }

    ds >> data_output >> b_output >> a_output;
    MCH_CLIENT_INFO("反序列化: a_output = {}, b_output = {}", a_output, b_output)
    MCH_CLIENT_INFO("反序列化: data_output: {}, {}", data_output.name, data_output.age)

    // 引擎运行入口
    // engine.run();

    // 引擎销毁入口
    engine.destroy();
    return 0;
}
