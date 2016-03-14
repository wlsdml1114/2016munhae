#include <iostream>

using namespace std;
int arr[21][21];
int check(int sx, int sy){
	int yoff[4]={0,1,1,1};
	int xoff[4]={1,1,0,-1};
	for (int i = 0; i < 4; ++i)
	{
		if(sx<5&&i==3)break;
		if((arr[sx][sy]!=arr[sx-xoff[i]][sy-yoff[i]])&&(arr[sx][sy]!=arr[sx+5*xoff[i]][sy+5*yoff[i]])){
			int temp=0;
			for (int k = 1; k <= 4; ++k)
				{
					if(arr[sx][sy]!=arr[sx+k*xoff[i]][sy+k*yoff[i]])
						temp++;
				}	
			if(temp==0){
				return 1;
			}

		}
		
	}
	return 0;
}
int main(){
	for (int i = 0; i < 21; ++i)
		for (int j = 0; j < 21; ++j)
			arr[i][j]=0;
	for (int i = 1; i < 20; ++i)
		for (int j = 1; j < 20; ++j)
			cin>>arr[i][j];

	for (int j = 1; j < 20; ++j)
	{
		for (int i = 1; i < 20; ++i)
		{
			if(arr[i][j]!=0){
				if(check(i,j)==1){
					cout<<arr[i][j]<<"\n"<<i<<" "<<j;
					return 0;
				}
			}
		}
	}
	cout<<"0";
	return 0;
}