#pragma once

#include <MatchEngine/MatchEngine.hpp>

// 解析器只能解析头文件
REFLECT_CLASS( SERIALIZABLE_CLASS_ONLY )
struct Time {
    REFLECT_MEMBER( SERIALIZABLE_MEMBER )
    std::string year;
    REFLECT_MEMBER( SERIALIZABLE_MEMBER )
    std::string month;
    REFLECT_MEMBER( SERIALIZABLE_MEMBER )
    std::string day;
};

// 支持嵌套命名空间

namespace HJDGAUC::BBCUIAHCS::ACIUS {

// 声明一个类
// 告诉解析器这个 类 需要自动生成解析信息
// 通过 NAME 指定 名字
// 成员变量 和 成员函数 也可以指定名字
// 告诉解析器这个 类 需要自动生成 序列化和反序列化 信息
REFLECT_CLASS(NAME = "My RRR CCC" SERIALIZABLE_CLASS )
// 告诉解析器这个 类 只允许序列化和反序列化 (不允许反射)
// REFLECT_CLASS(NAME = "My RRR CCC" SERIALIZABLE_CLASS_ONLY )
class MyReflectClass {
    // 告诉编译器 允许反射私有成员变量
    ALLOW_REFLECT_PRIVATE()
    // 告诉编译器 允许序列化接口访问私有成员变量
    ALLOW_SERIALIZE_PRIVATE(MyReflectClass)

    // 允许解析嵌套类
    REFLECT_CLASS()
    class InnerClass {
        ALLOW_REFLECT_PRIVATE()
    private:
        REFLECT_MEMBER( NAME = "Int Member !" )
        int member;

        double dontReflectMember;
    public:
        REFLECT_FUNCTION()
        void func() {}

        void dontReflect();
    };
private:
    // 告诉解析器这个 成员变量 需要自动生成解析信息
    // 告诉解析器这个 成员变量 只允许序列化 (不允许反射)
    // REFLECT_MEMBER( SERIALIZABLE_MEMBER_ONLY )
    REFLECT_MEMBER( SERIALIZABLE_MEMBER )
    std::string name;

    // 告诉解析器这个 成员变量 需要自动生成解析信息
    // 告诉解析器这个 成员变量 可以序列化
    REFLECT_MEMBER( SERIALIZABLE_MEMBER )
    int age;

public:
    MyReflectClass() : name("Default"), age(999) {}
    MyReflectClass(const std::string &name, int age) : name(name), age(age) {}

    REFLECT_FUNCTION( NAME = "自我介绍" )
    // 告诉解析器这个 成员函数 需要自动生成解析信息
    void speak() {
        MCH_CLIENT_INFO("我叫{}, 我{}岁了.", name, age)
    }

    // 告诉解析器这个 成员函数 需要自动生成解析信息
    REFLECT_FUNCTION()
    Time askTime(float param) {
        MCH_CLIENT_INFO("现在是2024年2月5日.")
        MCH_CLIENT_INFO("传入的参数是: {}", param)
        if (param > 10) {
            return {
                .year = "年4202",
                .month = "月20",
                .day = "日50"
            }; 
        }
        return {
            .year = "2024年",
            .month = "2月",
            .day = "5日"
        };
    }
};

}
