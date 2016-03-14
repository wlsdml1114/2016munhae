#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctype.h>
using namespace std;
int main(){
	char c;
	string str="";
	vector <string> vec;
	while(getline(cin,str)){
		string str2="";
		for (int i = 0; i < str.length(); ++i)
		{
			if(str[i]>='a'&&str[i]<='z'){
				str2+=str[i];
			}
			else if(str[i]>='A'&&str[i]<='Z'){
				str2+=(str[i]+32);
			}
			else {
				vec.push_back(str2);
				str2="";
			}
		}
		vec.push_back(str2);
		str2="";

	}
	sort(vec.begin(),vec.end());
	string distinct="";
	for (int i = 0; i < vec.size(); ++i)
	{	
		if(distinct.compare(vec[i])){
			cout<<vec[i]<<endl;
			distinct=vec[i];
			continue;
		}
	}
	return 0;
}