#include "crypto_api.h"

#include "md5.h"
#include "sha1.h"
#include "sha256.h"

#include <stdio.h>

/////////////////////////////////// API for hashing algorithms ///////////////////////////////////

crypt_status crypt_md5(const unsigned char *input,
                 unsigned int input_len, unsigned char *output,
                 unsigned int output_len
)
{
    if(!input || !output) return CRYPT_NULL_PTR;

    if((output_len) < MD5_DIGESTS_LEN_BYTES){
        return CRYPT_BAD_BUFFER_LEN;
    }

    md5_ctx ctx = {0};

    CRYPT_CHECK_OKAY(md5_init(&ctx));
    CRYPT_CHECK_OKAY(md5_update(&ctx, input, input_len));
    CRYPT_CHECK_OKAY(md5_finish(&ctx, output));
    return CRYPT_OKAY;
}

crypt_status crypt_sha1(const unsigned char *input,
                 unsigned int input_len, unsigned char *output,
                 unsigned int output_len
)
{
    if(!input || !output) return CRYPT_NULL_PTR;

    if(output_len < SHA1_DIGEST_LEN_BYTES){
        return CRYPT_BAD_BUFFER_LEN;
    }    

    sha1_ctx ctx = {0};

    CRYPT_CHECK_OKAY(sha1_init(&ctx));
    CRYPT_CHECK_OKAY(sha1_update(&ctx, input, input_len));
    CRYPT_CHECK_OKAY(sha1_finish(&ctx, output, output_len));

    return CRYPT_OKAY;
}

crypt_status crypt_sha256(const unsigned char *input,
                 unsigned int input_len, unsigned char *output,
                 unsigned int output_len, SHA2 sha
){
    if(!input || !output) return CRYPT_NULL_PTR;

    switch (sha){
        case SHA224:
            if(output_len < SHA224_DIGEST_LEN_BYTES){
                return CRYPT_BAD_BUFFER_LEN;
            } 
            sha224_256_ctx ctx_224 = {0};
            CRYPT_CHECK_OKAY(sha224_256_init(&ctx_224, SHA224));
            CRYPT_CHECK_OKAY(sha224_256_update(&ctx_224, input, input_len));
            CRYPT_CHECK_OKAY(sha224_256_finish(&ctx_224, output, output_len, SHA224));
            break;
        case SHA256:
            if(output_len < SHA256_DIGEST_LEN_BYTES){
                return CRYPT_BAD_BUFFER_LEN;
            } 
            sha224_256_ctx ctx_256 = {0};
            CRYPT_CHECK_OKAY(sha224_256_init(&ctx_256, SHA256));
            CRYPT_CHECK_OKAY(sha224_256_update(&ctx_256, input, input_len));
            CRYPT_CHECK_OKAY(sha224_256_finish(&ctx_256, output, output_len, SHA256));
            break;
        default:
            return CRYPT_UNSUPPORTED_ALGO;
    }

    return CRYPT_OKAY;

}

/////////////////////////////////// API for encryption algorithms ///////////////////////////////////
