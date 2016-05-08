
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;
int n, cond, m, g[20][20];
string topic[20];
int ch[20];
void dfs(int idx, int i) {
    if(idx == m) {
        for(i = 0; i < idx; i++) {
            if(i)   putchar(' ');
            printf("%s", topic[ch[i]].c_str());
        }
        puts("");
        return ;
    }
    int j;
    for(; i < n; i++) {
        for(j = 0; j < idx; j++)
            if(g[i][ch[j]] == 1)
                break;
        if(j == idx) {
            ch[idx] = i;
            dfs(idx+1, i+1);
        }
    }
}
bool cmp(string a, string b) {
    if(a.length() != b.length())
        return a.length() > b.length();
    return a < b;
}
int main() {
    int testcase, cases = 0;
    scanf("%d", &testcase);
    while(testcase--) {
        scanf("%d %d %d", &n, &cond, &m);
        memset(g, 0, sizeof(g));
        int i, j, k;
        for(i = 0; i < n; i++) {
            cin >> topic[i];
            for(j = 0; j < topic[i].length(); j++) {
                if(topic[i][j] >= 'a' && topic[i][j] <= 'z')
                    topic[i][j] -= 32;
            }
        }
        sort(topic, topic+n, cmp);
        map<string, int> R;
        for(i = 0; i < n; i++)
            R[topic[i]] = i;
        while(cond--) {
            string tx, ty;
            cin >> tx >> ty;
            for(i = 0; i < tx.length(); i++) {
                if(tx[i] >= 'a' && tx[i] <= 'z')
                    tx[i] -= 32;
            }
            for(i = 0; i < ty.length(); i++) {
                if(ty[i] >= 'a' && ty[i] <= 'z')
                    ty[i] -= 32;
            }
            int x = R[tx], y = R[ty];
            g[x][y] = g[y][x] = 1;
        }
        printf("Set %d:\n", ++cases);
        dfs(0, 0);
        puts("");
    }
    return 0;
}
