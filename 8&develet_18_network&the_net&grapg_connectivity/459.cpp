#include <iostream>
#include <string>
#include <string.h>
#include <cstdio>
#include <stdlib.h>

using namespace std;

class UF {
  int *id, cnt, *sz;
  public:
// Create an empty union find data structure with N isolated sets.
void initial(int N) {
    cnt = N; id = new int[N]; sz = new int[N];
    for (int i = 0; i<N; i++)  id[i] = i, sz[i] = 1; }
~UF() { delete[] id; delete[] sz; }

// Return the id of component corresponding to object p.
int find(int p) {
    int root = p;
    while (root != id[root])    root = id[root];
    while (p != root) { int newp = id[p]; id[p] = root; p = newp; }
    return root;
}
// Replace sets containing x and y with their union.
void merge(int x, int y) {
    int i = find(x); int j = find(y); if (i == j) return;
    // make smaller root point to larger one
    if (sz[i] < sz[j]) { id[i] = j, sz[j] += sz[i]; }
    else { id[j] = i, sz[i] += sz[j]; }
    cnt--;
}
// Are objects x and y in the same set?
bool connected(int x, int y) { return find(x) == find(y); }
// Return the number of disjoint sets.
int count() { return cnt; }
};

int main(){
    string s;
    getline(cin,s);
    // cout<<s[0];
    int n = atoi(s.c_str());
    // cout<< n;
    // cin>>n;
    // cout<< n;
    for(int i=0;i<n;++i){
        string str;
        // char num;
        // cin>>num;
        getline(cin,str);
        // cout<< num;
        while(str.compare("")==0)getline(cin,str);
        // cout<< str;
        // cout<<num;
        UF uf;
        uf.initial((str[0]-'A'+1));
        // cout<<num-'A'+1<<endl;
        // char c1,c2;
        // scanf("%c%c\n",&c1,&c2);
        getline(cin,str);
        string temp=str;
        while(str.compare("")!=0){
            // cout<<str[0]<<str[1]<<endl;
            temp=str;
            // getline(cin,str);
            uf.merge(str[0]-'A',str[1]-'A');
            // cout<<str[0]<<str[0]-'A'<<str[1]<<str[1]-'A'<<endl;
            getline(cin,str);
            
            if(str.compare(temp)==0)break;
            // uf.merge(str[0]-'A',str[1]-'A');
            // if(scanf("%c%c\n",&c1,&c2)==0)break;
        }
        cout<<uf.count()<<endl;
        if(i<n-1)cout<<endl;
    }
    return 0;
}