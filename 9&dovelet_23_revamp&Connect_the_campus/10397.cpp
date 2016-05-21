#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

vector <pair<double ,pair<int ,int>> > edge;
vector <int> set,vx,vy;


int find (int i){
    if(set[i]==i) return set[i];
    return set[i]=find (set[i]);
}

bool same(int i,int j){ return find (i)==find (j); }

void addset(int i,int j){ set[find (i)]=find (j); }

void get(int n){
    int a, b;
    for(int i=0;i<n;i++){
        scanf("%d %d",&a,&b);
        vx.push_back(a); vy.push_back(b);
    }
}

void calculate(int n){
    double dist;
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            dist=sqrt( (vx[i]-vx[j])*(vx[i]-vx[j]) + (vy[i]-vy[j])*(vy[i]-vy[j]) );
            pair<int ,int> tmp=make_pair(i,j);
            edge.push_back(make_pair(dist,tmp));
        }
    }
}

double get_sum(){
    double sum=0.0;
    for(int i=0;i<edge.size();i++){
        if(!same(edge[i].second.first,edge[i].second.second)){
            sum+=edge[i].first;
            addset(edge[i].second.first,edge[i].second.second);
        }
    }
    return sum;
}
int main(){
    int n,m,a,b;
    while(scanf("%d",&n)==1){
        set.assign(n,0);
        for(int i=0;i<n;i++) set[i]=i;
        edge.clear(); vx.clear(); vy.clear();
        get(n);
        calculate(n);

        scanf("%d",&m);
        while(m--){
            scanf("%d %d",&a,&b);
            a--,b--;
            if(!same(a,b)) addset(a,b);
        }

        sort(edge.begin(),edge.end());
        double sum=get_sum();
        printf("%0.2lf\n",sum);
    }
    return 0;
}