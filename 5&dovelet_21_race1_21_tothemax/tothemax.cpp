#include <iostream>
#include <algorithm>

using namespace std;

int main(){
	
	int n;
	cin>>n;
	int arr[n+1][n+1];
	for(int i=0;i<=n;i++){
		arr[0][i]=0;
		arr[i][0]=0;
	}
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			cin>>arr[i][j];
		}
	}
	int sum[n+1][n+1];
	for(int i=0;i<=n;++i){
		sum[0][i]=0;
		sum[i][0]=0;
	}
	// for(int i=1;i<=n;i++){
	// 	sum[1][i] = sum[1][i-1]+arr[1][i];
	// 	sum[i][1] = sum[i-1][1]+arr[i][1];
	// }
	
	for(int i=1;i<=n;i++){
		for(int j=1;j<=n;j++){
			sum[i][j] = sum[i-1][j]+sum[i][j-1]-sum[i-1][j-1]+arr[i][j];
		}
	}
	int maxi=-12800;
	
	for(int i=1;i<=n;i++){
		for(int k=1;k<=i;k++){
		int maxSofar=-12800;
		int maxendhere=-12800;
			for(int j=1;j<=n;j++){
				int cur = sum[i][j]-sum[i][j-1]-sum[k-1][j]+sum[k-1][j-1];
				maxendhere = max(maxendhere+cur,cur);
				maxSofar = max(maxSofar,maxendhere);
			}
			maxi = max(maxi,maxSofar);
		}
	}
	cout<< maxi;
	return 0;
}