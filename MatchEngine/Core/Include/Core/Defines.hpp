#pragma once

#define DefineDefaultAssignmentOperator(className) \
    className &operator=(const className &other) = default; \
    className &operator=(className &&other) = default; \
