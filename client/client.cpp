#include <array>
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <string>
#include<sstream>
#include<fstream>
#include"order.h"

using namespace std;
using boost::asio::ip::tcp;

stringstream order;
string filename;
bool if_new;
map<string, string> tochange;
set<string> mykeys;

int main(int argc, char* argv[])
{
	boost::asio::io_service io_service;
	tcp::resolver resolver(io_service);
	tcp::resolver::query query("192.168.0.103", "9876");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::socket socket(io_service);
	boost::asio::connect(socket, endpoint_iterator);
	boost::system::error_code error;
	try {
		while (true) {

			//用户选择
			if (get_order()) {
				boost::asio::write(socket, boost::asio::buffer(order.str()), error);
				//收到回应	
				//如果是新发送的order，记录第一条回复(包含orderID等关键信息)
				std::array<char, 256> input_buffer;
				std::size_t rsize = socket.read_some(
					boost::asio::buffer(input_buffer), error);
				string info = string(input_buffer.data(), input_buffer.data() + rsize);
				cout << filename << endl;

				if (if_new)
					save_order(info, filename + ".txt");
				save_order(info, filename + "record.txt");

				std::cout << info << std::endl;

				int n = 0;
				while (n < 20) {
					std::array<char, 256> input_buffer;
					std::size_t rsize = socket.read_some(
						boost::asio::buffer(input_buffer), error);
					string response = string(input_buffer.data(), input_buffer.data() + rsize);
					save_order(response, filename + "record.txt");
					std::cout << response << std::endl;
					n++;
				}
			}
		}



		std::cin.get();
		std::cin.get();

		return 0;
	}
	catch (Quit q) {
		return 0;
	}

}


