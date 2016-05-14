#include <iostream>
#include <string.h>
#include <algorithm>
#include <stdio.h>
using namespace std;
#define WHITE 0
#define GRAY 1
#define BLACK 2
#define clear( A , X ) memset ( A , X , sizeof A )
#define REP( i , n ) for ( int i = 0 ; i < n ; ++ i )
#define FF( i , a , b ) for ( int i = a ; i <= b ; ++ i )

const int maxN = 20005 ;
const int maxM = 50005 ;
const int maxS = 20005 ;
const int maxE = 1000005 ;

typedef struct{
	int edge[101];
	int num;
}Ver;
int color[101];
int pred[101];
int d[101];
int f[101];
int timee;
int answer1;
int answer2;
int n;
Ver ver[101];
Ver ver_r[101];
// int reverse[100];
int rev;
// int scc[100][100];
int num_scc[101];
int count_scc;
Ver DAG[101];


struct Edge {
	int v,n;
	Edge(){}
	Edge(int var,int next):v(var),n(next){}
};

struct SCC{
	Edge edge[maxE];
	int adj[maxN],cntE;
	int scc[maxN],ins[maxN],Dfn[maxN],Low[maxN],scc_cnt;
	int S[maxS],top,dfs_clock;
	int in[maxN],ou[maxN];
	
	void addedge(int u,int v){
		edge[cntE]=Edge(v,adj[u]);
		adj[u]=cntE++;
	}
	
	void Tarjan(int u){
		S[top++]=u;
		ins[u]=1;
		Dfn[u]=Low[u]=++dfs_clock;
		for(int i=adj[u];~i;i=edge[i].n){
			int v=edge[i].v;
			if(!Dfn[v]){
				Tarjan(v);
				Low[u]=min(Low[u],Low[v]);
			}
			else if(ins[v]){
				Low[u]=min(Low[u],Dfn[v]);
			}
		}
		if(Low[u]==Dfn[u]){
			++scc_cnt;
			while(1){
				int v=S[--top];
				ins[v]=0;
				scc[v]=scc_cnt;
				if(v==u)break ;
			}
		}
	}
	
	void Init(){
		top=0;
		cntE=0;
		scc_cnt=0;
		dfs_clock=0;
		clear(Dfn,0);
		clear(ins,0);
		clear(adj,-1);
	}
	
	void Find_SCC(int n){
		REP(i,n){
			// cout<<i<<endl;
			if(!Dfn[i])
				Tarjan(i);
		}
	}
	
	void Solve(int n){
		if(scc_cnt==1){
			printf("0");
			return;
		}
		clear(in,0);
		clear(ou,0);
		FF(u,1,n)
			for(int i=adj[u];~i;i=edge[i].n){
				int v=edge[i].v;
				if(scc[u]!=scc[v]){
					ou[scc[u]]=1;
					in[scc[v]]=1;
				}
			}
		int No_In=0,No_Out=0;
		FF(i,1,scc_cnt){
			if(!in[i])++No_In;
			if(!ou[i])++No_Out;
		}
		printf("%d\n",max(No_In,No_Out));
	}
};

SCC scc;


// void DFS_Visit_reverse(int in){
// 	color[in] = GRAY;
// 	timee+=1;
// 	d[in] = timee;

// 	for(int j=0;j<ver_r[in].num;++j){
// 		if(color[ver_r[in].edge[j]]==WHITE){
// 			pred[ver_r[in].edge[j]] = in;
// 			DFS_Visit_reverse(ver_r[in].edge[j]);
// 		}
// 	}
// 	color[in] = BLACK;
// 	timee+=1;
// 	reverse[rev++] = in;
// 	f[in] = timee;
// 	// scc[count_scc][num_scc[count_scc]++] = in;
// 			// cout<<in<<" : "<<d[in]<<f[in]<<endl;
// }

void DFS_Visit(int in){
	color[in] = GRAY;
	timee+=1;
	d[in] = timee;

	for(int j=0;j<ver[in].num;++j){
		if(color[ver[in].edge[j]]==WHITE){
			pred[ver[in].edge[j]] = in;
			DFS_Visit(ver[in].edge[j]);
		}
	}
	color[in] = BLACK;
	timee+=1;
	// reverse[rev++] = in;
	f[in] = timee;

			// cout<<in<<" : "<<d[in]<<f[in]<<endl;
}

void DFS(int num){
	answer1=0;
	for(int i=0;i<n;++i){
		if(color[(i+num)%n]==WHITE){
			// cout<< i<<" ";
			answer1++;
			DFS_Visit((i+num)%n);
		}
	}
}

// void DFS_reverse(){
// 	count_scc=0;
// 	answer2=0;
// 	for(int i=0;i<n;++i){
// 		if(color[i]==WHITE){
// 			// cout<< i<<" ";
// 			DFS_Visit_reverse(i);
// 			count_scc++;
// 		}
// 	}
// }


void work(){
	// int n,m,u,v;
	scc.Init();
	// scanf("%d%d",&n,&m);
	// REP(i,m){
	// 	scanf("%d%d",&u,&v);
	// 	scc.addedge(u,v);
	// }
	for(int i=0;i<n;++i){
		for(int j=0;j<ver_r[i].num;++j){

			scc.addedge(i,ver_r[i].edge[j]);
			// cout<<i<<"-"<<ver_r[i].edge[j]<<endl;
		}
		// cout<<endl;
	}
	// cout<<"asdf1"<<endl;
	scc.Find_SCC(n);
	// cout<<"asdf2";
	scc.Solve(n);
}

int main(){
	int input,answer=9999;
	cin>>n;
	for(int i=0;i<n;++i){
		color[i]=WHITE;
		pred[i]=-1;
		cin>>input;
		while(1){
			if(input==0)break;
			ver[i].edge[ver[i].num++]=input-1;
			cin>>input;
		}
	}
	for(int i=0;i<n;++i){
		for(int j=0;j<ver[i].num;++j){
			// cout<<i<<" "<<ver[i].edge[j]<<endl;
		}
	}
	timee =0;
	for(int i=0;i<n;++i){

		for(int j=0;j<n;++j){
			color[j]=WHITE;
			pred[j] = -1;
		}
		DFS(i);
		// cout<< answer1<<endl;
		answer = min(answer,answer1);
	}
	cout<<answer<<endl;
	rev=0;
	timee=0;



	for(int i=0;i<n;++i){
		color[i]=WHITE;
		pred[i]=-1;
	}

	for(int i=0;i<n;++i){
		for(int j=0;j<ver[i].num;++j){
			ver_r[ver[i].edge[j]].edge[ver_r[ver[i].edge[j]].num++] = i;
			// cout<<ver[i].edge[j];
		}
	}

	// for(int i=0;i<n;++i){
	// 	for(int j=0;j<ver_r[i].num;++j){
	// 		cout<<ver_r[i].edge[j];
	// 	}
	// 	cout<<endl;
	// }

	work();
	return 0;

}