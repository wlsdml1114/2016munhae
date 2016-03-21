#include <iostream>
#include<string>
#include <sstream>
#include<stdio.h>
#include<algorithm>

using namespace std;

struct goods{
	int amout;
	double price;
};
int main(){
	double unit_price;
	int number=1;		
	cout<<fixed;
	cout.precision(2);

	while(cin>>unit_price){
		cout<<"Case "<<number++<<":\n";
		int count;
		cin>>count;
		struct goods goods[count+1];
		goods[0].amout = 1;
		goods[0].price = unit_price;
		for(int i=1;i<=count;++i){
			cin>>goods[i].amout;
			cin>>goods[i].price;
		}
		string line;
		getchar();
		getline(cin,line);
		stringstream stream1;
		stream1.str(line);
		int amount_goods;
		double cal[count+1][101];
		for(int i=0;i<=100;i++){
			cal[0][i]=unit_price*i;
		}
		for(int i=1;i<=count;++i){
			for(int j=0;j<=100;j++){
				if(j<=goods[i].amout){
					cal[i][j] = min(cal[i-1][j],goods[i].price);
				}
				else{
					cal[i][j] = min(cal[i][j-goods[i].amout]+goods[i].price,min(cal[i-1][j],cal[i-1][j-goods[i].amout]+goods[i].price));
				}
			}
		}
		while(stream1>>amount_goods){
			cout<<"Buy "<<amount_goods<<" for $"<<cal[count][amount_goods]<<endl;
		}
	}
	return 0;
}