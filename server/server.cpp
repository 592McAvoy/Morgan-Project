#include <array>
#include <iostream>
#include <string>
#include<sstream>
#include<fstream>
#include<queue>
#include<vector>
#include"message.h"
#include <boost/asio.hpp>


using boost::asio::ip::tcp;
using namespace std;

vector<Order*> orders;
queue<string> response;
string filename;

int main()
{
	boost::asio::io_service io_service;
	tcp::acceptor acc(io_service, tcp::endpoint(tcp::v6(), 9876));


	while (1) {

		boost::system::error_code ignored;

		tcp::socket socket(io_service);
		acc.accept(socket);

		//��ȡ�û�����������
		std::array<char, 256> input_buffer;
		std::size_t input_size = socket.read_some(
			boost::asio::buffer(input_buffer),
			ignored);
		std::string order(input_buffer.data(),
			input_buffer.data() + input_size);

		//��������
		check(order);
		std::cout<< order << std::endl;
		
		//���ͻ�Ӧ
		save(filename);
		while (!response.empty()) {
			string mes = response.front()+"\n";
			response.pop();
			boost::asio::write(socket, boost::asio::buffer(mes), ignored);
		}		
		
		socket.shutdown(tcp::socket::shutdown_both, ignored);
		socket.close();
	}


	return 0;
}

