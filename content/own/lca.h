#pragma once
#include "./iterSegT.h"

struct lca {
    using ST = iterSegT<z, []fn2r(min(a, b)), INF>;

    vz n2t, t2n, t2pt; //node2time, time2node, time2par's_time
    ST *t2ptST;

    void dfs(z u, z p, vvz &g) {
        n2t[u] = t2n.size();
        t2n.push_back(u);
        t2pt.push_back(n2t[p]);
        for (z v: g[u]) if (v-p) dfs(v, u, g);
    }
    lca(vvz g, z r = 0) : n2t(g.size()) {
        dfs(r, r, g);
        for (z i = 0; i < g.size(); ++i) if(i - r && !n2t[i]) dfs(i, i, g);
        t2ptST = new ST(t2pt);
    }

    z of(z u, z v) {
        if(u == v) return u;
        auto [l, r] = minmax(n2t[u], n2t[v]);
        return t2n[t2ptST->query(l+1, r+1)]; //ignore left node's par bc left node might be lca and bc if left node's par is lca, some node on the path lca-->right node will have that par, too
    }
    /// dist(u, v) = dep(u) + dep(v) - 2 dep(lca(u, v))
};
