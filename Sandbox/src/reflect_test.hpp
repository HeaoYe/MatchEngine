#pragma once

#include <MatchEngine/MatchEngine.hpp>

// 支持反射结构体
// 支持虚函数
REFLECT_CLASS( SERIALIZABLE_CLASS )
struct Master {
    REFLECT_MEMBER()
    double num { -999.77777 };

    REFLECT_MEMBER( SERIALIZABLE_MEMBER )
    uint32_t max { 1234567890 };

    REFLECT_FUNCTION()
    virtual void func() {
        MCH_CLIENT_INFO("Master Function num = {}", num);
    }
};

// 用SERIALIZE_PARENT_CLASS启用父类的序列化
REFLECT_CLASS( SERIALIZABLE_CLASS SERIALIZE_PARENT_CLASS )
class Child : Master {
public:
    REFLECT_MEMBER( SERIALIZABLE_MEMBER )
    std::string name = "默认Name";

    REFLECT_MEMBER( SERIALIZABLE_MEMBER )
    float num = 1.6666f;

    REFLECT_FUNCTION()
    void func() override {
        MCH_CLIENT_INFO("Child Function num = {}", num);
    }
};
