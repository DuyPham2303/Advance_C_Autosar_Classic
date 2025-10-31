#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>
#include <stdint.h>

/* SHA-256 API (tự viết) */
void sha256(const uint8_t* data, size_t len, uint8_t out[32]);

#endif /* CRYPTO_H */
