#pragma once
#include "./header.h"

struct unionFind {
    vz par;
    vz h; //height of subtree. only correct for roots, but that's ok

    union_find(z sz): par(sz), h(sz) {
        iota(be(par), 0);
    }
    z find(z a) {
        return a - par[a] ? par[a] = find(par[a]) : par[a];
    }
    bool same(z a, z b) {
        return find(a) == find(b);
    }
    void unite(z a, z b) {
        a = find(a), b = find(b);
        if(a == b) return;
        if(h[a] < h[b]) swap(a, b);
        par[b] = a;
        h[a] += h[a] == h[b];
    }
};
