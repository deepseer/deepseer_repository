#include <iostream>
#include<vector>
using namespace std;

typedef vector<int> VI;
VI func() {
return vector<int>{1};
}

int main(){
	cout<<func()[1]<<endl;
	return 0;
}