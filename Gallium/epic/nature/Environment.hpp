#ifndef __ENVIRONMENT__
#define __ENVIRONMENT__

#include <Ogre.h>
#include <SDL2/SDL.h>
#include <Bites/OgreApplicationContext.h>
#include <Bites/OgreInput.h>
#include <map>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <boost/asio.hpp>
#include <thread>
#include <functional>
#include <nlohmann/json.hpp>
#include "PhysicsManager.hpp"
#include "PhysicsFrameListener.hpp"
#include "StaticObject.hpp"
#include "Character.hpp"

using boost::asio::ip::udp;
using json = nlohmann::json;

#define GAME_PORT 12345
#define NEW_PLAYER 0
#define LIST 1

namespace world
{
namespace nature
{
	typedef struct Network
	{
        std::shared_ptr<udp::socket> socket;
        udp::endpoint server_endpoint;
        udp::endpoint sender_endpoint;
        std::array<char, 1024> rbuffer;

        Network() = default;
	} Network;

	class Environment : public OgreBites::ApplicationContext, public OgreBites::InputListener
	{
	private:
		Ogre::Root* root_;
		Ogre::SceneManager* sceneManager_;
		Ogre::RTShader::ShaderGenerator* shaderGenerator_;
		std::map<Ogre::String, bool> events_;
		std::map<Ogre::String, Ogre::SceneNode*> sceneNodes_;
		//std::vector<Epic::Entity::Movable::Character*> characters_;
		std::map<std::string, Epic::Entity::Movable::Character*> characters_;
		std::map<Ogre::String, std::map<Ogre::String, Ogre::AnimationState*>> animations_;
		world::physics::PhysicsManager* physics_;
		Network* net_;
		json data;
		std::string id_;
		int index_;
		std::vector<std::shared_ptr<Epic::Entity::Movable::Character>> players_;
		boost::asio::io_context io_context;
		std::thread io_thread_;

		void setupSceneNodes();
		void setupLights();
		void setupCameras();
		void setupEntities();
		void setupGrounds();

	public:
		Environment() = default;

		void init();
		void initNetwork();
		void startReceive();
		void setup() override;
		bool frameRenderingQueued(const Ogre::FrameEvent&) override;
		bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
		bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
		bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
	};
}
}

#endif
