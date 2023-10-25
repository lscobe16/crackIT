
#include <bits/stdc++.h>

#define rep(a, b)   for(int a = 0; a < (b); ++a)
#define endl        '\n'

using namespace std;
using ll = int64_t;

struct edge {
    int from, to;
    int flow, cap;
    edge* rev;
};

vector<vector<edge*>> adj;
int s,t;
int INF = 2e9;

int dfs(int v, int aug, vector<int>& dist, vector<int>& next) {
    if (v == t) return aug;
    for (int& i = next[v]; i<adj[v].size(); ++i) {
        auto e = adj[v][i];
        if (e->flow == e->cap) continue;
        if (dist[e->to] != dist[v] + 1) continue;
        int pushed = dfs(e->to, min(aug, e->cap - e->flow), dist, next);
        if (pushed == 0) continue;
        e->flow += pushed;
        e->rev->flow -= pushed;
        return pushed;
    }
    return 0;
}

int main() {
    ios::sync_with_stdio(false);

    int n,m; cin >> n >> m;
    cin >> s >> t; --s; --t;
    adj.resize(n);
    rep(i,m) {
        int a,b,cap; cin>>a>>b>>cap;
        --a;--b;
        adj[a].push_back(new edge{a,b,0,cap,nullptr});
        adj[b].push_back(new edge{b,a,0,0,nullptr,});
        adj[a].back()->rev = adj[b].back();
        adj[b].back()->rev = adj[a].back();
    }


    ll flow =0;
    while(true) {
        vector<int> dist(n,INF);
        dist[s] = 0;
        queue<int> q{{s}};
        while(!q.empty()) {
            auto v = q.front(); q.pop();
            for(auto e : adj[v]) {
                if(dist[e->to] == INF && e->cap > e->flow) {
                    dist[e->to] = dist[v] + 1;
                    q.push(e->to);
                }
            }
        }
        if(dist[t] == INF) break;
        vector<int> next(n,0);
        int aug;
        while(aug = dfs(s,INF,dist,next)) flow += aug;
    }

    vector<pair<vector<edge*>,int>> paths;
    vector<int> next(n,0);
    while(flow>0) {
        paths.push_back({{},INF});
        auto& [path, aug] = paths.back();
        vector<bool> vis(n,false);
        while(path.empty() || path.back()->to != t) {
            int v = path.empty() ? s : path.back()->to;
            if(vis[v]) {
                int cycle_aug = INF;
                int cycle_begin = path.size()-1;
                while(path[cycle_begin]->from != v) cycle_begin--;
                for(int i=cycle_begin; i<path.size(); ++i)
                    cycle_aug = min(cycle_aug, path[i]->flow);
                for(int i=cycle_begin; i<path.size(); ++i) {
                    path[i]->flow -= cycle_aug;
                    vis[path[i]->to] = false;
                }
                path.resize(cycle_begin);
            }
            vis[v] = true;
            while(adj[v][next[v]]->flow <= 0) next[v]++;
            path.push_back(adj[v][next[v]]);
        }
        for(auto e : path) aug = min(aug, e->flow);
        flow -= aug;
        for(auto e : path) e->flow -= aug;
    }

    cout << paths.size() << endl;
    for(auto& [path,aug] : paths) {
        cout << aug << ' ' << path.size()+1 << ' ' << s+1 << ' ';
        for(auto e : path) cout << e->to+1 << ' ';
        cout << endl;
    }

    return 0;
}

