#include "Message.h"

using namespace std;

void Message::ReadFrom(istream& is) {
	is.read(reinterpret_cast<char*>(&type_), sizeof(type_));
	is.read(reinterpret_cast<char*>(&time_), sizeof(time_));
	unsigned text_length = 0;
	is.read(reinterpret_cast<char*>(&text_length), sizeof(text_length));
	text_.resize(text_length);
	for (unsigned i = 0; i < text_length; ++i) {
		char x = ' ';
		is.read(&x, sizeof(x));
		text_[i] = x;
	}
}
void Message::WriteTo(ostream& os) const {
	os.write(reinterpret_cast<const char*>(&type_), sizeof(type_));
	os.write(reinterpret_cast<const char*>(&time_), sizeof(time_));
	unsigned text_length = static_cast<unsigned>(text_.length());
	os.write(reinterpret_cast<const char*>(&text_length), sizeof(text_length));
	for (size_t i = 0; i < text_.length(); ++i) {
		const char x = text_[i];
		os.write(&x, sizeof(x));
	}
}
