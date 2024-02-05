#pragma once

// 空的宏
// 用于给解析器传递信息

#define EMPTY()

#define SERIALIZABLE_CLASS EMPTY()
#define SERIALIZABLE_CLASS_ONLY EMPTY()
#define SERIALIZE_PARENT_CLASS EMPTY()
#define SERIALIZABLE_MEMBER EMPTY()
#define SERIALIZABLE_MEMBER_ONLY EMPTY()

#define ALLOW_SERIALIZE_PRIVATE(class_name) friend struct ::MatchEngine::SerializeTrait<class_name>;
