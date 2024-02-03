#pragma once

#define __NoCopyConstruction(cls_name) cls_name(const cls_name &) = delete;
#define __NoMoveConstruction(cls_name) cls_name(cls_name &&) = delete;
#define __DefaultConstruction(cls_name) public: \
    cls_name() = default;
#define NoCopyMoveConstruction(cls_name) __NoCopyConstruction(cls_name) __NoMoveConstruction(cls_name)
#define DefaultNoCopyMoveConstruction(cls_name) __DefaultConstruction(cls_name) __NoCopyConstruction(cls_name) __NoMoveConstruction(cls_name)
