#pragma once
#include "./header.h"

struct flow_edge {
    z from, to;
    z flow, cap;
    flow_edge* twin;
    z cost; //may be unused
};

vec<vec<flow_edge*>> flow_adj;
bool flow_directed;

//inits (or resets, if used before) flow
//if n is not big enough, grow as needed
//(only!) reason for n: one time I only resized flow_adj when needed and wanted to access flow_adj[k] for a treap k
//                      where all nodes j>=k had no edges => error
void init_flow(z n, bool directed) {
    flow_adj.assign(n, {});
    flow_directed = directed; err //todo: make dinic,... (in contest weglassbare) methods of class flow!?
}

void add_edge(z a, z b, z cap, z cost = 0) {
    auto ab = new flow_edge{a, b, 0, cap, nullptr, cost};
    auto ba = new flow_edge{b, a, 0, flow_directed ? 0 : cap, nullptr, -cost};
    ab->twin = ba;
    ba->twin = ab;
    flow_adj.resize(max((z) flow_adj.size(), max(a, b) + 1));
    flow_adj[a].push_back(ab);
    flow_adj[b].push_back(ba);
}

z dinic_dfs(z v, z aug, z t, vz &next, vz &dist) {
    if (v == t) return aug;
    for (z &j = next[v]; j < flow_adj[v].size(); ++j) {
        auto e = flow_adj[v][j];
        if (e->flow == e->cap) continue;
        if (dist[e->to] != dist[v] + 1) continue;
        if(z pushed = dinic_dfs(e->to, min(aug, e->cap - e->flow), t, next, dist)) {
            e->flow += pushed;
            e->twin->flow -= pushed;
            return pushed;
        }
    }
    return 0;
}

//computes max flow
//runs in O(mn^2), O(m sqrt(n)) in unit networks(*) (e.g. bipartite matching)
//*: when each vertex, except for source and sink, either has a single entering edge of capacity one,
//   or a single outgoing edge of capacity one, and all other capacities are arbitrary integers
z dinic (z s, z t) {
    flow_adj.resize(max((z) flow_adj.size(), max(s, t) + 1)); //just to be sure...
    z flow = 0;
    while (true) {
        // create layered network
        vz dist(flow_adj.size(), INF);
        dist[s] = 0;
        queue<z> q{{s}};
        while (!q.empty()) {
            auto v = q.front();
            q.pop();
            for (auto e: flow_adj[v]) {
                if (dist[e->to] == INF && e->flow < e->cap) {
                    q.push(e->to);
                    dist[e->to] = dist[v] + 1;
                }
            }
        }
        // break if no s-t path found
        if (dist[t] == INF) break;

        // while s-t path in L
        // augment path
        vz next(flow_adj.size());
        while (z aug = dinic_dfs(s,INF,t,next,dist)) flow += aug;
    }
    return flow;
}

//computes that max flow that has minimal cost
//successive shortest path algorithm (with shortest path faster algorithm)
//runs in O(nmB) where B=value of resulting flow (but in comprog we can assume it runs in O((n + m) log(n) B))
//other algorithm for MinCostFlow: cycle cancelling, but has an (almost) always worse runtime of O(nm^2UC) where U=max.cap, C=max.|cost|
pzz ssp(z s, z t) {
    flow_adj.resize(max((z) flow_adj.size(), max(s, t) + 1)); //just to be sure...
    z n = flow_adj.size();

    z f = 0, c = 0;

    while (true) {
        lz queue = {s};
        vb in_q(n);
        in_q[s] = true;
        vz d(n, INF);
        d[s] = 0;
        vec<flow_edge*> pare(n);

        while (!queue.empty()) { //runs endless if negative cycle
            z v = queue.front();
            queue.pop_front();
            in_q[v] = false;

            for (auto e: flow_adj[v]) {
                if (e->flow < e->cap) {
                    z to = e->to;
                    z d2 = d[e->from] + e->cost;
                    if (d2 < d[to]) {
                        d[to] = d2;
                        pare[to] = e;
                        if (!in_q[to]) {
                            in_q[to] = true;
                            queue.push_back(to);
                        }
                    }
                }
            }
        }

        if (d[t] == INF) break;

        z flow = INF;
        for (auto e = pare[t]; e; e = pare[e->from]) flow = min(flow, e->cap - e->flow);

        for (auto e = pare[t]; e; e = pare[e->from]) {
            e->flow += flow;
            e->twin->flow -= flow;
            c += e->cost * flow;
        }
        f += flow;
    }
    return {f, c};
}

struct flow_path {
    vz path;
    z flow;
};

//decomposes already computed(!) flow into paths
//VL: "Only works if null flow is cost minimal! e.g. if G does not contain negative cycles" (1. Teil stimmt aber nicht ganz) todo bei ssp? und meint, dass der Nicht-Fluss billiger sein muss als jeder andere Fluss mit Gesamt-s-t-fluss 0 (z.B. nicht so, wenns einen Zyklus gibt)
//runs in O(m^2)
//todo: O(nm) is possible //dafür cycles entfernen, siehe w01/decomp-ML.cpp
z decomp_dfs(z v, z aug, z t, vb &vis, vz& path) {
    path.push_back(v);
    vis[v] = true;
    if (v == t) return aug;
    for(z j = 0; j < flow_adj[v].size(); ++j) {
        auto e = flow_adj[v][j];
        z to = e->to;
        if (e->flow <= 0 or vis[to]) continue;
        z pushed = decomp_dfs(to, min(aug, e->flow), t, vis, path);
        if (!pushed) continue;
        e->flow -= pushed;
        e->twin->flow += pushed;
        return pushed;
    }
    path.pop_back();
    return 0;
}

vec<flow_path> decomp(z s, z t) {
    z n = flow_adj.size();
    vec<flow_path> paths;
    // while s-t path in L
    // augment path
    vz next(n), path;
    vb vis(n);
    while (z aug = decomp_dfs(s, INF, t, vis, path)) {
        paths.push_back({path, aug});
        path = {};
        vis = vb(n);
    }
    return paths;
}

//dinic: O(mn^2)
//ssp: O(nmFLOW), but in comprog we can assume it runs in O((n + m) log(n) FLOW)
//decomp: O(m^2) //todo: O(nm) is possible //dafür cycles entfernen, siehe w01/decomp-ML.cpp


void example() {
    init_flow(0, true);
}
