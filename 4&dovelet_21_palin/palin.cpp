

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

int arr[5001][5001];

int main(){
	int length;
	cin>>length;
	getchar();
	char c1[length+1], c2[length+1];
	for(int i=1;i<=length;i++){
		char ch;
		cin>>ch;
		c1[i] = ch;
		c2[length-i+1]=ch;
	}
	arr[0][0]=0;
	for(int i=0;i<length;++i){
		arr[i][0]=0;
		arr[0][i]=0;
	}
	for (int i = 1; i <=length; ++i)
	{
		for (int j = 1; j <= length; ++j)
		{
			if(c1[i] == c2[j])
				arr[i][j] = arr[i-1][j-1]+1;
			else{
				arr[i][j] = max(arr[i-1][j],arr[i][j-1]);
			}
		}
	}
	cout<< length - arr[length][length];
	return 0;
}