#include <MatchEngine/MatchEngine.hpp>
#include <iostream>
// 引入自动生成头文件, 包含自动生成的序列化与反序列化函数
#include <MatchEngine_AutoGenerate.hpp>
#include "reflect_test.hpp"

int main() {
    // 使用 MatchEngine 提供的用户接口 命名空间
    using namespace MatchEngine::UserInterface;

    MatchEngine::MatchEngine engine;

    // 引擎初始化入口
    if (!engine.initialize()) {
        std::cout << "Failed initialize engine." << std::endl;
        return -1;
    }

    HJDGAUC::BBCUIAHCS::ACIUS::MyReflectClass person { "李华", 18 };

    // 使用反射

    // 获取反射类信息
    auto &class_MyRC = reflect->getClassByName("My RRR CCC");
    // 通过反射可以访问私有成员变量 或 成员函数

    // 遍历获取反射类
    for (auto &member_name : class_MyRC.getMemberNames()) {
        MCH_CLIENT_INFO("成员变量名: {}", member_name)
    }
    for (auto &function_name : class_MyRC.getFunctionNames()) {
        MCH_CLIENT_INFO("成员函数名: {}", function_name)
    }

    {
        // 获取反射类 成员变量信息
        auto &member_name = class_MyRC.getMember("name");
        // 通过成员变量信息 获取一个实例的成员变量的值
        // 延时获取类型
        auto person_name_wrapper = member_name.getValue(person);
        // 直接获取类型
        auto person_name = member_name.getValue<std::string>(person);
        MCH_CLIENT_INFO("{} {}", person_name_wrapper.cast<std::string>(), person_name);

        // 通过成员变量信息 设置一个实例的成员变量的值
        member_name.setValue(person, std::string("李军"));
        MCH_CLIENT_INFO(member_name.getValue<std::string>(person))

        // 通过成员变量信息 获取一个实例的成员变量的引用
        auto &member_age = class_MyRC.getMember("age");
        // 延时获取类型
        auto person_age_ref_wrapper = member_age.getValueReference(person);
        // 直接获取类型
        auto &person_age_ref = member_age.getValueReference<int>(person);

        MCH_CLIENT_INFO("person.age = {}", member_age.getValue<int>(person))
        person_age_ref_wrapper.cast<int &>() = 99;
        MCH_CLIENT_INFO("person.age = {}", member_age.getValue<int>(person))
        person_age_ref = 38;
        MCH_CLIENT_INFO("person.age = {}", member_age.getValue<int>(person))
    }

    {
        // 获取反射类 成员函数信息
        auto &function_speak = class_MyRC.getFunction("自我介绍");

        // 通过成员函数信息调用一个实例的成员函数
        function_speak.invoke(person);

        // 通过成员函数信息调用一个实例的成员函数 并传参 以及获取返回值
        auto &function_askTime = class_MyRC.getFunction("askTime");
        // 延时获取类型
        auto result_wrapper = function_askTime.invoke(person, 3.1415926f);
        // 直接获取类型
        auto result = function_askTime.invokeEx<Time>(person, 82387432.0f);
        MCH_CLIENT_INFO("{} {} {}", result_wrapper.cast<Time>().year, result_wrapper.cast<Time>().month, result_wrapper.cast<Time>().day)
        MCH_CLIENT_INFO("{} {} {}", result.year, result.month, result.day)
    }

    {
        // 通过 反射类信息 直接创建实例
        auto *object = class_MyRC.createObject();

        MCH_CLIENT_INFO("{} {}", object->getValue<std::string>("name"), object->getValue<int>("age"))

        // 设置和获取类型时, 以及函数传参时, 参数类型不匹配会报错!!!
        object->setValue("name", std::string("小李"));
        object->getValueReference<int>("age") = 18;

        // 调用成员函数
        object->invoke("自我介绍");
        // 参数类型不匹配会报错!!!
        MCH_CLIENT_INFO(object->invoke("askTime", 99.0f).cast<Time>().month)

        // 释放object
        auto *person_ptr = object->release<HJDGAUC::BBCUIAHCS::ACIUS::MyReflectClass>();
        delete object;
        object = nullptr;

        person_ptr->speak();
        person_ptr->askTime(10203040);

        // 从ptr创建object
        object = class_MyRC.createObject(person_ptr);
        person_ptr = nullptr;

        object->invoke("自我介绍");
    }

    MCH_CLIENT_INFO("...........................")

    {
        // 序列化
        MatchEngine::SerializeStream ss;
        person.speak();
        ss << person;
        auto time = person.askTime(9);
        ss << time;

        HJDGAUC::BBCUIAHCS::ACIUS::MyReflectClass person2;
        Time time_output;
        // 反序列化
        MatchEngine::DeserializeStream ds(std::move(ss));
        ds >> time_output;
        MCH_CLIENT_INFO("{} {} {}", time.year, time.month, time.day)
        MCH_CLIENT_INFO("{} {} {}", time_output.year, time_output.month, time_output.day)
        person2.speak();
        ds >> person2;
        person2.speak();
    }

    MCH_CLIENT_INFO("...........................")

    {
        auto &class_master = reflect->getClassByName("Master");
        auto &class_child = reflect->getClassByName("Child");

        auto *child_object = class_child.createObject();

        MCH_CLIENT_INFO(child_object->getValue<std::string>("name"));

        // 使用反射获取父类的成员变量
        auto &member_max = class_master.getMember("max");
        Child *ptr = child_object->release<Child>();
        auto max = member_max.getValueByPtr<uint32_t>(ptr);

        MCH_CLIENT_INFO(max)


        member_max.setValueByPtr<uint32_t>(ptr, 0);
        MCH_CLIENT_INFO(member_max.getValueByPtr<uint32_t>(ptr))

        // 因为序列化不会存储父类信息
        MatchEngine::SerializeStream ss;
        ss << *ptr;

        Child child2;
        MatchEngine::DeserializeStream ds(std::move(ss));
        ds >> child2;

        // 所以child2.max = 1234567890
        // 启用父类序列化后
        // child2.max 变为 0
        MCH_CLIENT_INFO(member_max.getValueByPtr<uint32_t>(&child2))

        auto &master_func = class_master.getFunction("func");
        auto &func = class_child.getFunction("func");
        func.invokeByPtr(&child2);
        master_func.invokeByPtr(&child2);
        child2.num = 99.2;
        func.invokeByPtr(&child2);
    }

    // 引擎运行入口
    // engine.run();

    // 引擎销毁入口
    engine.destroy();
    return 0;
}
