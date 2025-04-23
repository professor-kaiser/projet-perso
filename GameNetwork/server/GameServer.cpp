#include "GameServer.hpp"
#include <iostream>

namespace Net
{
namespace Server
{
	GameServer::GameServer(boost::asio::io_context& io_context, short port)
	: socket_(io_context, udp::endpoint(udp::v4(), port))
	{
		run();
	}

	void
	GameServer::run()
	{
		auto generate_uuid = []() -> std::string
		{
			boost::uuids::random_generator generator;
			boost::uuids::uuid uuid { generator() };

			return boost::uuids::to_string(uuid);
		};

		socket_.async_receive_from(
            boost::asio::buffer(rbuffer_), remote_endpoint_,
            [this, generate_uuid](const boost::system::error_code& error, std::size_t bytes_transferred) 
            {
                if (!error && bytes_transferred > 0) 
                {
                	std::string message(rbuffer_.data(), bytes_transferred);

                	if(nlohmann::json::accept(message.begin(), message.end()))
                	{
	                	json response = json::parse(message);;

	                	if(endpoints_.find(remote_endpoint_) == endpoints_.end())
	                	{
	                		response["id"] = generate_uuid();

	                		endpoints_.insert({remote_endpoint_, response["id"]});
	                		players_.push_back(response["id"]);
/*
	                		for(auto endpoint : endpoints_)
	                		{
	                			players.push_back(endpoint.second);
	                		}
*/
	                		response["players"][LIST] = players_;
	                		response["players"][NEW_PLAYER] = true;
	                	}

	                	for(const auto& endpoint : endpoints_)
	                    {
		                    socket_.async_send_to(
		                        boost::asio::buffer(response.dump()), endpoint.first,
		                        [this](const boost::system::error_code& error, std::size_t) 
		                        {
		                            if (error) {
		                            }
		                            run();
		                    	}
		                    );
		                }
	                }
	            }

	            else {
	                std::cout << "Error receiving: " << error.message() << std::endl;
	                run();
	            }
            });
	}
}
}