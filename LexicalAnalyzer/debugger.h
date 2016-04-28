#ifndef DEBUGGER_H_INCLUDED
#define DEBUGGER_H_INCLUDED

#include <bits/stdc++.h>

#define O4 10007
#define O5 100003
#define O6 1000003
#define O7 10000019
#define O8 100000007
#define O9 1000000007
#define all(v) ((v).begin()), ((v).end())
#define rall(v) ((v).rbegin()), ((v).rend())
#define sz(v) ((int)((v).size()))
#define setZero(a) memset(a,0,sizeof(a))
#define setNeg(a) memset(a,-1,sizeof(a))
#define fa(i,n) for(int i=0;i<n;i++)
#define fb(i,n) for(int i=1;i<=n;i++)
#define fz(i,n) for(int i=n-1;i>=0;--i)
#define rep(i,v) for(int i=0;i<sz(v);++i)
#define rap(i,v) for(int i=sz(v)-1;i>=0;--i)
#define tr(it,v) for(typeof((v).begin()) it = (v).begin(); it != (v).end(); it++)
#define print(arr,n,str) { cout<<str<<" : [ " ; fa(_i,n){ cout<<arr[_i]<<" "; } cout<<"]"<<endl;;}
#define pnt(v,s) print(v,sz(v),s)
#define pntAd(adj) rep(i,adj){ cout<<i+1<<" : " ; rep(j,adj[i])cout<<adj[i][j]+1<<" "; cout<<endl; ; }
#define mod(n,m) ((n%m)+m)%m
#define pb(n) push_back(n)
#define mp(n,m) make_pair(n,m)
#define dbg(n) cout<<"Debug "<<n<<endl
#define debg(s,n) cout<<s<<" : "<<n<<endl
#define ln cout<<"\n"
using namespace std;

#endif // DEBUGGER_H_INCLUDED
