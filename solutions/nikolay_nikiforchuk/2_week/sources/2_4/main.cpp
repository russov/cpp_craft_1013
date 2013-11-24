#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <../binary_reader/market_message.h>

/*class Message
{
	public:
		const static enum TYPES {
			MARKET_OPEN = 1,
			TRADE = 2,
			QUOTE = 3,
			MARKET_CLOSE = 4
		};
		Message(const uint32_t type, const uint32_t time, 
			const uint32_t len, const char msg){
				this->type = type;
				this->time = time;
				this->len = len;
				*this->msg = msg;
		}
		~Message(){}

		uint32_t get_type();
		uint32_t get_time();
		uint32_t get_len();
		char get_msg();
		void toString();

	private:
		uint32_t type;
		uint32_t time;
		uint32_t len;
		char *msg;
};*/

/*Message::Message(const uint32_t type, const uint32_t time,
	const uint32_t len, const char msg){
		this->type = type;
		this->time = time;
		this->len = len;
		*this->msg = msg;
}

uint32_t market_message::type(){
	return this->type;
};
uint32_t market_message::time(){
	return this->time;
};
char market_message::msg(){
	return *this->msg;
};
void Message::toString(){
	std::cout << "msg:" << this->msg() << "::type:"
		<< this->type() << "::time:" << this->time() << std::ends;
}

class IBinaryReader{
	public:
		virtual market_message readMessage(std::ifstream &is);
};

class BinaryReader : IBinaryReader {
	public:
		BinaryReader();
		market_message readMessage(std::ifstream &is);
		std::vector<market_message> readAll(std::ifstream &is);
		//TODO implements set message filter
		~BinaryReader();
};

market_message BinaryReader::readMessage(std::ifstream &is){
	market_message *message;
	uint32_t type;
	uint32_t time;
	uint32_t len;
	char *msg;
	is.read((char*)&type, sizeof(type));
	is.read((char*)&time, sizeof(time));
	is.read((char*)&len, sizeof(len));
	if ((uint32_t)len > 0){
		msg = new char[(uint32_t)len];
		is.read((char*)msg, (uint32_t)len);
	} else {
		msg = "";
		std::cout << "Zero lengh message found" << std::ends;
	}
	message = new market_message(type, time, len, *msg);
	(*message).toString();
	return *message;
};

std::vector<market_message> BinaryReader::readAll(std::ifstream &is){
	std::vector<market_message> messages;
	if (is){
		messages.push_back(readMessage(is));
	}
	return messages;
};*/
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

