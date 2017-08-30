#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

const int MAX = 15;

string last_str;
int last_count;
bool last_printed;
std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
char last_val;

char list[MAX] {'A', 'B', 'B', 'C', 'C', 'C', 'A', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D'};

// expected output:
// time (AAA)
// time [BBB]
// time repeat 2
// time {CCC}
// time repeat 2
// lasttime repeat 3
// time (AAA)
// time |DDD|
// time repeat 2
// time repeat 4
// lasttime repeat 7
//

std::string TimePointToString(const std::chrono::time_point<std::chrono::high_resolution_clock> &p)
{
	using namespace std::chrono;

	milliseconds ms = duration_cast<milliseconds>(p.time_since_epoch());

	seconds s = duration_cast<seconds>(ms);
	std::time_t t = s.count();
	std::size_t fractional_seconds = ms.count() % 1000;

	std::stringstream ss;
	char buffer[256];
	struct tm *lt = std::localtime(&t);
	std::strftime(buffer, 256, "%F %T", lt);
	ss << buffer << ".";
	if (fractional_seconds < 10) ss << "0";
	if (fractional_seconds < 100) ss << "0";
	ss << fractional_seconds;
	std::string timestr = ss.str();
	return timestr;
}

std::string CurrentTime()
{
	return TimePointToString(std::chrono::high_resolution_clock::now());
}

void nospam_print(char val)
{
	string output;

	if (val != '\0')
		output = string(3, val);
	else
		output = "";

	bool changed = (output != last_str);

	if (changed) {
		string print;
		// print last line's count if necessary
		if (!last_printed) {
			print = last_str;
			switch (last_val) {
				case 'A':
					cout << TimePointToString(last_time) << " (" << print << ")" << " x" << last_count << endl;
					break;
				case 'B':
					cout << TimePointToString(last_time) << " [" << print << "]" << " x" << last_count << endl;
					break;
				case 'C':
					cout << TimePointToString(last_time) << " {" << print << "}" << " x" << last_count << endl;
					break;
				case '\0':
//				cout << TimePointToString(last_time) << " NULL" << " x" << last_count << endl;
					break;
				default:
					cout << TimePointToString(last_time) << " |" << print << "|" << " x" << last_count << endl;
			}
		}
		// print new line
		print = output;
		last_time = std::chrono::high_resolution_clock::now();
		last_str = output;
		last_val = val;
		last_count = 1;
		last_printed = true;
		switch (val) {
			case 'A':
				cout << TimePointToString(last_time) << " (" << print << ")" << " x" << last_count << endl;
				break;
			case 'B':
				cout << TimePointToString(last_time) << " [" << print << "]" << " x" << last_count << endl;
				break;
			case 'C':
				cout << TimePointToString(last_time) << " {" << print << "}" << " x" << last_count << endl;
				break;
			case '\0':
//			cout << TimePointToString(last_time) << " NULL" << " x" << last_count << endl;
				break;
			default:
				cout << TimePointToString(last_time) << " |" << print << "|" << " x" << last_count << endl;
		}
	} else {
		last_count++;
		last_time = std::chrono::high_resolution_clock::now();
		bool do_print;
		int l = last_count;
		while ((l & 1) == 0)
			l >>= 1;
		if ((l == 1) && (last_count != 1)) // l = 2, 4, 8...
			do_print = true;
		else
			do_print = false;
		if (do_print) {
			string print = output;
			last_printed = true;
			switch (val) {
				case 'A':
					cout << TimePointToString(last_time) << " (" << print << ")" << " x" << last_count << endl;
					break;
				case 'B':
					cout << TimePointToString(last_time) << " [" << print << "]" << " x" << last_count << endl;
					break;
				case 'C':
					cout << TimePointToString(last_time) << " {" << print << "}" << " x" << last_count << endl;
					break;
				case '\0':
//				cout << TimePointToString(last_time) << " NULL" << " x" << last_count << endl;
					break;
				default:
					cout << TimePointToString(last_time) << " |" << print << "|" << " x" << last_count << endl;
			}
		} else {
			last_printed = false;
		}
	}
}

int main()
{
	last_str = "";
	last_val = '\0';
	last_count = 0;
	last_printed = true;
	for (int i = 0; i < MAX; i++) {
		nospam_print(list[i]);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	nospam_print('\0'); // send a dummy value to empty the cache
	return 0;
}
