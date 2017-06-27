#include <iostream>
#include<vector>
using namespace std;

typedef vector<int> VI;
VI func() {
return vector<int>{1};
}

int main(){
	cout<<func()[0]<<endl;
	return 0;
}
