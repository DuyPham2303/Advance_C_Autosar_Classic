#ifndef ECC_TOY_H
#define ECC_TOY_H
/*
 * ========================================================================
 *  ecc_toy.h — API tối giản cho Elliptic Curve (phiên bản minh hoạ “toy”)
 * ========================================================================
 *
 *  Mục đích:
 *    - Cung cấp các kiểu dữ liệu và hàm thao tác điểm trên đường cong elliptic
 *      dạng Weierstrass rút gọn:  y^2 = x^3 + a*x + b (mod p).
 *    - Cung cấp các hàm ECDSA (keygen/sign/verify) bản đơn giản để demo.
 *
 *  Lưu ý quan trọng:
 *    - Đây là mã minh hoạ (toy), dùng số nguyên 64-bit và một “đường cong nhỏ”.
 *      KHÔNG an toàn để dùng trong sản phẩm thật, không chống timing side-channel,
 *      không kiểm tra tất cả trường hợp biên, không sinh ngẫu nhiên an toàn, v.v.
 *    - Mọi phép tính đều thực hiện modulo p (số nguyên tố nhỏ). Với p lớn/ thực tế
 *      cần big-integer, kiểm tra lỗi đầy đủ và hằng số thời gian (constant-time).
 *
 *  Quy ước:
 *    - Tất cả toạ độ điểm (x, y) và hệ số (a, b, p) đều là uint64_t.
 *    - “∞” (điểm vô cực) được biểu diễn bằng cờ infinity = true, x/y bị bỏ qua.
 *    - G là điểm sinh (base point), n là bậc của G (số điểm trong <G>).
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------
 * Cấu trúc điểm trên đường cong: P = (x, y) hoặc điểm vô cực (infinity = true)
 *  - Khi infinity = true: x/y không có ý nghĩa và có thể bị bỏ qua.
 * ------------------------------------------------------------------------ */
typedef struct {
    uint64_t x, y;
    bool infinity;
} ECPoint;

/* ------------------------------------------------------------------------
 * Tham số đường cong:
 *    p : số nguyên tố mô-đun
 *    a,b : hệ số phương trình Weierstrass rút gọn
 *    G : điểm sinh (base point)
 *    n : bậc của G (nhỏ trong bản “toy”; thực tế rất lớn)
 *
 * Phương trình: y^2 = x^3 + a*x + b (mod p)
 * ------------------------------------------------------------------------ */
typedef struct {
    uint64_t p, a, b;  /* y^2 = x^3 + a*x + b (mod p) */
    ECPoint  G;        /* điểm sinh (base point) */
    uint64_t n;        /* bậc của G (order(G)) */
} ECC_Curve;

/* ------------------------------------------------------------------------
 * Đường cong toàn cục (bản “toy”)
 *  - Được khởi tạo trong ecc_init_curve().
 *  - Sau khi gọi ecc_init_curve(), các hàm khác sẽ dùng g_curve.
 * ------------------------------------------------------------------------ */
extern ECC_Curve g_curve;

/* ------------------------------------------------------------------------
 * ecc_init_curve
 *  - Khởi tạo g_curve với (p, a, b, G, n).
 *  - Trong bản minh hoạ: p, a, b, G được cố định; n có thể tính/ấn định sẵn.
 *  - PHẢI được gọi trước khi sử dụng các hàm ECC khác.
 * ------------------------------------------------------------------------ */
void ecc_init_curve(void);

/* =========================== Số học điểm ================================ */

/* ------------------------------------------------------------------------
 * ecc_add(P, Q)
 *  - Cộng hai điểm P + Q trên đường cong:
 *      + Xử lý đầy đủ các trường hợp đặc biệt:
 *          * P = ∞  => P + Q = Q
 *          * Q = ∞  => P + Q = P
 *          * Q = −P => P + Q = ∞
 *          * P = Q  => dùng công thức nhân đôi (doubling)
 *      + Tất cả phép toán modulo p (g_curve.p).
 *
 *  Trả về:
 *    - Điểm kết quả R = P + Q (có thể là ∞).
 * ------------------------------------------------------------------------ */
ECPoint ecc_add(ECPoint P, ECPoint Q);

/* ------------------------------------------------------------------------
 * ecc_scalar_mul(k, P)
 *  - Nhân vô hướng k*P bằng thuật toán double-and-add (minh hoạ).
 *  - Hỗ trợ k = 0 (trả ∞), P = ∞ (trả ∞).
 *
 *  Tham số:
 *    k : vô hướng (uint64_t), thường k mod n (n = order(G)).
 *    P : điểm trên đường cong.
 *
 *  Trả về:
 *    - Điểm Q = k*P (có thể là ∞).
 * ------------------------------------------------------------------------ */
ECPoint ecc_scalar_mul(uint64_t k, ECPoint P);

/* ============================= ECDSA (toy) ============================== */

/* ------------------------------------------------------------------------
 * ecdsa_keygen(priv, pub)
 *  - Tạo khoá công khai từ khoá bí mật:
 *      Q = d * G
 *
 *  Tham số:
 *    priv : d (1 <= d < n). Trong “toy” caller chịu trách nhiệm hợp lệ hoá d.
 *    pub  : [out] con trỏ ECPoint nhận Q.
 *
 *  Trả về:
 *    true  nếu thành công (pub nhận Q hợp lệ),
 *    false nếu đầu vào không hợp lệ (ví dụ d = 0 hoặc d >= n).
 * ------------------------------------------------------------------------ */
bool ecdsa_keygen(uint64_t priv, ECPoint* pub);

/* ------------------------------------------------------------------------
 * ecdsa_sign(hash32, priv, r, s)
 *  - Ký ECDSA trên thông điệp đã băm (SHA-256) => 32 byte.
 *  - Bản minh hoạ:
 *      + Sử dụng p/n nhỏ và “nonce k” theo cách đơn giản (không an toàn).
 *      + Thực tế phải dùng RFC 6979 (deterministic k) hoặc RNG mạnh và bảo mật.
 *
 *  Tham số:
 *    hash32 : mảng 32 byte là giá trị băm SHA-256 của thông điệp.
 *    priv   : d (1 <= d < n).
 *    r, s   : [out] hai thành phần chữ ký mod n.
 *
 *  Trả về:
 *    true  nếu tạo chữ ký thành công,
 *    false nếu lỗi (ví dụ r = 0 hoặc s = 0 sau tính toán, d không hợp lệ…).
 * ------------------------------------------------------------------------ */
bool ecdsa_sign(const uint8_t hash32[32], uint64_t priv, uint64_t* r, uint64_t* s);

/* ------------------------------------------------------------------------
 * ecdsa_verify(hash32, pub, r, s)
 *  - Xác minh chữ ký ECDSA cho thông điệp đã băm.
 *
 *  Tham số:
 *    hash32 : mảng 32 byte SHA-256(message).
 *    pub    : Q = d*G (điểm công khai), cần nằm trên đường cong và không ∞.
 *    r, s   : chữ ký đã cho (0 < r,s < n).
 *
 *  Trả về:
 *    true  nếu chữ ký hợp lệ cho hash32 và pub,
 *    false nếu không hợp lệ (tham số sai hoặc kiểm tra thất bại).
 * ------------------------------------------------------------------------ */
bool ecdsa_verify(const uint8_t hash32[32], ECPoint pub, uint64_t r, uint64_t s);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ECC_TOY_H */
