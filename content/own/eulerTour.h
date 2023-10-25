#pragma once
#include "./lca.h"

/// todo: Knotengewichte ermöglichen; siehe hld
/// (A, agg, e) group with inverse inve
/// jede Kante muss in Rückrichtung inverses Element haben! (Gewicht "gerichtet")
template<typename A, auto agg, const A &e, auto inve>
struct eulerTour { //for edges with updates
    vz n2t; //node2(first)time
    map<pzz, z> e2t; //(directed)edge2time (before going over edge)
    vec<A> t2a; //time2A (at edge we'll go over next) //not updated by set
    iterSegT<A, agg, e> *t2aST;
    lca *t;

    void dfs(z u, z p, auto &g) {
        n2t[u] = t2a.size();
        for (auto [v, w]: g[u]) if (v - p) {
            e2t[{u, v}] = t2a.size();
            t2a.push_back(w);
            dfs(v, u, g);
            e2t[{v, u}] = t2a.size();
            t2a.push_back(inve(w));
        }
    }
    euler_tour(vec<vec<pair<z, A>>> g, z r = 0): n2t(g.size()) {
        dfs(r, r, g);
        for (z i = 0; i < g.size(); ++i) if(i - r && !n2t[i]) dfs(i, i, g);
        t2aST = new iterSegT<A, agg, e>(t2a);
        vvz g2(g.size()); for(z u = 0; u < g.size(); ++u) for(auto [v, w] : g[u]) g2[u].push_back(v);
        t = new lca(g2, r);
    }

    void set(z u, z v, A a) {
        t2aST->set(e2t[{u, v}], a);
        t2aST->set(e2t[{v, u}], inve(a));
    }

    A query(z u, z v) {
        z m = n2t[t->of(u, v)];
        auto [l, r] = minmax(n2t[u], n2t[v]);
        return agg(t2aST->query(m, l), t2aST->query(m, r));
    }
};
