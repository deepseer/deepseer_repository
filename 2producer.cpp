#include <stdexcept>
#include <iostream>
#include <boost/program_options.hpp>
#include "cppkafka/producer.h"
#include "cppkafka/configuration.h"
#include <chrono>
#include <sstream>

using std::string;
using std::exception;
using std::getline;
using std::cin;
using std::cout;
using std::endl;

using cppkafka::Producer;
using cppkafka::Configuration;
using cppkafka::Topic;
using cppkafka::MessageBuilder;

namespace po = boost::program_options;

std::string TimePointToString(const std::chrono::time_point<std::chrono::high_resolution_clock> &p);

int main(int argc, char*argv[])
{
	string brokers;
	string topic_name;
	int partition_value = -1;

	po::options_description options("Options");
	options.add_options()
	    ("help,h", "produce this help message")
	    ("brokers,b", po::value<string>(&brokers)->required(),
	    "the kafka broker list")
	    ("topic,t", po::value<string>(&topic_name)->required(),
	    "the topic in which to write to")
	    ("partition,p", po::value<int>(&partition_value),
	    "the partition to write into (unassigned if not provided)")
	;

	po::variables_map vm;

	try {
		po::store(po::command_line_parser(argc, argv).options(options).run(), vm);
		po::notify(vm);
	}
	catch (exception& ex) {
		cout << "Error parsing options: " << ex.what() << endl;
		cout << endl;
		cout << options << endl;
		return 1;
	}

	// Create a message builder for this topic
	MessageBuilder builder(topic_name);

	// Get the partition we want to write to. If no partition is provided, this will be
	// an unassigned one
	if (partition_value != -1)
		builder.partition(partition_value);

	// Construct the configuration
	Configuration config = {
		{"metadata.broker.list", brokers}
	};

	// Create the producer
	Producer producer(config);

	cout << "Producing messages into topic " << topic_name << endl;

	for (int i = 1; i <= 10; i++) {
		std::stringstream ss;
		ss << i;
		string str = ss.str();
		builder.key(str);
		builder.payload(str);

		auto time_point = std::chrono::high_resolution_clock::now();
		cout << "[" << TimePointToString(time_point) << "] ";
		cout << i << endl;

		producer.produce(builder);
		usleep(50000);
	}
	cout << "Waiting for in-flight messages to be produced..." << endl;
	usleep(2000000);
}

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