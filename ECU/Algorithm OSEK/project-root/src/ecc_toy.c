#include "ecc_toy.h"
#include <stdio.h>

/*
 * ===========================================================================
 *  ecc_toy.c — Cài đặt “toy” ECC + ECDSA để minh hoạ nguyên lý
 * ===========================================================================
 *
 *  CẢNH BÁO:
 *    - Mã này CHỈ phục vụ học tập / demo. KHÔNG an toàn để dùng trong sản phẩm:
 *        + Sử dụng số 64-bit và mô-đun p rất nhỏ (~1e4), không có big integer.
 *        + Không chống tấn công kênh kề (timing/power), không random an toàn.
 *        + Nonce k trong ECDSA ở đây chọn rất đơn giản (deterministic toy),
 *          KHÔNG được dùng ngoài thực tế (phải dùng RNG mạnh hoặc RFC6979).
 *    - Mục tiêu: thấy được công thức cộng điểm, nhân vô hướng, và ECDSA (r,s).
 */

/* -------------------------------------------------------------------------
 * 1) Tham số đường cong “toy”
 *    Phương trình: y^2 = x^3 + a*x + b (mod p)
 *    G: điểm sinh (base point)
 *    n: bậc của G (order). Nếu n=0 khi khởi tạo, ta sẽ tính ở ecc_init_curve().
 * ------------------------------------------------------------------------- */
ECC_Curve g_curve = {
    .p = 9739,
    .a = 497,
    .b = 1768,
    .G = { .x = 1804, .y = 5368, .infinity=false },
    .n = 0
};

/* =========================== TIỆN ÍCH SỐ HỌC MOD =========================
 *  Các phép toán cộng/trừ/nhân modulo p. Vì p nhỏ, phép nhân dùng % trực tiếp.
 * ======================================================================== */

/* (a + b) mod p — đã tối giản hoá nhánh để tránh tràn nhỏ */
static inline uint64_t addp(uint64_t a, uint64_t b) {
    uint64_t r = a + b;
    if (r >= g_curve.p) r -= g_curve.p;
    return r;
}

/* (a - b) mod p (trả kết quả trong [0, p-1]) */
static inline uint64_t subp(uint64_t a, uint64_t b) {
    return (a >= b) ? (a - b) : (a + g_curve.p - b);
}

/* (a * b) mod p — p nhỏ ~1e4 nên dùng phép nhân thường rồi lấy % */
static inline uint64_t mulp(uint64_t a, uint64_t b) {
    return (a * b) % g_curve.p;
}

/* UCLN (Greatest Common Divisor) — dùng trong kiểm tra gcd(k, n) = 1 */
static uint64_t ugcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* Nghịch đảo modulo (a^{-1} mod m) — Thuật toán Euclid mở rộng
 *  - Trả 0 nếu a không khả nghịch mod m (tức gcd(a, m) != 1)
 */
static uint64_t invmod(uint64_t a, uint64_t m) {
    int64_t t = 0, newt = 1;                   /* hệ số cho Bezout */
    int64_t r = (int64_t)m, newr = (int64_t)(a % m); /* phần dư tiến trình */
    while (newr != 0) {
        int64_t q = r / newr;                  /* thương nguyên */
        int64_t tmp = t - q*newt; t = newt; newt = tmp;
        tmp = r - q*newr; r = newr; newr = tmp;
    }
    if (r != 1) return 0;                      /* không có nghịch đảo */
    if (t < 0) t += (int64_t)m;                /* chuẩn hoá về [0, m-1] */
    return (uint64_t)t;
}

/* Kiểm tra 1 điểm có nằm trên đường cong hay không:
 *  y^2 ?= x^3 + a*x + b (mod p)
 */
static bool is_on_curve(ECPoint P) {
    if (P.infinity) return true; /* ∞ coi là hợp lệ trong nhóm */
    uint64_t left  = mulp(P.y, P.y);
    uint64_t right = addp(mulp(mulp(P.x,P.x), P.x), addp(mulp(g_curve.a, P.x), g_curve.b));
    return left == right;
}

/* =========================== PHÉP CỘNG ĐIỂM =============================
 *  R = P + Q
 *  Trường hợp đặc biệt:
 *   - P = ∞ => R = Q
 *   - Q = ∞ => R = P
 *   - Q = -P (x2 = x1, y2 = -y1) => R = ∞
 *  Công thức:
 *   - Nếu P != Q:
 *       λ = (y2 - y1) * (x2 - x1)^{-1} mod p
 *       x3 = λ^2 - x1 - x2
 *       y3 = λ*(x1 - x3) - y1
 *   - Nếu P = Q (doubling):
 *       λ = (3*x1^2 + a) * (2*y1)^{-1} mod p
 *       x3, y3 tương tự
 * ====================================================================== */
ECPoint ecc_add(ECPoint P, ECPoint Q) {
    /* ∞ + Q = Q ; P + ∞ = P */
    if (P.infinity) return Q;
    if (Q.infinity) return P;

    /* Cùng hoành độ */
    if (P.x == Q.x) {
        /* y1 + y2 ≡ 0 (mod p) => Q = -P => P+Q = ∞ */
        if ((P.y + Q.y) % g_curve.p == 0) return (ECPoint){0,0,true};

        /* Trường hợp P = Q (doubling) vì x1 = x2 và y1 != -y2 */
        uint64_t num = addp(mulp(3, mulp(P.x, P.x)), g_curve.a); /* 3x^2 + a */
        uint64_t den = invmod((2*P.y) % g_curve.p, g_curve.p);   /* (2y)^{-1} */
        uint64_t lam = mulp(num, den);                           /* λ */

        uint64_t xr = subp(subp(mulp(lam, lam), P.x), Q.x);      /* x3 = λ^2 - x1 - x2 */
        uint64_t yr = subp(mulp(lam, subp(P.x, xr)), P.y);       /* y3 = λ*(x1 - x3) - y1 */
        return (ECPoint){xr, yr, false};
    } else {
        /* P != Q: công thức cộng hai điểm khác nhau */
        uint64_t num = subp(Q.y, P.y);                           /* y2 - y1 */
        uint64_t den = invmod(subp(Q.x, P.x), g_curve.p);        /* (x2 - x1)^{-1} */
        uint64_t lam = mulp(num, den);                           /* λ */

        uint64_t xr = subp(subp(mulp(lam, lam), P.x), Q.x);      /* x3 = λ^2 - x1 - x2 */
        uint64_t yr = subp(mulp(lam, subp(P.x, xr)), P.y);       /* y3 = λ*(x1 - x3) - y1 */
        return (ECPoint){xr, yr, false};
    }
}

/* ========================== NHÂN VÔ HƯỚNG k*P ===========================
 *  Thuật toán double-and-add “naive”:
 *    - Duyệt bit của k: nếu bit=1 thì R += Q; mỗi bước nhân đôi Q (Q = 2Q).
 *  Lưu ý:
 *    - Không chống timing side-channel.
 * ====================================================================== */
ECPoint ecc_scalar_mul(uint64_t k, ECPoint P) {
    ECPoint R = {0,0,true}; /* R = ∞ */
    ECPoint Q = P;
    while (k) {
        if (k & 1) R = ecc_add(R, Q);
        Q = ecc_add(Q, Q);
        k >>= 1;
    }
    return R;
}

/* ========================== TÍNH BẬC CỦA ĐIỂM ===========================
 *  Tìm số nhỏ nhất k sao cho k*P = ∞ (ước tính đơn giản để minh hoạ).
 *  Vì p nhỏ nên cách lặp tuyến tính vẫn chạy được (nhưng không tối ưu).
 * ====================================================================== */
static uint64_t order_of_point(ECPoint P) {
    ECPoint R = P;
    uint64_t k = 1;
    while (!R.infinity && k < g_curve.p + 2) {
        R = ecc_add(R, P); /* tăng dần: 2P, 3P, 4P, ... */
        ++k;
    }
    return k;
}

/* ============================= KHỞI TẠO CURVE ===========================
 *  - Đảm bảo G nằm trên đường cong; nếu không, tìm điểm hợp lệ đầu tiên
 *    (minh hoạ) và gán làm G.
 *  - Tính bậc n = order(G).
 * ====================================================================== */
void ecc_init_curve(void) {
    if (!is_on_curve(g_curve.G)) {
        /* Tìm 1 điểm hợp lệ (cách brute-force cho “toy”) */
        for (uint64_t x=1; x<g_curve.p; ++x) {
            for (uint64_t y=1; y<g_curve.p; ++y) {
                ECPoint cand = (ECPoint){x,y,false};
                if (is_on_curve(cand)) { g_curve.G = cand; goto found; }
            }
        }
    }
found:
    g_curve.n = order_of_point(g_curve.G);
}

/* =========================== HASH → SỐ NHỎ =============================
 *  Chuyển 32 byte hash (SHA-256) thành số nguyên “z” trong [1, m-1]
 *   - Ở đây lấy 8 byte cao (h[0..7]) rồi mod m (toy).
 *   - Đảm bảo z != 0 (nếu 0 thì đặt thành 1).
 * ====================================================================== */
static uint64_t hash_to_int(const uint8_t h[32], uint64_t m) {
    uint64_t z = 0;
    for (int i=0;i<8;i++) z = (z<<8) | h[i];
    z %= m;
    if (z == 0) z = 1;
    return z;
}

/* ============================== KEYGEN ==================================
 *  Q = d * G
 *  - Yêu cầu: 1 <= d < n
 *  - Trả false nếu d mod n = 0
 * ====================================================================== */
bool ecdsa_keygen(uint64_t priv, ECPoint* pub) {
    if (g_curve.n == 0) ecc_init_curve();
    priv %= g_curve.n;
    if (priv == 0) return false;
    *pub = ecc_scalar_mul(priv, g_curve.G);
    return true;
}

/* =============================== SIGN ===================================
 *  ECDSA ký trên hash:
 *    Input: hash32 (SHA-256(message)), d (private), đầu ra (r, s)
 *    B1) z = H(m) → số trong [1, n-1]
 *    B2) Chọn nonce k trong [1, n-1], với gcd(k, n) = 1
 *        (TOY: k = (z + d) mod n, rồi hiệu chỉnh → không an toàn!)
 *    B3) R = kG = (xR, yR); r = xR mod n; nếu r=0 → chọn k khác
 *    B4) s = k^{-1} * (z + r*d) mod n; nếu s=0 → chọn k khác
 * ====================================================================== */
bool ecdsa_sign(const uint8_t hash32[32], uint64_t priv, uint64_t* r, uint64_t* s) {
    if (g_curve.n == 0) ecc_init_curve();

    /* z trong [1, n-1] */
    uint64_t z = hash_to_int(hash32, g_curve.n);

    /* d hợp lệ */
    priv %= g_curve.n; if (priv == 0) return false;

    /* TOY NONCE (KHÔNG AN TOÀN): k = (z + d) mod n; lặp tăng nếu không khả nghịch */
    uint64_t k = (z + priv) % g_curve.n; if (k == 0) k = 1;
    while (ugcd(k, g_curve.n) != 1) { k = (k + 1) % g_curve.n; if (k == 0) k = 1; }

    /* Lặp đến khi có (r, s) đều khác 0 */
    while (1) {
        /* R = kG */
        ECPoint R = ecc_scalar_mul(k, g_curve.G);

        /* r = xR mod n; r != 0 */
        uint64_t r0 = R.x % g_curve.n;
        if (r0 == 0) { k = (k + 1) % g_curve.n; if (!k) k=1; continue; }

        /* k^{-1} mod n */
        uint64_t kinv = invmod(k, g_curve.n);
        if (!kinv) { k = (k + 1) % g_curve.n; if (!k) k=1; continue; }

        /* s = k^{-1} * (z + r*d) mod n; s != 0 */
        uint64_t s0 = (kinv * ((z + (r0 * priv) % g_curve.n) % g_curve.n)) % g_curve.n;
        if (s0 == 0) { k = (k + 1) % g_curve.n; if (!k) k=1; continue; }

        *r = r0; *s = s0;
        return true;
    }
}

/* ============================== VERIFY ==================================
 *  Kiểm tra chữ ký (r, s) cho hash32 và Q:
 *    Yêu cầu: 0 < r,s < n; Q hợp lệ; n đã biết.
 *    B1) z = H(m) → số trong [1, n-1]
 *    B2) w = s^{-1} mod n
 *    B3) u1 = z*w mod n ; u2 = r*w mod n
 *    B4) X = u1*G + u2*Q = (xX, yX)
 *    B5) Hợp lệ nếu (xX mod n) == r và X != ∞
 * ====================================================================== */
bool ecdsa_verify(const uint8_t hash32[32], ECPoint pub, uint64_t r, uint64_t s) {
    if (g_curve.n == 0) ecc_init_curve();

    /* Điều kiện biên: r,s trong (0, n) */
    if (r==0 || r>=g_curve.n || s==0 || s>=g_curve.n) return false;

    /* z trong [1, n-1] */
    uint64_t z = hash_to_int(hash32, g_curve.n);

    /* w = s^{-1} mod n */
    uint64_t w = invmod(s, g_curve.n);
    if (!w) return false;

    /* u1, u2 */
    uint64_t u1 = (z * w) % g_curve.n;
    uint64_t u2 = (r * w) % g_curve.n;

    /* X = u1*G + u2*Q */
    ECPoint X = ecc_add(ecc_scalar_mul(u1, g_curve.G), ecc_scalar_mul(u2, pub));
    if (X.infinity) return false;

    /* Kiểm tra xX mod n == r */
    return (X.x % g_curve.n) == r;
}
