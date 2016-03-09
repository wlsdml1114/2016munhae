#include <iostream>
using namespace std;
int main(){
	int n=0,m=0,k=0,a[1000];
	for(int i=0;i<1000;i++)
		a[i]=true;
	cin>>n>>m>>k;
	int c=1,t=m%n,r=0;
	a[t]=false;
	if(t==0)cout<<n<<" ";
	else cout<<t<<" ";
	while(true){
		if(a[t]==true)r++;
		if(r==k){
			r=0;
			a[t]=false;
			if(t==0)	cout<<n<<" ";
			else cout<<t<<" ";
			c++;
			if(c==n)break;
		}
		t=(t+1)%n;
	}
	return 0;
}
