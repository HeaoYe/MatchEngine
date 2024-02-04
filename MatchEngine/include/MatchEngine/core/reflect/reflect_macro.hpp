#pragma once

// 空的宏
// 用于给解析器传递信息

#define REFLECT_CLASS(...)
#define REFLECT_MEMBER(...)
#define REFLECT_FUNCTION(...)

#define ALLOW_REFLECT_PRIVATE() friend void ::MatchEngineParser_RegisterReflectProperties();

extern void MatchEngineParser_RegisterReflectProperties();
