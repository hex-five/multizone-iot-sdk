/* Copyright(C) 2020 Hex Five Security, Inc. - All Rights Reserved */

#ifndef __ARCH_CC_H__
#define __ARCH_CC_H__


#include <stdint.h>

#undef  BYTE_ORDER
#define BYTE_ORDER  LITTLE_ENDIAN

typedef uint8_t     u8_t;
typedef int8_t      s8_t;
typedef uint16_t    u16_t;
typedef int16_t     s16_t;
typedef uint32_t    u32_t;
typedef int32_t     s32_t;

typedef uintptr_t   mem_ptr_t;

/* Define (sn)printf formatters for these lwIP types */
#define U16_F "hu"
#define S16_F "hd"
#define X16_F "hx"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"


#endif /* __ARCH_CC_H__ */
