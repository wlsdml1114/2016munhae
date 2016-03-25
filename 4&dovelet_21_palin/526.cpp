#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int mymin(int a, int b, int c){
	int ret=0;
	if(a<=b){
		if(a<=c)
			ret=1;
		else
			ret=3;
	}
	else if(b<=c){//a>b
		ret=2;
	}
	else{
		ret=3;
	}
	return ret;
}

int main(){
	string str1,str2;
	int first=0;
	while(getline(cin,str1)){
		getline(cin,str2);
		if(first!=0)cout<<endl;
		int d[str1.length()+1][str2.length()+1];
		d[0][0]=0;
		for(int i=1;i<=str1.length();++i){
			d[i][0] = i;
		}
		
		for(int i=1;i<=str2.length();++i){
			d[0][i] = i;
		}
		
		for(int i=1;i<=str1.length();++i){
			for(int j=1;j<=str2.length();++j){
				if(str1.at(i-1)==str2.at(j-1)){
					d[i][j]=d[i-1][j-1];
				}
				else{
					d[i][j] = min(min(d[i-1][j],d[i][j-1]),d[i-1][j-1])+1;
				}
				// int temp = (str1.at(i-1)==str2.at(j-1))?0:1;
				// d[i][j] = min(min(d[i-1][j],d[i][j-1])+1,d[i-1][j-1]+temp);
			}
		}
		cout<<d[str1.length()][str2.length()]<<endl;
		int i=str1.length(),j=str2.length(),count=1;
		//1x = i++j, 2x = i++j++,3x = ij++, x0 = do nothing x1 = replacement, x2 = insertion, x3 = deletion
		while(i!=0 ||j!=0){
			if (str1[i-1]==str2[j-1]){
				i--;
				j--;
				continue;
			}
			cout<<count++<<" ";
			if (i>0&&d[i][j]==d[i-1][j]+1) { // delete
				cout<<"Delete "<<i;
				str1.erase(i-1,1);
				i--;
			}
			else if (j>0&&d[i][j]==d[i][j-1]+1) { // insert
				cout<<"Insert "<<i+1<<","<<str2.at(j-1);
				str1.insert(i,1,str2.at(j-1));
				j--;
			}
			else if (i>0&&j>0) {  // replace
				cout<<"Replace "<<i<<","<<str2.at(j-1);
				str1[i-1]=str2[j-1];
				i--;
				j--;
			}
			cout<<endl;
			first++;
		}
		// while(i!=0||j!=0){
		// 	int x,y;
		// 	if(i==0){
		// 		x=0;
		// 		y=j-1;
		// 	}
		// 	if(j==0){
		// 		y=0;
		// 		x=i-1;
		// 	}
		// 	if(i!=0&&j!=0){
		// 		x=i-1;
		// 		y=j-1;
		// 	}
		// 	int check = mymin(d[x][y],d[x][j],d[i][y]);
		// 	if(check==1){
		// 		if(i==0){
		// 			track[x][y]=30;
		// 		}
		// 		else if(j==0){
		// 			track[x][y]=10;
		// 		}
		// 		else{
		// 			track[x][y] = 20;
		// 		}
		// 		if(d[x][y]!=d[i][j]){
		// 			if(i==0){
		// 				track[x][y]+=3;
		// 			}
		// 			else if(j==0){
		// 				track[x][y]+=2;
		// 			}
		// 			else{
		// 				track[x][y] +=1;
		// 			}
		// 		}
		// 		i=x;
		// 		j=y;
		// 	}
		// 	else if(check==2){
		// 		track[x][j]=10;
		// 		if(d[x][j]!=d[i][j]){
		// 			track[x][j]+=2;
		// 		}
		// 		i=x;
		// 	}
		// 	else{
		// 		track[x][y]=30;
		// 		if(d[i][y]!=d[i][j]){
		// 			track[i][y]+=3;
		// 		}
		// 		j=y;
		// 	}
		// }
		// int count=1,index=0;//index insert++ replacement nothing, delete --
		// while(i!=str1.length()||j!=str2.length()){
		// 	if(track[i][j]%10==0){
		// 		i++;
		// 		j++;
		// 	}
		// 	else if(track[i][j]%10==1){
		// 		cout<<count++<<" Replace "<<i+index<<","<<str2.at(j)<<endl;;
		// 		i++;
		// 		j++;
		// 	}
		// 	else if(track[i][j]%10==3){
		// 		cout<<count++<< " Insert "<<i+index++<<","<<str1.at(j)<<endl;
		// 		i++;
		// 	}else{
		// 		cout<<count++<<" Delete "<<i+index--<<endl;
		// 		j++;
		// 	}
		// }
		// cout<<endl;
	}
	return 0;
}
