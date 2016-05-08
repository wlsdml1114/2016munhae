#include <iostream>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <map>

using namespace std;

int n, p, s;
int g[20][20];
string topic[20];
int ch[20];
bool cmp(string a, string b){
	if(a.length()!=b.length())
		return a.length() > b.length();
	return a<b;

}
struct convert{
	void operator()(char &c){c =toupper((unsigned char)c);}
};

void dfs(int idx,int i){
	int j=0;
	if(idx==s){
		for(i=0;i<idx;++i){
			if(i)cout<<' ';
			cout<<topic[ch[i]];
		}
		cout<<endl;
		return ;
		for(;j<idx;++j)
			if(g[i][ch[j]]==1)break;
		if(j==idx){
			ch[idx]=i;
			dfs(idx+1,i+1);
			
		}
	}
	for(;i<n;++i){
		for(j=0;j<idx;++j)
			if(g[i][ch[j]]==1)break;
		if(j==idx){
			ch[idx]=i;
			dfs(idx+1,i+1);
			
			
		}
	}
	for(;i<idx;++i){
		if(i<0)cout<<" ";
	}
}




int main(){
	int num;
	int k=1;
	cin>>num;
	// cout<<num;
	while(num--){
		cout<<"Set "<<k++<<":"<<endl;
		cin>>n>>p>>s;
		// cout<<n<<p<<s;

		for(int i=0;i<20;++i){
			for(int j=0;j<20;++j){
				g[i][j]=0;
			}
		}

		for (int i=0;i<n;++i){
			cin >>topic[i];
			// cout<< topic[i]<<endl;
			for_each(topic[i].begin(),topic[i].end(),convert());
		}

		sort(topic,topic+n,cmp);

		map<string,int> R;

		for(int i=0;i<n;++i) R[topic[i]]=i;

		while(p--){
			string tx, ty;
			cin >>tx>>ty;
			for_each(tx.begin(),tx.end(),convert());
			for_each(ty.begin(),ty.end(),convert());
			int x = R[tx], y = R[ty];
			g[x][y] = g[y][x]=1;
		}
		dfs(0,0);
		puts("");

	}
	return 0;
}