#pragma once
#include "./header.h"

/// (A, agg, e) Monoid
//e wegoptimierbar, aber IMMER einfach in A und agg einbaubar und spart hier viel Code
template<typename A = z, auto agg = sum, const A& e = ZERO>
struct iterSegT {
    z n;
    vec<A> v;

    iterSegT(vec<A> s) : n(s.size()), v(n, e) {
        v.insert(end(v), be(s));
        for (z i = n; --i;)
            v[i] = agg(v[2 * i], v[2 * i + 1]);
    }
    iterSegT(z n) : n(n), v(2 * n, e) {}

    void set(z i, A a) {
        v[i += n] = a;
        while (i /= 2)
            v[i] = agg(v[2 * i], v[2 * i + 1]);
    }

    A query(z l, z rex) {
        A la = e, ra = e;
        for (l += n, rex += n; l < rex; l /= 2, rex /= 2) {
            if (l & 1) la = agg(la, v[l++]);
            if (rex & 1) ra = agg(v[--rex], ra);
        }
        return agg(la, ra);
    }
};
