#pragma once

#define __NoCopyConstruction(cls_name) cls_name(const cls_name &) = delete;
#define __NoMoveConstruction(cls_name) cls_name(cls_name &&) = delete;
#define __DefaultConstruction(cls_name) public: \
    cls_name() = default;
#define NoCopyMoveConstruction(cls_name) __NoCopyConstruction(cls_name) __NoMoveConstruction(cls_name)
#define DefaultNoCopyMoveConstruction(cls_name) __DefaultConstruction(cls_name) __NoCopyConstruction(cls_name) __NoMoveConstruction(cls_name)

#if defined (MATCHEDITOR_INNER_VISIBLE)
    #define MATCHEDITOR_VISIBLE public
    #define MATCHEDITOR_PROTECT public
#else
    #define MATCHEDITOR_VISIBLE private
    #define MATCHEDITOR_PROTECT protected
#endif