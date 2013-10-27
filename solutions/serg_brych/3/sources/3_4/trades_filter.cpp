#define BOOST_FILESYSTEM_VERSION 3
#include <iostream> 
#include <fstream> 
#include <algorithm>
#include <stdint.h>
#include <memory>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

static const uint32_t max_type = 4u;
static const int BufferSize = 2048;

struct deal
{
	uint32_t type;
	uint32_t time;
	uint32_t len;
};

typedef boost::shared_ptr<std::ifstream> shared_ifstream;
typedef boost::shared_ptr<std::ofstream> shared_ofstream;

void ThreadFunction(shared_ifstream input_file, shared_ofstream output_file)
{
	uint32_t max_time = 0;
	char buffer[BufferSize] = {0};
	deal current_deal ={0};
	bool store_deal = false;

	if(input_file->is_open() && output_file->is_open())
	{
		while(input_file->read(reinterpret_cast<char*>(&current_deal), sizeof(deal)))
		{
			store_deal = false;

			if((current_deal.type > 0) && ( current_deal.type <= max_type))
			{
				if(max_time < current_deal.time)
				{
					max_time = current_deal.time;
					store_deal = true;
				}
				else
				{
					if((current_deal.time + 2) > max_time)
					{
						store_deal = true;
					}
				}
			}

			if(store_deal)
			{
				output_file->write(reinterpret_cast<char*>(&current_deal), sizeof(deal));
				uint32_t read_bytes_msg = 0;
				uint32_t bytes_to_read = 0;
				while(read_bytes_msg < current_deal.len)
				{
					bytes_to_read = BufferSize < current_deal.len? BufferSize: current_deal.len;

					input_file->read(buffer, bytes_to_read);
					output_file->write(buffer, bytes_to_read);
					read_bytes_msg += bytes_to_read;
				}
				
			}
			else
			{
				std::streampos pos = input_file->tellg();
				input_file->seekg(pos + static_cast<std::streamoff>(current_deal.len));
			}
		}
	}
}

int main() 
{ 
	const std::string path = BINARY_DIR"\\";
	const boost::regex filter("input_\\d{3}.txt$");

	boost::filesystem::directory_iterator end_it;

	for(boost::filesystem::directory_iterator it(path); it != end_it; it++)
	{
		if(boost::filesystem::is_regular_file(it->status()))
		{
			boost::smatch what;
			if( boost::regex_match(it->path().leaf().string(), filter ) )
			{
				std::stringstream output_path;
				output_path << it->path().parent_path().string() << "\\output" << it->path().leaf().string().substr(5);
				
				std::cout << it->path()<< " to "  << std::endl<< output_path.str() << std::endl;



				ThreadFunction(
					boost::make_shared<std::ifstream>(it->path().c_str(), std::ifstream::binary),
					boost::make_shared<std::ofstream>(output_path.str(), std::ofstream::binary));
			}
		}
	}
	
	//uint32_t count_of_threads = boost::thread::hardware_concurrency() > 0 ? boost::thread::hardware_concurrency() : 2;
	//uint32_t current_threads = 0;
	//while(true)
	//{
	//	if(current_threads < count_of_threads)
	//	{
	//		
	//	}
	//}

	

	/*ThreadFunction(
		boost::make_shared<std::ifstream>(BINARY_DIR"\\input.txt",std::ifstream::binary),
		boost::make_shared<std::ofstream>(BINARY_DIR"\\output.txt",std::ofstream::binary));*/
	return 0;
}