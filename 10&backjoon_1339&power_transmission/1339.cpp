#include <iostream>
#include <cstdio>
#include <string.h>
using namespace std;
// int matrix[10][26];
int answer[26] = {};
int jaritsu[8] = {1,10,100,1000,10000,100000,1000000,10000000};
int main(){
	int n,m;
	scanf("%d",&n);
	m=n;
	while(m--){
		char ch[9];
		scanf("%s",ch);
		// printf("%d",strlen(ch));
		int j=0;
		for(int i=strlen(ch)-1;i>=0;--i){
			answer[ch[j++]-'A'] += jaritsu[i];
			// printf("%d",ch[j++]-'A');
		}
	}
	int sum=0;
	int num=9;
	for(int k=0;k<n;++k){
		int index=0;
		for(int l=0;l<26;++l){
			if(answer[index]<answer[l])
				index = l;
		}
		sum+=answer[index]*num--;
		answer[index]=0;
	}
	printf("%d",sum);
	return 0;
}