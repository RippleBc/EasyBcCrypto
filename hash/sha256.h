#ifndef _SHA256_H_
#define _SHA256_H_

#include <stddef.h>
#include <stdint.h>

#define SHA256_BYTES 32

typedef struct {
	uint8_t  buf[64];
	uint32_t hash[8];
	uint32_t bits[2];
	uint32_t len;
} sha256_context;

extern void sha256_init(sha256_context *ctx);
extern void sha256_hash(sha256_context *ctx, const void *data, size_t len);
extern void sha256_done(sha256_context *ctx, uint8_t *hash);

extern void sha256(const void *data, size_t len, uint8_t *hash);

#endif
