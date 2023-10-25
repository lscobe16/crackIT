#pragma once
#include "./header.h"

/// todo: Knotengewichte ermöglichen; siehe hld
/// (A, agg, e) Monoid, agg kommutativ
/// Gewicht einer Kante muss in beide Richtungen gleich sein
//(e) und (Kommutativität und gleiche Gewichte) jew. wegoptimierbar, aber IMMER einfach in A und agg einbaubar und spart hier viel Code
template<typename A, auto agg, auto e>
struct binLifting {
    z n;
    z max_exp = log2(n); //rounding down is ok; max path len is n-1
    vvz anc;
    vec<vec<A>> jmp;
    vz dep;

    void dfs(z u, auto &g) {
        for (z i = 0; i < max_exp; ++i) {
            anc[u].push_back(anc[anc[u][i]][i]);
            jmp[u].push_back(agg(jmp[u][i], jmp[anc[u][i]][i]));
        }
        for(auto& [v, w]:g[u]) if(v-anc[u][0]) {
            anc[v] = {u};
                jmp[v] = {w};
            dep[v] = dep[u]+1;
            dfs(v, g);
        }
    }

    bin_lft(vec<vec<pair<z, A>>> g, z r = 0): n(g.size()), anc(n), jmp(n), dep(n) {
        anc[r] = {r};
        jmp[r] = {e};
        dfs(r, g);
    }

    void lft(z &u, A& a, z exp) {
        a = agg(a, jmp[u][exp]);
        u = anc[u][exp];
    }

    pair<z, A> lca(z u, z v) {
        z d = dep[u] - dep[v];
        if(d < 0) swap(u, v);
        d = abs(d);
        A a = e;
        for (z i = max_exp; i + 1; --i) if(1 << i & d) lft(u, a, i);
        if(u != v) {
            for (z i = max_exp; i + 1; --i) if (anc[u][i] - anc[v][i])
                lft(u, a, i), lft(v, a, i);
            lft(u, a, 0), lft(v, a, 0);
        }
        return {u, a};
    }
};
