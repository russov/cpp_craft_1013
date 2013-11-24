#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <vector>
#include <market_message.h>

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
}*/

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
};

std::string in_file = BINARY_DIR "/2.4_example.in";
std::string out_file = BINARY_DIR "/out.txt";
std::vector<market_message> messages;
BinaryReader *reader;
int main()
{	
	reader = new BinaryReader();
	std::ifstream is(in_file, std::ifstream::binary);
	if (is){
		messages = (*reader).readAll(is);
	}
	std::ifstream os(out_file, std::ofstream::binary);
}
