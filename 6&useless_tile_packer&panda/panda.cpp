#include <iostream>
#include <algorithm>
using namespace std;

int in[500][500];
int res[500][500];
int n;	

int visit(int y, int x){
	int offsetx[4] ={0,0,1,-1}; 
	int offsety[4] ={1,-1,0,0};
	if(res[y][x]==1){
		for(int i=0;i<4;++i){
			int yy = offsety[i]+y;
			int xx = offsetx[i]+x;

			if(yy<0|| xx<0||yy>=n||xx>=n)continue;
			if(in[y][x]<=in[yy][xx])continue;
			res[y][x] = max(res[y][x],visit(yy,xx)+1);
		}
	}
	return res[y][x];
}
int main(){
	int result=-1;
	cin>>n;
	for(int i=0;i<n;++i){
		for(int j=0;j<n;j++){
			res[i][j]=1;
			cin>>in[i][j];
		}		
	}
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			result = max(visit(i,j),result);
		}
	}
	cout<<result<<endl;

	return 0;
}