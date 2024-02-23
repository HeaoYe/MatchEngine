#pragma once

#define DefineDefaultAssignmentOperator(className) \
    className &operator=(const className &other) = default; \
    className &operator=(className &&other) = default; \

#define DefineDefaultConstexprAssignmentOperator(className) \
    constexpr className &operator=(const className &other) = default; \
    constexpr className &operator=(className &&other) = default; \
