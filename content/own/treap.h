#pragma once
#include "./header.h"

/// if needed, wrap include with a namespace

#define tt template<typename T, typename B = void> //T: treap*, B: border type
const uz END = INF;
const bool INCL = true;

tt uz sz(T t) {return t ? t->s : 0;}
tt T L(T t) {return (T)t->l;}
tt T R(T t) {return (T)t->r;}
#define $l L(this)
#define $r R(this)

struct treap {
    z prio = rand();
    treap *l = 0, *r = 0;
    uz s = 1;

    void push(){} /// call before visiting/changing l or r
    void update(){s = sz(l) + sz(r) + 1;} /// call after changing (the pointers) l or r
};

template<typename A, auto aggr> /// aggr associative
struct atreap: treap {
    static constexpr auto aggrf = aggr; //(only) for bin_search
    A a;
    A agg = a;
    atreap(A _a): a(_a) {}

    void update() {
        agg = a;
        if($l) agg = aggr($l->agg, agg);
        if($r) agg = aggr(agg, $r->agg);
        treap::update();
    }
    void seta(A _a) {
        a = _a;
        update();
    }
};

/// aggr associative
/// (U, comp, id) Monoid
/// app(a, id, s) = a
/// aggr(app(a1, u, s1), app(a2, u, s2)) = app(aggr(a1, a2), u, s1+s2)
/// app(app(a, u1, s), u2, s) = app(comp(u2, u1), a, s)
template<typename A, auto aggr, typename U, const U& id, auto app, auto comp>
struct utreap: atreap<A, aggr> {
    U lazy = id;
    bool lazy_rev = false;

    void apply(U u) { /// apply update on range
        this->a = app(this->a, u, 1);
        this->agg = app(this->agg, u, this->s);
        lazy = comp(u, lazy);
    }
    void rev() { lazy_rev ^= 1; } /// don't use with non-commutative aggr or on treaps with key

    void push() {
        for(auto c : {$l, $r}) if(c) {
            c->apply(lazy);
            c->lazy_rev ^= lazy_rev;
        }
        lazy = id;
        if(lazy_rev) swap(this->l, this->r);
        lazy_rev = false;
    }
};

/// bool operator< must induce a strict total order on K
//ints wollen wir eh immer mit normalem < vergleichen (und sonst halt in struct auslagern)
template<typename K, typename T = treap>
struct ktreap: T {
    K k;
    ktreap(K _k, T t = {}): k(_k), T(t) {}
};


template<auto mix(const z&, const z&)>
using mix_add_treap = utreap<z, mix, z, ZERO, []fn3r(a + b), sum>;
using sum_add_treap = utreap<z, sum, z, ZERO, []fn3r(a + b * c), sum>;

const z MINZ = numeric_limits<z>::min();
template<auto mix(const z&, const z&)>
using mix_set_treap = utreap<z, mix, z, MINZ, []fn3r(b - MINZ ? b : a), []fn2r(b - MINZ ? b : a)>;
using sum_set_treap = utreap<z, sum, z, MINZ, []fn3r(b - MINZ ? b * c : a), []fn2r(b - MINZ ? b : a)>;



tt T split(T t, B b, bool after = false) { //default after = false for on, onr
    if(!t) return pair{t, t};
    t->push();
    bool spl;
    if constexpr (is_same_v<B, uz>) spl = b + after <= sz(t->l) || (b -= sz(t->l) + 1) & 0;
    else spl = after ? b < t->k : !(t->k < b);
    if(spl) {
        auto [l, r] = split(L(t), b, after);
        t->l = r;
        if(l) t->update();
        return pair{l, t};
    }
    else {
        auto [l, r] = split(R(t), b, after);
        t->r = l;
        if (r) t->update();
        return pair{t, r};
    }
}

tt T merge(T l, T r) {
    if (!l) return r;
    if (!r) return l;
    if (l->prio < r->prio) {
        l->push();
        l->r = merge(R(l), r);
        l->update();
        return l;
    }
    else {
        r->push();
        r->l = merge(l, L(r));
        r->update();
        return r;
    }
}

struct cod { //call on death
    function<void()> f;
    ~cod(){f();}
};
/// m is re-inserted into t at end of current scope
#define on(m, t, a, ...) auto [m##_, m##r] = split(t, __VA_ARGS__); auto [m##l, m] = split(m##_, a); cod ins##m{[&]{t = merge(merge(m##l, m), m##r);}};
#define onr(res, ...) [&]{on(m, __VA_ARGS__) return res;}()

tt T extract(T &t, B a, B b, bool bin = false) { //default bin = true for single element
    on(m, t, a, b, bin) return vec{m}[(z)(m=0)];
}

tt void insert(T &t, T o, B b, bool after = false) {
    on(m, t, b, b, after) m = merge(m, o); //bei "m=o" statt merge würde m gelöscht
}

/// (range) update, reverse (only for arraylike), query, contains (only for setlike) = on ...
/// delete = extract
/// move = extract + insert
/// order of key = onr(..., sz(ml))
/// order of key for unordered keys that may only occur once per treap: put par(ent) in struct treap; array/map keyToTreapPtr; then just accumulate sizes of left (unsauber) children on path to root

tt auto agg(T t, auto e) {return t ? t->agg : e;}

tt void traverse(T t, auto f) { //call f with treap -> can use a.a and a.k
    if(!t) return;
    t->push();
    traverse(L(t), f);
    f(*t);
    traverse(R(t), f);
}

#define to_str(t, f) [&]{str _=""; traverse(t, [&]fn{_+=f+' ';}); return _;}()    //to_str (und nicht print) für Debugging  //_, damit's nix shadowt

/// returns treap with every key that is in at least one of a and b
/// runs in O(m log(t/m)), where t=bigger size, m=smaller size      //https://www.cs.cmu.edu/afs/cs.cmu.edu/project/scandal/public/papers/treaps-spaa98.pdf
tt T unite(T a, T b) {
    if(!a) return b;
    if(!b) return a;
    if (a->prio < b->prio) swap(a, b);
    auto [l, r] = split(a, b->k);
    extract(r, b->k, b->k, INCL);
    b->push();
    b->l = unite(l, L(b));
    b->r = unite(r, R(b));
    b->update();
    return b;
}

tt void heapify(T v) {
    if (!v) return;
    T mx = v;
    if (L(v) && L(v)->prio > mx->prio) mx = L(v);
    if (R(v) && R(v)->prio > mx->prio) mx = R(v);
    if (mx != v) {
        swap(v->prio, mx->prio);
        heapify(mx);
    }
}
/// builds a treap in O(n) time
tt T* build(auto* A, uz n) {
    if (n == 0) return nullptr;
    uz mid = n / 2;
    T *v = new T{A[mid]};
    v->l = build<T>(A, mid);
    v->r = build<T>(A + mid + 1, n - mid - 1);
    heapify(v);
    v->update();
    return v;
}

/// returns size of smallest subrange starting {left, right}[rev] for which p returns true
/// returns (uz)-1 if there is no such range
/// p(aggr(a_l, ..., a_r)) must increase with increasing {r, l}[rev]
template<bool rev, typename T>
uz bin_search(T t, car p, auto neutralA) {
    //returning -1 can only happen at depth 0. Vorsicht beim Umordnen!
    if(p(neutralA)) return 0;
    if(!t) return -1;
    car aggr = [&]fn2r(t->aggrf(rev ? b : a, rev ? a : b));
    if(!p(aggr(neutralA, t->agg))) return -1; //aggring neutralA necessary; otherwise might return -1 for right children
    t->push();
    T l = L(t), r = R(t);
    if(rev) swap(l, r);
    if(l && p(aggr(neutralA, l->agg))) return bin_search<rev>(l, p, neutralA);
    if(p(aggr(l ? neutralA = aggr(neutralA, l->agg) : neutralA, t->a))) return sz(l) + 1;
    return sz(l) + 1 + bin_search<rev>(r, p, aggr(neutralA, t->a));
}

/// modify the single aggregate of one element at known position FAST
/// A change(A)
tt void change_one(T t, uz pos, car change) {
    t->push();
    T l = L(t);
    if(l && pos < sz(l)) change_one(l, pos, change);
    else if(pos > sz(l)) change_one(R(t), pos - sz(l) - 1, change);
    else t->a = change(t->a);
    t->update();
}

#undef tt
#undef $l
#undef $r
