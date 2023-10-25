#pragma once
#include <bits/stdc++.h>
using namespace std;

////////////////////////// types
using z = int64_t;
using uz = uint64_t;
#define int z /// in case I forget to use "z" or copy code containing "int"
using pzz = pair<z, z>;
using ld = long double;
using str = string;
#define vec vector
#define maxpq priority_queue
template<typename T>
using minpq = maxpq<T, vec<T>, greater<T>>;
template<typename W = z>
using G = vec<vec<pair<z, W>>>;

#define defv(T) using v##T = vec<T>;
using vb = vec<bool>;
defv(z)
defv(uz)
defv(ld)
defv(vz)
defv(pzz)
defv(vpzz)

using lz = list<z>;
using setz = set<z>;
using mapzz = map<z, z>;

#define car const auto&


////////////////////////// unseful z's
const z ZERO = 0; //only for iterSegT+treap
const z INF = 1e18; /// a better very big integer than numeric_limits<z>::max() ca.= 9e18 because it doesn't overflow as easily
const z PRIME = 1e9 + 7;


////////////////////////// functions
/// write fnr((stuff)) instead of fnr(stuff) when stuff.contains(',') (e.g. when returning a pair)      //other trick that we hopefully won't need: "#define $ ," and then write stuff.replace(',', " $ ") instead of stuff
/// pass lambda&'s to functions: use car instead of lambda&
#define fn (car a)
#define fn2 (car a, car b)
#define fn3 (car a, car b, car c)
#define fnr(res) fn{return res;}
#define fn2r(res) fn2{return res;}
#define fn3r(res) fn3{return res;}

#define cmpby(prop) [&]fn2r([&]fnr(prop)(a) < [&]fnr(prop)(b))


////////////////////////// for-each (mainly for IO)
void fe(car){}
template<typename T>
void fe(car f, vec<T>& v) {for(T& t : v) fe(f, t);}
void fe(car f, auto& t, auto&... ts) {f(t); fe(f, ts...);}
void fe(car... rv) {fe(rv...);} //for rvalues ("constants")
#define fe(f, ...) fe([](auto& a){f;}, __VA_ARGS__);


////////////////////////// input
/// in/ind can be used to (re-)input already declared vars, too
#define in(...) __VA_ARGS__; fe(cin >> a, __VA_ARGS__)
#define ind(...) in(__VA_ARGS__) fe(--a, __VA_ARGS__)

#define inv(T, v, sz) vec<T> v(sz); in(v)
#define invd(T, v, sz) vec<T> v(sz); ind(v)

#define inz(...) z in(__VA_ARGS__)
#define inzd(...) z ind(__VA_ARGS__)
#define invz(v, sz) inv(z, v, sz)
#define invzd(v, sz) invd(z, v, sz)


////////////////////////// output
#define out(...) fe(cout << a << '\n', __VA_ARGS__)
#define outs(...) fe(cout << a << ' ', __VA_ARGS__)
#define outse(...) {outs(__VA_ARGS__) out("")}


////////////////////////// misc
#define be(ctnr) ctnr.begin(), ctnr.end()
#define case break;case
#define default break;default
auto sum = []fn2r(a + b); //only for iterSegT+treap


////////////////////////// debugging
#define err exit(1);
#define asrt(cond) if(!(cond)) err

#define DB if(debug)
#define DBO DB cerr <<
#define DBT DB err /// "doubt"


////////////////////////// useful resources
/// https://en.cppreference.com/w/cpp/algorithm
/// https://en.cppreference.com/w/cpp/language/lambda#Lambda_capture
/// https://en.cppreference.com/w/c/language/eval_order
/// https://en.cppreference.com/w/c/language/struct_initialization#Nested_initialization
/// https://www.scs.stanford.edu/~dm/blog/
/// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments


////////////////////////// tips
/// Bei interaktiven Problemen: flushen nicht vergessen!
/// eine Zahl als Ausgabe: binäre Suche über dem Lösungsraum!
/// NEVER use   tie(a, b) = minmax(a, b)!!!


////////////////////////// reasons for some decisions
//no shorter for-loops; those are not faster to type than for/fe+Enter, but less flexible and understandable
//removed idvz; too much (typing) overhead
