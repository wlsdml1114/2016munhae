#include <iostream>
#include <algorithm>
using namespace std;

int mymax(int a, int b){
	return (a<b)?b:a;
}
int main(){
	int n,i,j,k;
	cin>>n;
	int arr[n],sum[n+1],d[4][n+1];
	cin>>arr[0];
	sum[0]=0;
	sum[1]=arr[0];
	for(i=1;i<n;++i){
		cin>>arr[i];
		sum[i+1]=sum[i]+arr[i];
	}
	int c=0,max=0,check=0;
	cin>>c;
	for(int i=0;i<=n;++i){
		d[0][i]=0;
	}
	for(int i=1;i<=3;++i){
		for(int j=c*(i-1);j<=n;++j){
			if(j<c*i){
				d[i][j] = sum[j];
			}
			else{
				d[i][j] = mymax(d[i][j-1],d[i-1][j-c]+sum[j]-sum[j-c]);
			}
		}
	}
	cout<<d[3][n]<<endl;
	return 0;
}