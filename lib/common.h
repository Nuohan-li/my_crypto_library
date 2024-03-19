#ifndef COMMON_H
#define COMMON_H

/*
    This file contains some common defines and enums that will be used by 
    all other files
*/

#define NUM_BITS_IN_BYTE 8

#define CRYPT_CHECK_OKAY(a) do {if (a != CRYPT_OKAY) return a; } while(0) 

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define LE16TOBE16(value) ((value >> 8) | (value << 8))
    #define LE32TOBE32(value) ( ((value >> 24) & 0x000000FF) | \
                                ((value >> 8) & 0x0000FF00) | \
                                ((value << 8) & 0x00FF0000) | \
                                ((value << 24) & 0xFF000000) )

    #define LE64TOBE64(value) ( ((value >> 56) & 0x00000000000000FF) | \
                                ((value >> 40) & 0x000000000000FF00) | \
                                ((value >> 24) & 0x0000000000FF0000) | \
                                ((value >> 8) & 0x00000000FF000000) | \
                                ((value << 8) & 0x000000FF00000000) | \
                                ((value << 24) & 0x0000FF0000000000) | \
                                ((value << 40) & 0x00FF000000000000) | \
                                ((value << 56) & 0xFF00000000000000) )
#else 
    #define LE16TOBE16(value) value
    #define LE32TOBE32(value) value
    #define LE64TOBE64(value) value
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define BE16TOLE16(value) ((value >> 8) | (value << 8))
    #define BE32TOLE32(value) ( ((value >> 24) & 0x000000FF) | \
                                ((value >> 8) & 0x0000FF00) | \
                                ((value << 8) & 0x00FF0000) | \
                                ((value << 24) & 0xFF000000) )

    #define BE64TOLE64(value) ( ((value >> 56) & 0x00000000000000FF) | \
                                ((value >> 40) & 0x000000000000FF00) | \
                                ((value >> 24) & 0x0000000000FF0000) | \
                                ((value >> 8) & 0x00000000FF000000) | \
                                ((value << 8) & 0x000000FF00000000) | \
                                ((value << 24) & 0x0000FF0000000000) | \
                                ((value << 40) & 0x00FF000000000000) | \
                                ((value << 56) & 0xFF00000000000000) )
#else 
    #define BE16TOLE16(value) value
    #define BE32TOLE32(value) value
    #define BE64TOLE64(value) value
#endif

typedef enum {
    CRYPT_OKAY = 0,
    CRYPT_FAILURE = 1, 
    CRYPT_NULL_PTR = 2,
    CRYPT_BAD_BUFFER_LEN = 3, 
} crypt_status;

#endif /* COMMON_H */