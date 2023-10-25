#pragma once
#include "./header.h"

z modmul(z a, z b, z M) {
    return (__int128_t) a * b % M;
}

z modpow(z a, z b, z M) {
    if(b == 1) return a;
    z x = modpow(a, b / 2, M);
    x = modmul(x, x, M);
    return b % 2 ? modmul(a, x, M) : x;
}

car wit = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
/// n >= 0
bool prime(z n) {
    if(n < 41) return count(be(wit), n);

    z d = n - 1, s = 0;
    while(d % 2 == 0) d /= 2, ++s;

    for (z a : wit) {
        z x = modpow(a, d, n), r = 0;
        if(x != 1 && x != n-1)
            while(++r < s && (x = modmul(x, x, n)) != n-1);
        if(r >= s) return 0; //== not equiv.!
    }
    return 1;
}

/// n > 1, n composite
/// returns divisor of n other than n or 1
z rho(z n) {
    if (n % 2 == 0) return 2;
    auto f = [=](z x) { return (modmul(x, x, n) + 1) % n; };
    for (z x0 = 0; x0 < n; ++x0) {
        z x = x0, y = f(x), g;
        while ((g = gcd(x - y, n)) == 1)
            x = f(x), y = f(f(y));
        if(g != n) return g;
    }
    err
}

/// n > 0
vz factors(z n) {
    if(n == 1) return {};
    if(prime(n)) return {n};
    z r = rho(n);
    vz v0 = factors(r), v1 = factors(n / r);
    v0.insert(v0.end(), be(v1));
    return v0;
}

/// a, b coprime
array<z, 3> gcd_ext(z a, z b) {
    if(!a) return {b, 0, 1};
    auto [d, x1, y1] = gcd_ext(b % a, a);
    return {d, y1 - (b / a) * x1, x1};
}

// better use crt
pzz _crt(z a1, z m1, z a2, z m2) { DBT //todo: how big may lcm(m1, m2) be? use modmul???
    auto [g, x, y] = gcd_ext(m1, m2);
    z l = m1 / g * m2;
    z ret = (a1 + ((x * (a2-a1) / g) % (l/m1)) * m1) % l;
    return {(a2-a1) % g ? -1 : (ret + l) % l, l};
}

/// lcm := lcm(m_1, ...)
/// if there is x in [0, lcm) so that x ≡ a_i (mod m_i), return {x, lcm}        ///there can be at most one such x, or no int at all
/// else return {-1, lcm}
pzz crt(vz a, vz m) { DBT //todo: how big may lcm(m_1, ...) be (before overflows happen)? use modmul???
    z sol = a[0], l = m[0];
    for(z i=1; sol+1 && i<a.size(); i++)
        tie(sol, l) = _crt(sol, l, a[i], m[i]);
    return {sol, l};
}
