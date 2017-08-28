#include <iostream>
#include <string>

using namespace std;

const int MAX = 10;

char last_val;
int last_count;

char list[MAX-1]{'A', 'B', 'C', 'C', 'C', 'A', 'D', 'D', 'D', 'D'}

// expected output:
// (A)
// [B]
// {C x3}
// (A)
// |D x4|
//

void nospam_print(char val)
{
	if (val != last_val) {
		if (last_val != '\0') {
			if (last_count > 1)
				last_message += (" x" + std::to_string(last_count));
			switch (val) {
				case 'A':
					cout<<"("<<last_message<<")"<<endl;
					break;
				case 'B':
					cout<<"("<<last_message<<")"<<endl;
					break;
				case 'C':
					cout<<"("<<last_message<<")"<<endl;
					break;
				default:
					cout<<"|"<<last_message<<"|"<<endl;
			}
		}
		last_val = val;
		last_count = 1;
	} else
		last_count ++;
}

int main()
{
	last_val = '\0';
	last_count = 0;
	for (i = 0; i < MAX; i++)
		nospam_print(list[i]);
	nospam_print('\0'); // send a dummy value to empty the cache
	return 0;
}