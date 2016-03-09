#include<iostream>
#include<stdlib.h>
#include<string>
using namespace std;
int main(){
	string str;
	int i=0,first=0;;
	while(getline(cin,str)){
		const char *c = str.c_str();
		for(int k=0;k<str.length();k++){
			if(c[k]=='b'){
				for(int j=0;j<i;j++){
					cout<<' ';
				}
				i=0;
			}
			else if(c[k]=='!'){
				cout<<endl;
			}
			else if(c[k]>='0'&&c[k]<='9')i+=c[k]-'0';
			else if('A'<=c[k]&&c[k]<='Z'||c[k]=='*'){
				for(int j=0;j<i;j++){
					cout<<c[k];
				}
				i=0;
			}
			else{
				cout<<c[k];
			}
		}
		cout<<endl;
		
	}
	return 0;
}
