#include "GameServer.hpp"
#include <boost/asio.hpp>
#include <iostream>

using namespace Net::Server;

int main()
{
	try
	{
		boost::asio::io_context io_context;
		GameServer server(io_context, 12345);

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}