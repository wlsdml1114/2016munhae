#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <queue>
using namespace std;
typedef long long ll;
const int OF = 100;
const int INF = 0x3f3f3f3f;
const int N = 500;
int n, m, G[N][N], f[N][N];
void init() {
    memset(G, 0, sizeof(G));
    memset(f, 0, sizeof(f));
    int num;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &num);  
        G[i + OF][i] = num;
    }
    scanf("%d", &m);
    int a, b, c;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &a, &b, &c);
        G[a][b + OF] = c;
    }
    scanf("%d %d", &a, &b); 
    for (int i = 0; i < a; i++) {
        scanf("%d", &c);    
        G[0][c + OF] = INF;
    }
    for (int i = 0; i < b; i++) {
        scanf("%d", &c);
        G[c][n + OF + 1] = INF;
    }
}
int EK() {
    queue<int> Q;
    n = n + OF + 1;
    int pre[N], a[N], ans = 0;
    memset(f, 0, sizeof(f));    
    while (1) {
        memset(pre, 0, sizeof(pre));
        memset(a, 0, sizeof(a));
        a[0] = INF;
        Q.push(0);
        while (!Q.empty()) { 
            int u = Q.front(); Q.pop(); 
            for (int v = 0; v <= n; v++) {
                if (!a[v] && G[u][v] > f[u][v]) {
                    pre[v] = u;
                    Q.push(v); 
                    a[v] = min(a[u], G[u][v] - f[u][v]); 
                }
            }
        }
        if (a[n] == 0) break; 
        ans += a[n]; 
        for (int u = n; u != 0; u = pre[u]) { 
            f[pre[u]][u] += a[n]; 
            f[u][pre[u]] -= a[n]; 
        }

    }
    return ans;
}
int main() {
    while (scanf("%d", &n) != EOF) {
        init();
        printf("%d\n", EK());
    }   
    return 0;
}