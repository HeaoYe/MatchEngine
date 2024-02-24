#pragma once

#define DefineDefaultAssignmentOperator(ClassName) \
    ClassName &operator=(const ClassName &other) = default; \
    ClassName &operator=(ClassName &&other) = default; \

#define DefineDefaultConstexprAssignmentOperator(ClassName) \
    constexpr ClassName &operator=(const ClassName &other) = default; \
    constexpr ClassName &operator=(ClassName &&other) = default; \
