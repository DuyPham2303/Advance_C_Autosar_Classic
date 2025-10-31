#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "crypto.h"
#include "ecc_toy.h"

/*
 * =====================================================================
 *  main.c — Demo “tối giản” ký & xác minh ECDSA trên đường cong đồ chơi
 * =====================================================================
 *
 *  Mục tiêu:
 *   - Băm một thông điệp bằng SHA-256 (triển khai tự viết trong crypto.c).
 *   - Tạo khoá công khai từ khoá bí mật với ECC toy (ecc_toy.c).
 *   - Ký ECDSA trên giá trị băm và xác minh chữ ký.
 *
 *  LƯU Ý:
 *   - Đây là minh hoạ học thuật, sử dụng đường cong “đồ chơi” số rất nhỏ,
 *     KHÔNG AN TOÀN để dùng trong sản phẩm thực tế.
 *   - Không có biện pháp chống rò rỉ kênh kề, không bảo vệ bộ nhớ, v.v.
 */

/* In mảng byte ở dạng hex liền nhau (không có khoảng trắng) */
static void print_hex(const uint8_t* b, size_t n) {
    for (size_t i = 0; i < n; i++)
        printf("%02x", b[i]);
}

int main(void) {
    /* 1) Khởi tạo đường cong (tính order(G) nếu n==0) */
    ecc_init_curve();

    /* In thông số đường cong đang dùng (đồ chơi) */
    printf("Curve p=%llu, a=%llu, b=%llu\n",
           (unsigned long long)g_curve.p,
           (unsigned long long)g_curve.a,
           (unsigned long long)g_curve.b);
    printf("G=(%llu,%llu), order n=%llu\n",
           (unsigned long long)g_curve.G.x,
           (unsigned long long)g_curve.G.y,
           (unsigned long long)g_curve.n);

    /* 2) Chuẩn bị thông điệp và băm SHA-256 */
    const char* msg = "Hello, ECDSA (toy)!";
    uint8_t h[32];
    sha256((const uint8_t*)msg, strlen(msg), h);
    printf("SHA-256(msg) = ");
    print_hex(h, 32);
    printf("\n");

    /* 3) Khoá bí mật (ví dụ minh hoạ). Thực tế phải ngẫu nhiên, đủ lớn. */
    uint64_t priv = 1234;

    /* 4) Tạo khoá công khai Q = d*G */
    ECPoint pub;
    if (!ecdsa_keygen(priv, &pub)) {
        printf("Bad private key\n");
        return 1;
    }
    printf("Public Q = d*G = (%llu,%llu)\n",
           (unsigned long long)pub.x,
           (unsigned long long)pub.y);

    /* 5) Ký ECDSA trên giá trị băm h → (r,s) */
    uint64_t r, s;
    if (!ecdsa_sign(h, priv, &r, &s)) {
        printf("Sign failed\n");
        return 1;
    }
    printf("Signature (r,s) = (%llu, %llu)\n",
           (unsigned long long)r,
           (unsigned long long)s);

    /* 6) Xác minh chữ ký với đúng thông điệp */
    int ok = ecdsa_verify(h, pub, r, s);
    printf("Verify(correct msg): %s\n", ok ? "OK" : "FAIL");

    /* 7) Thử xác minh lại với thông điệp sai → KỲ VỌNG FAIL */
    const char* bad = "tamper";
    uint8_t h2[32];
    sha256((const uint8_t*)bad, strlen(bad), h2);
    ok = ecdsa_verify(h2, pub, r, s);
    printf("Verify(wrong msg): %s\n", ok ? "OK" : "FAIL (expected)");

    return 0;
}
