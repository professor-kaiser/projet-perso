#ifndef __GAME_SERVER__
#define __GAME_SERVER__

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>
#include <map>
#include <string>

using boost::asio::ip::udp;
using json = nlohmann::json;

#define DEBUG true
#define NEW_PLAYER 0
#define LIST 1

namespace Net
{
namespace Server
{
	/*
	typedef struct Character
	{
		int id;
		int action;
	} Character;
	*/

	typedef struct Position
	{
		float x;
		float y;
		float z;
	} Position;

	typedef struct Rotation
	{
		float w;
		float x;
		float y;
		float z;
	} Rotation;

	typedef struct Character
	{
		int action;
	} Character;

	class GameServer
	{
	private:
		udp::endpoint remote_endpoint_;
		std::map<udp::endpoint, std::string> endpoints_;
    	udp::socket socket_;
    	std::array<char, 1024> rbuffer_;
    	std::vector<std::string> players_;

    	void run();

    public:
    	GameServer(boost::asio::io_context& io_context, short port);
	};
} // namespace Server
} // namespace Net

#endif