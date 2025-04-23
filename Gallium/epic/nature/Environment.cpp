#include "Environment.hpp"
#include "WCollisionListener.hpp"
#include <vector>
#include <iostream>
#include <OgreTagPoint.h>
#include <memory>

namespace world
{
namespace nature
{
	void 
	Environment::setup()
	{
		OgreBites::ApplicationContext::setup();
		this->init();

		this->setupSceneNodes();
		this->setupLights();
		this->setupGrounds();
		this->setupEntities();
		this->initNetwork();
		this->setupCameras();

		//sceneManager_->setSkyBox(true, "SkyBoxMaterial", 10000.0f);
	}

	void
	Environment::init()
	{
		this->root_ = getRoot();
		this->sceneManager_  = root_->createSceneManager();
		this->shaderGenerator_  = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		this->physics_ = new world::physics::PhysicsManager();

		this->root_->addFrameListener(new world::physics::PhysicsFrameListener(physics_));

		shaderGenerator_->addSceneManager(sceneManager_);
		addInputListener(this);
	}

	void
	Environment::initNetwork()
	{
		/// New Client
		data["new"] = true;

		/*-------------------------------------------------------------------*
		 * 			Establish first connection with server					 *
		 *-------------------------------------------------------------------*/
		net_ = new Network();
		udp::resolver resolver(io_context);

		/// CONNECTION: Initialize socket 
		net_->socket = std::make_shared<udp::socket>(io_context, udp::v4());

		/// CONNECTION: Resolve server endpoint
		net_->server_endpoint = *resolver.resolve(
			udp::v4(), 
			"127.0.0.1", 
			std::to_string(GAME_PORT)
		).begin();

		/// CONNECTION: Send first message (first connection) to server
		net_->socket->send_to(
			boost::asio::buffer(data.dump()), 
			net_->server_endpoint
		);

		/// CONNECTION: Receive initialisation
		size_t bytes_transferred = net_->socket->receive_from(
			boost::asio::buffer(net_->rbuffer), 
			net_->sender_endpoint
		);
		/*-------------------------------------------------------------------*/

		std::string rdata(net_->rbuffer.data(), bytes_transferred);
		json response = json::parse(rdata);

		id_ = response["id"];
		index_ = 0;

		data["id"]  = id_;
		data["players"][NEW_PLAYER] = false;

		if(!response["players"][LIST].is_null())
		{
			for(auto player : response["players"][LIST])
			{
				players_[index_]->getSceneNode()->setVisible(true);
				characters_.insert({player, players_[index_].get()});

				index_++;
			}
		}

		std::cout << "Id: " << id_ << std::endl;

		startReceive();
		io_thread_ = std::thread([this]() {
	        io_context.run();
	    });
	}

	/**
	 * @param std::function<void (json)> : synchronizeCharacter
	 * @return void
	 */
	void 
	Environment::startReceive()
	{
		/**
		 *  Control character animation and action
		 *
		 *	@param nlohman::json : response
		 *  @return void
		 */
		auto controlCharacter = [this](json response)
		{
			std::string id { response["id"].get<std::string>() };

			/// CONTROL: action and animation
		    characters_[id]->onEvent(
		    	!response["action"].is_null() ? response["action"].get<int>() : 0
		    );

            /// CONTROL: rotation and camera
            json act = response["act"][id];
			Ogre::Real px { !act["x"].is_null() ? act["x"].get<Ogre::Real>() : Ogre::Real { 0 } };
			Ogre::Real py { !act["y"].is_null() ? act["y"].get<Ogre::Real>() : Ogre::Real { 0 } };

            characters_[id]->act(
            	Ogre::Degree(px), 
            	Ogre::Degree(py)
            );
		};

		/**
		 *  synchronize Character position, rotation
		 *
		 *	@param nlohman::json : response
		 *  @return void
		 */
		auto synchronizeCharacter = [this](json response)
		{
			std::string i(response["id"]);

			if(characters_.find(i) == characters_.end())
			{
				players_[index_]->getSceneNode()->setVisible(true);
				characters_.insert({i, players_[index_].get()});
				index_++;
			}

			json position = response["position"][i];
			json rotation = response["rotation"][i];

        	if(i != id_ && !position.is_null() && !rotation.is_null())
        	{
			    btTransform transform;
			    transform.setIdentity();

			    /// SYNCHRONIZATION: position
			    transform.setOrigin(btVector3(
			    	position["x"].get<float>(),
			    	position["y"].get<float>(),
			    	position["z"].get<float>()
			    ));

			    /// SYNCHRONIZATION: rotation
			    transform.setRotation(btQuaternion(
				    rotation["x"].get<float>(),
				    rotation["y"].get<float>(),
				    rotation["z"].get<float>(),
				    rotation["w"].get<float>()
				));

			    characters_[i]->getRigidBody()
			    	->setWorldTransform(transform);
			}
		};

	    net_->socket->async_receive_from(
	        boost::asio::buffer(net_->rbuffer), 
	        net_->sender_endpoint, 
	        [this, synchronizeCharacter, controlCharacter]
	        (const boost::system::error_code& error, std::size_t bytes_transferred) 
	        {
	            if (!error && bytes_transferred > 0) 
	            {
	                std::string rdata(net_->rbuffer.data(), bytes_transferred);
	                if(nlohmann::json::accept(rdata.begin(), rdata.end()))
	                {
	                	json response = json::parse(rdata);

	                	/// CONTROL: Other character
						synchronizeCharacter(response);

	                	/// CONTROL: Local character
	                	controlCharacter(response);
		            }
	            }
	            else
	            {
	                std::cout << "Error: " << error.message() << std::endl;
	            }

	            startReceive();
	        }
	    );
	}

	void 
	Environment::setupSceneNodes()
	{
		sceneNodes_.insert({ "Light",  sceneManager_->getRootSceneNode()->createChildSceneNode() });
		sceneNodes_.insert({ "Camera", sceneManager_->getRootSceneNode()->createChildSceneNode() });
		sceneNodes_.insert({ "Entity", sceneManager_->getRootSceneNode()->createChildSceneNode() });
		sceneNodes_.insert({ "Ground", sceneManager_->getRootSceneNode()->createChildSceneNode() });
	}

	void 
	Environment::setupLights()
	{
		Ogre::Light* light { sceneManager_->createLight("MainLight") };
		light->setType(Ogre::Light::LT_DIRECTIONAL);

		sceneNodes_["Light"]->attachObject(light);
		sceneNodes_["Light"]->setDirection(Ogre::Vector3(1, -1, 0));
		sceneManager_->setAmbientLight(Ogre::ColourValue(0.1f, 0.1f, 0.1f));
	}

	void 
	Environment::setupCameras()
	{
		using namespace Epic::Entity::Movable;

		std::cout << "©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©" << std::endl;
		std::cout << "Camera for: " << id_ << std::endl;
		std::cout << "©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©©" << std::endl;

		this->getRenderWindow()->addViewport(
			characters_[id_]->getBackCamera()
		);
/*
		Ogre::ColourValue fadeColour(0.8, 0.8, 0.8);
		getRenderWindow()->getViewport(0)->setBackgroundColour(fadeColour);
		sceneManager_->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 2000, 10000);
*/
	}

	void
	Environment::setupGrounds()
	{
		using namespace Epic::Entity::Static;

		StaticObject::Builder* builder { new StaticObject::Builder(sceneManager_, physics_->get()) };
		builder
			->mesh("city.mesh", "GroundNode")
			->position(Ogre::Vector3(0, 0, 0))
			->body(Ogre::Bullet::CT_TRIMESH)
			->build()
		;
	}

	void
	Environment::setupEntities()
	{
		using namespace Epic::Entity::Movable;
		using namespace reflect;

		for(int i {0}; i < 4; i++)
		{
			std::string fileConfig("../media/config/character/vln.yaml");

			if(i == 1 || i == 2)
			{
				fileConfig = "../media/config/character/fire.yaml";
			}

			YAML::Node config { YAML::LoadFile(fileConfig) };

			Character::Builder* builder { new Character::Builder(sceneManager_, physics_->get()) };
			std::shared_ptr<Character> perso { std::shared_ptr<Character>(builder->fromConfig(config)) };

			if(i == 1)
			{
				perso->reflect("CHARGE", [](Character& character, bool loop) {
					auto animations { character.getAnimations() };
					auto animationKey { character.getAnimationKey() };

					character.stopAllAnimations();
					if(animations.find(animationKey.at("CHARGE")) != animations.end())
					{
						animations[animationKey.at("CHARGE")].state->setEnabled(true);
						animations[animationKey.at("CHARGE")].state->setLoop(loop);
					}
				});
			}

			perso->getSceneNode()->setVisible(false);

			players_.push_back(perso);
		}

		/*
		physics_->setDebugDrawer(std::make_unique<Ogre::Bullet::DebugDrawer>(
			perso0->getSceneNode(), 
			physics_->get()->getBtWorld())
		);
		*/
	}

	bool 
	Environment::frameRenderingQueued(const Ogre::FrameEvent& event)
	{
		float time = event.timeSinceLastFrame;

		btTransform transform;
		characters_[id_]->getRigidBody()->getMotionState()->getWorldTransform(transform);

		btVector3 pos = transform.getOrigin();
		btQuaternion rot = transform.getRotation();

		data["position"][id_]["x"] = pos.x();
		data["position"][id_]["y"] = pos.y();
		data["position"][id_]["z"] = pos.z();

		data["rotation"][id_]["w"] = rot.w();
		data["rotation"][id_]["x"] = rot.x();
		data["rotation"][id_]["y"] = rot.y();
		data["rotation"][id_]["z"] = rot.z();

		for(const auto character : characters_)
		{
			character.second->update(time);
		}

		net_->socket->async_send_to(
			boost::asio::buffer(data.dump()), 
			net_->server_endpoint,
			[this](const boost::system::error_code& error, std::size_t){}
		);
		
		data["action"] = nullptr;

		std::function<void ()> centerMouse = [this]() -> void {
			int width, height;
			size_t windowHandle = { 0 };
		    this->getRenderWindow()->getCustomAttribute("WINDOW", &windowHandle);
		    SDL_Window* sdlWindow = SDL_CreateWindowFrom((void*)windowHandle);

		    SDL_ShowCursor(SDL_DISABLE);
		    SDL_GetWindowSize(sdlWindow, &width, &height);
			SDL_WarpMouseInWindow(sdlWindow, width / 2, height / 2);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		};

		centerMouse();

		return true;
	}

	bool 
	Environment::keyPressed(const OgreBites::KeyboardEvent& evt) 
	{
		//characters_[id_]->onEvent(evt.keysym.sym);
		data["action"] = evt.keysym.sym;
		data["id"] = id_;

	    return true;
	}

	bool
	Environment::mousePressed(const OgreBites::MouseButtonEvent& evt)
	{
		switch(evt.button)
		{
		}

		return true;
	}

	bool 
	Environment::mouseMoved(const OgreBites::MouseMotionEvent& evt)
	{
		Ogre::Real sensitivity { 0.1f };
		Ogre::Real px { -evt.xrel * sensitivity };
		Ogre::Real py { -evt.yrel * sensitivity };

		data["act"][id_]["x"] = px;
		data["act"][id_]["y"] = py;

		return true;
	}

}
}
