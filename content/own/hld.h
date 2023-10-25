#pragma once
#include "./header.h"

//todo: document constraints for agg and updates
/// agg kommutativ (wegoptimierbar!?, aber spart hier viel Code)
template<bool nodes = true>
struct hld {
    z n;
    vuz par, dep, sz, heavy, head, pos; //vuz for calling f which might operate on a treap
    z p = 0;

    void dfs(z u, z p, vvz &g) {
        par[u] = p;
        z hsz = 0;
        for (z v: g[u]) if(v-p) {
            dep[v] = dep[u] + 1;
            dfs(v, u, g);
            sz[u] += sz[v];
            if(sz[v] > hsz) hsz = sz[heavy[u] = v];
        }
    }
    void decomp(z u, z h, vvz &g) {
        head[u] = h, pos[u] = p++;
        if(heavy[u]) decomp(heavy[u], h, g);
        for (z v: g[u]) if(v-par[u] && v-heavy[u]) decomp(v, v, g);
    }
    /// can initialize all nodes/edges in O(n) after construction, using this->pos. todo: void initDS/for_each/...!?
    hld(vvz &g): n(g.size()), par(n), dep(n), sz(n, 1), heavy(n), head(n), pos(n) {
        dfs(0, 0, g);
        decomp(0, 0, g);
    }

    /// O(log n) calls to f  (=> often time complexity O(log^2n))
    /// u, v inclusive
    void on_path(z u, z v, auto f) { /// void f(l, rex), where always l < rex. calls are not ordered!
        for(; head[u] - head[v]; v = par[head[v]]) {
            if(dep[head[u]] > dep[head[v]]) swap(u, v);
            f(pos[head[v]], pos[v] + 1);
        }
        auto [l, r] = minmax(pos[u], pos[v]);
        if (r+nodes-l) f(l, r + nodes); //conditional => no call to f with empty range
    }

    /// O(1) calls  (=> often time complexity O(log n))
    void on_tree(z r, auto f) { /// void f(l, rex), where always l < rex
        if(sz[r] + nodes > 1) f(pos[r] + !nodes, pos[r] + sz[r]); //conditional => no call to f with empty range
    }
};
