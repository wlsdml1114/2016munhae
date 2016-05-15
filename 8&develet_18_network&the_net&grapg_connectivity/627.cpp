#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
#include <cstdio>
#include <stdlib.h>
#include <algorithm>

using namespace std;

int n;
vector <int> arr [305];
int pred [305];
int coun [305];
 
void init ()
{
    for ( int i = 0; i < 305; i++ ) {
        pred [i] = i;
        coun [i] = 2147483647;
    }
}
void print_path (int t)
{
    vector <int> path;
    path.push_back (t);
 
    while ( pred [t] != t ) {
        t = pred [t];
        path.push_back (t);
    }
 
    reverse (path.begin (), path.end ());
 
    printf ("%d", path [0]);
 
    for ( size_t i = 1; i < path.size (); i++ ) printf (" %d", path [i]);
    printf ("\n");
}
 
void bfs (int s, int t)
{
    queue <int> q;
    q.push (s);
 
    coun [s] = 0;
 
    while ( !q.empty () ) {
        int p = q.front (); q.pop ();
 
        if ( p == t ) return;
 
        // Fors (i, arr [p]) {
        for(size_t i=0;i<arr[p].size();++i){
            if ( coun [arr [p] [i]] == 2147483647 ) {
                pred [arr [p] [i]] = p;
                coun [arr [p] [i]] = coun [p] + 1;
                q.push (arr [p] [i]);
            }
        }
    }
}
 
 
int main ()
{
    //Read ("in.in");
    //Write ("out.txt");
 
    while ( scanf ("%d", &n) != EOF ) {
        getchar ();
 
        for ( int i = 0; i < 305; i++ ) arr [i].clear ();
 
        for ( int i = 1; i <= n; i++ ) {
            char inp [1000]; gets (inp);
            char *pch;
            pch = strtok (inp, "-,");
            int id = atoi (pch);
            pch = strtok (NULL, "-,");
 
            while ( pch != NULL ) {
                int sees = atoi (pch);
                arr [id].push_back (sees);
                pch = strtok (NULL, "-,");
            }
        }
 
        int m; scanf ("%d", &m);
        printf ("-----\n");
 
        while ( m-- ) {
            init ();
            int start, dest; scanf ("%d %d", &start, &dest);
            bfs (start, dest);
            if ( coun [dest] == 2147483647 ) printf ("connection impossible\n");
            else print_path (dest);
        }
    }
 
    return 0;
}