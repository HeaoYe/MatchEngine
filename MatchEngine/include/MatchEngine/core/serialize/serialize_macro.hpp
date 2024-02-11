#pragma once

#include <MatchEngine/core/serialize/serialize_trait.hpp>

// 空的宏
// 用于给解析器传递信息

#define SERIALIZABLE_CLASS
#define SERIALIZABLE_CLASS_ONLY
#define SERIALIZE_PARENT_CLASS
#define SERIALIZABLE_MEMBER
#define SERIALIZABLE_MEMBER_ONLY

#define ALLOW_SERIALIZE_PRIVATE(class_name) friend struct ::MatchEngine::SerializeTrait<class_name>;
