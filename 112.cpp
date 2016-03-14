#include	<iostream>
#include	<stack>
#include <vector>
using namespace std;
int main(){
	int  sum;
	while(cin>>sum){
		char c=1,prec=2;
		int count=0,check=0;
		stack<int> st;
		st.push(sum);
		while(cin>>c){
			if(c=='('){
				int in;
				cin>>in;
				count+=in;
				if(in!=0){st.push(in);}
				prec = c;
			}
			if (c==')'){
				cout<<"asdf"<<endl;
				if(prec==c){
					if(sum==count){
						check=1;
						break;
					}
					else {
						count-=st.top();
						st.pop();
						cout<<count;
					}
				}
				prec =c;
			}
			if(st.empty())break;
		}
		if(check==1){
			cout<<"yes"<<endl;
		}
		else{
			cout<<"no"<<endl;
		}
	}
	return 0;
}