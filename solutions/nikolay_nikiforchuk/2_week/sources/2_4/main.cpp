#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <../binary_reader/market_message.h>

const static enum TYPES {
	MARKET_OPEN = 1,
	TRADE = 2,
	QUOTE = 3,
	MARKET_CLOSE = 4
};

std::string in_file = BINARY_DIR "/2.4_example.in";
std::string out_file = BINARY_DIR "/2.4_example.out";
std::vector<binary_reader::market_message*> messages;
std::vector<uint32_t> messageTypeFilter = {TYPES::MARKET_OPEN, TYPES::TRADE, TYPES::QUOTE, TYPES::MARKET_CLOSE};

bool isAllowedType(binary_reader::market_message &message){
	return find(messageTypeFilter.begin(), messageTypeFilter.end(), message.type())
		!= messageTypeFilter.end();
}

struct comparator {
	bool operator() (binary_reader::market_message *i, binary_reader::market_message *j){
		return (i->time() > j->time());
	}
} message_comparator;

int main()
{	
	std::ifstream is(in_file, std::ifstream::binary);
	std::ofstream os(out_file, std::ofstream::binary);
	uint32_t max_time;

	if (is){
		while (!is.eof()){
			binary_reader::market_message *message = new binary_reader::market_message(is);
			if (!is.eof() && isAllowedType(*message)){
				messages.push_back(message);
			}
		}
		std::sort(messages.begin(), messages.end(), message_comparator);
		max_time = (messages.front())->time();
		std::vector<binary_reader::market_message*>::iterator it = messages.begin();
		while (it != messages.end()){
			binary_reader::market_message* tmp_msg = *it;
			if (tmp_msg->time() <= (max_time - 2)){
				break;
			}
			tmp_msg->write(os);
			std::cout << "Write -> msg:" << tmp_msg->msg() << "::type:"
				<< tmp_msg->type() << "::time:" << tmp_msg->time() << std::ends;
			it++;
		}

		is.close();
		os.close();
	}
	else {
		std::cout << "No input file found " << in_file << std::endl;
	}
}

