#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std ;

#define clear( A , X ) memset ( A , X , sizeof A )
#define REP( i , n ) for ( int i = 0 ; i < n ; ++ i )
#define FF( i , a , b ) for ( int i = a ; i <= b ; ++ i )

const int maxN = 20005 ;
const int maxM = 50005 ;
const int maxS = 20005 ;
const int maxE = 1000005 ;

struct Edge {
	int v , n ;
	Edge () {}
	Edge ( int var , int next ) : v(var) , n(next) {}
} ;

struct SCC {
	Edge edge[maxE] ;
	int adj[maxN] , cntE ;
	int scc[maxN] , ins[maxN] , Dfn[maxN] , Low[maxN] , scc_cnt ;
	int S[maxS] , top , dfs_clock ;
	int in[maxN] , ou[maxN] ;
	
	void addedge ( int u , int v ) {
		edge[cntE] = Edge ( v , adj[u] ) ;
		adj[u] = cntE ++ ;
	}
	
	void Tarjan ( int u ) {
		S[top ++] = u ;
		ins[u] = 1 ;
		Dfn[u] = Low[u] = ++ dfs_clock ;
		for ( int i = adj[u] ; ~i ; i = edge[i].n ) {
			int v = edge[i].v ;
			if ( !Dfn[v] ) {
				Tarjan ( v ) ;
				Low[u] = min ( Low[u] , Low[v] ) ;
			}
			else if ( ins[v] ) {
				Low[u] = min ( Low[u] , Dfn[v] ) ;
			}
		}
		if ( Low[u] == Dfn[u] ) {
			++ scc_cnt ;
			while ( 1 ) {
				int v = S[-- top] ;
				ins[v] = 0 ;
				scc[v] = scc_cnt ;
				if ( v == u ) break ;
			}
		}
	}
	
	void Init () {
		top = 0 ;
		cntE = 0 ;
		scc_cnt = 0 ;
		dfs_clock = 0 ;
		clear ( Dfn , 0 ) ;
		clear ( ins , 0 ) ;
		clear ( adj , -1 ) ;
	}
	
	void Find_SCC ( int n ) {
		FF ( i , 1 , n )
			if ( !Dfn[i] )
				Tarjan ( i ) ;
	}
	
	void Solve ( int n ) {
		if ( scc_cnt == 1 ) {
			printf ( "0\n" ) ;
			return ;
		}
		clear ( in , 0 ) ;
		clear ( ou , 0 ) ;
		FF ( u , 1 , n )
			for ( int i = adj[u] ; ~i ; i = edge[i].n ) {
				int v = edge[i].v ;
				if ( scc[u] != scc[v] ) {
					ou[scc[u]] = 1 ;
					in[scc[v]] = 1 ;
				}
			}
		int No_In = 0 , No_Out = 0 ;
		FF ( i , 1 , scc_cnt ) {
			if ( !in[i] ) ++ No_In ;
			if ( !ou[i] ) ++ No_Out ;
		}
		printf ( "%d\n" , max ( No_In , No_Out ) ) ;
	}
} ;

SCC scc ;

void work () {
	int n , m , u , v ;
	scc.Init () ;
	scanf ( "%d%d" , &n , &m ) ;
	REP ( i , m ) {
		scanf ( "%d%d" , &u , &v ) ;
		scc.addedge ( u , v ) ;
	}
	scc.Find_SCC ( n ) ;
	scc.Solve ( n ) ;
}

int main () {
	int T ;
	scanf ( "%d" , &T ) ;
	while ( T -- )
		work () ;
	return 0 ;
}
