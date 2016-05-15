#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <cctype>
#include <stack>
#include <queue>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>
#include <bitset>
#include <utility>
#include <set>
#include <numeric>
 
#define INF_MAX 2147483647
#define INF_MIN -2147483647
#define pi acos(-1.0)
#define N 1000000
#define LL long long
 
#define For(i, a, b) for( int i = (a); i < (b); i++ )
#define Fors(i, sz) for( size_t i = 0; i < sz.size (); i++ )
#define Fore(it, x) for(typeof (x.begin()) it = x.begin(); it != x.end (); it++)
#define Set(a, s) memset(a, s, sizeof (a))
#define Read(r) freopen(r, "r", stdin)
#define Write(w) freopen(w, "w", stdout)
 
using namespace std;
 
int routers;
vector <int> matrix [300 + 5];
int parent [300 + 5];
int hop_count [300 + 5];
 
void reset ()
{
    for ( int i = 0; i < 305; i++ ) {
        parent [i] = i;
        hop_count [i] = INF_MAX;
    }
}
 
void bfs (int s, int t)
{
    queue <int> q;
    q.push (s);
 
    hop_count [s] = 0;
 
    while ( !q.empty () ) {
        int p = q.front (); q.pop ();
 
        if ( p == t ) return;
 
        Fors (i, matrix [p]) {
            if ( hop_count [matrix [p] [i]] == INF_MAX ) {
                parent [matrix [p] [i]] = p;
                hop_count [matrix [p] [i]] = hop_count [p] + 1;
                q.push (matrix [p] [i]);
            }
        }
    }
}
 
void print_path (int t)
{
    vector <int> path;
    path.push_back (t);
 
    while ( parent [t] != t ) {
        t = parent [t];
        path.push_back (t);
    }
 
    reverse (path.begin (), path.end ());
 
    printf ("%d", path [0]);
 
    for ( size_t i = 1; i < path.size (); i++ ) printf (" %d", path [i]);
    printf ("\n");
}
 
int main ()
{
    //Read ("in.in");
    //Write ("out.txt");
 
    while ( scanf ("%d", &routers) != EOF ) {
        getchar ();
 
        for ( int i = 0; i < 305; i++ ) matrix [i].clear ();
 
        for ( int i = 1; i <= routers; i++ ) {
            char inp [1000]; gets (inp);
            char *pch;
            pch = strtok (inp, "-,");
            int id = atoi (pch);
            pch = strtok (NULL, "-,");
 
            while ( pch != NULL ) {
                int sees = atoi (pch);
                matrix [id].push_back (sees);
                pch = strtok (NULL, "-,");
            }
        }
 
        int m; scanf ("%d", &m);
        printf ("-----\n");
 
        while ( m-- ) {
            reset ();
            int start, dest; scanf ("%d %d", &start, &dest);
            bfs (start, dest);
            if ( hop_count [dest] == INF_MAX ) printf ("connection impossible\n");
            else print_path (dest);
        }
    }
 
    return 0;
}