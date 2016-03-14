#include	<iostream>
#include	<stack>
#include <stdlib.h>
using namespace std;
int main(){
	int  sum;
	while(cin>>sum){
		int b=0,count=0,su=0,check=0;
		char c, ca[1000000];
		stack <int> st;
		while(cin>>c){
			if(c=='('){
				b++;
				ca[count++]=c;
			}
			else if(c==')'){
				b--;
				ca[count++]=c;
			}
			else if(c>='0'&& c<='9'){
				ca[count++]=c;
			}
			else if(c=='-'){
				ca[count++]=c;
			}
			if (b==0)break;
		}
		int intflag=0;
		for(int i=0;i<count-1;i++){
			if(ca[i]=='(')intflag=0;
			if((ca[i]>='0'&&ca[i]<='9'&&intflag==0)||ca[i]=='-')//is digit?
			{
				st.push(atoi(&ca[i]));
				su+=st.top();
				intflag++;
			}
			if(i>2){
				if(ca[i-1]==')'&&ca[i]==')'){
					su-=st.top();
					st.pop();
				}
			}
			if(i>4){
				if (ca[i-3]=='('&&ca[i-2]==')'&&ca[i-1]=='('&&ca[i]==')')
				{	
					if(su==sum){
						check++;
						break;
					}
				}
			}
		}
		if(check!=0){
			cout<<"yes"<<endl;
		}
		else{
			cout<<"no"<<endl;
		}
	}
	return 0;
}