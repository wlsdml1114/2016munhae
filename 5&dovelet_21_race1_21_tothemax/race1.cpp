#include <iostream>
#include <algorithm>

using namespace std;

struct oil_bank{
	int term;
	int minute;
};

int main(){
	int distance;
	cin>>distance;
	int n;
	cin>>n;
	struct oil_bank oil_bank[n+2];
	for(int i=1;i<=n+1;++i){
		cin>>oil_bank[i].term;
	}
	for(int i=1;i<n+1;++i){
		cin>>oil_bank[i].minute;
	}
	oil_bank[n+1].minute=0;
	int dp[n+2];
	dp[0]=0;
	
	for(int i=1;i<=n+1;++i){
		int cumdist=0;//누적거리
		dp[i]=2100000000;
		for(int j=i;j>0;--j){
			cumdist+=oil_bank[j].term;
			if(cumdist>distance)break;
			dp[i]=min(dp[i],dp[j-1]+oil_bank[i].minute);
		}
		// cout<<dp[i]<<endl;
	}
	cout<<dp[n+1];
	return 0;
}