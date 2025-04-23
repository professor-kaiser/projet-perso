#ifndef __HUMAN__
#define __HUMAN__

#include <Ogre.h>
#include <Bites/OgreInput.h>
#include <tuple>
#include <Bullet/OgreBullet.h>
#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>
#include "BaseCharacter.hpp"
#include "Reflectable.hpp"
#include "WCollisionListener.hpp"
#include "Character.hpp"

namespace Epic
{
namespace Entity
{
namespace Movable
{
	typedef struct Animation
	{
		Ogre::String name;
		Ogre::AnimationState* state;
		bool loop;
	} Animation;

	class Character: public BaseCharacter, public reflect::Reflectable
	{
	private:
		Ogre::SceneManager* sceneManager_;
		std::map<int, Animation> animations_;
		std::map<Ogre::String, int> animsKey_;
		Ogre::Entity* mesh_;
		Ogre::SceneNode* point_;
		std::tuple<Ogre::Camera*, Ogre::Camera*> cameras_;
		std::tuple<Ogre::SceneNode*, Ogre::SceneNode*> cameraNodes_;
		std::string hello;

		std::shared_ptr<btRigidBody> rigidBody_;
		Ogre::Vector3 position_;
		Ogre::AxisAlignedBox bounds_;
		float speed_;
		float height_;
		float mass_;
		bool isRuning_;
		bool isJumping_;

		//Network* network_;
		
		friend class Builder;
		friend class reflect::Reflectable;

	public:
		Character(Ogre::String mesh, Ogre::SceneManager* sceneManager);

		/*--------------------------------------------------------------*
		 * Animations
		 *--------------------------------------------------------------*/
		void animations(std::map<Ogre::String, Ogre::String>);
		void run(bool loop = true);
		//void therorized_run(bool loop = true);
		void idle(bool loop = true);
		void jump(bool loop = true);
		bool allAnimationsEnded();
		void stopAllAnimations();
		/*--------------------------------------------------------------*/

		/*--------------------------------------------------------------*
		 * Physics
		 *--------------------------------------------------------------*/
		void move(Ogre::Vector3 position);
		void stop();
		void rigidBody(btRigidBody* body);
		void updatePhysics();
		/*--------------------------------------------------------------*/

		/*--------------------------------------------------------------*
		 * Camera
		 *--------------------------------------------------------------*/
		void setupCamera();
		void act(Ogre::Degree xd, Ogre::Degree yd);
		Ogre::Camera*    getBackCamera();
		Ogre::Camera*    getFrontCamera();
		Ogre::SceneNode* getBackCameraSceneNode();
		Ogre::SceneNode* getFrontCameraSceneNode();
		/*--------------------------------------------------------------*/

		Ogre::Entity*    get();
		btRigidBody*     getRigidBody();
		Ogre::Vector3    getPosition();
		Ogre::SceneNode* getSceneNode();

		float height()
		{
			return height_;
		}

		void setRuning(bool run)
		{
			isRuning_ = run;
		}

		bool getRuning()
		{ 
			return isRuning_; 
		}

		void setJumping(bool jump)
		{
			isJumping_ = jump;
		}

		bool getJumping()
		{
			return isJumping_;
		}

		auto getAnimations()
		{
			return animations_;
		}

		auto getAnimationKey()
		{
			return animsKey_;
		}

		void update(float deltatime) override;
		void onEvent(const int) override;

		bool isRuning()
		{
			return isRuning_;
		}

		class Builder
		{
		private:
			Character *human;
			Ogre::SceneManager* sceneManager_;
			std::map<Ogre::String, Ogre::AnimationState*> animations_;
			Ogre::Entity* mesh_;
			std::unique_ptr<btRigidBody> rigidBody_;
			Ogre::Degree angle_;
			std::shared_ptr<Ogre::Bullet::DynamicsWorld> dynamicsWorld;

		public:
			Builder(Ogre::SceneManager* scene, Ogre::Bullet::DynamicsWorld* dw) 
			: sceneManager_(scene),
			  dynamicsWorld(std::shared_ptr<Ogre::Bullet::DynamicsWorld>(dw)) 
			{}

			Builder* mesh(Ogre::String name)
			{
				human = new Character(name, sceneManager_);
				return this;
			}

			Builder* anims(std::map<int, std::tuple<Ogre::String, Ogre::String, bool>> anims)
			{
				for(auto animation : anims)
				{
					human->animations_.insert({
						animation.first,
						{ .name  =  std::get<0>(animation.second)/*animation.second.first*/, 
						  .state = human->mesh_->getAnimationState(std::get<1>(animation.second)),
						  .loop  = std::get<2>(animation.second)
						}
					});
					human->animsKey_.insert({
						std::get<0>(animation.second),
						animation.first
					});
				}

				return this;
			}

			Builder* scale(float sc)
			{
				human->point_->setScale(Ogre::Vector3(sc, sc, sc));
				return this;
			}

			Builder* mass(float m)
			{
				human->mass_ = m;
				return this;
			}

			Builder* position(Ogre::Vector3 position)
			{
				human->position_ = position;
				human->point_->setPosition(position);
				return this;
			}

			Builder* speed(float sp)
			{
				human->speed_ = sp;
				return this;
			}

			Builder* rigidBody(btRigidBody* body)
			{
				human->rigidBody(body);
				return this;
			}

			Character* build()
			{
				return human;
			}

			Character* fromConfig(YAML::Node node) 
			{
				YAML::Node properties { node["properties"] };
				Ogre::String mesh(properties["mesh"].as<std::string>());
				float speed { properties["speed"].as<float>() };
				float scale { properties["scale"].as<float>() };
				float mass  { properties["mass"].as<float>()  };
				Ogre::Vector3 position (
					properties["position"]["x"].as<float>(), 
					properties["position"]["y"].as<float>(), 
					properties["position"]["z"].as<float>()
				);
				std::map<int, std::tuple<Ogre::String, Ogre::String, bool>> anims;

				for(auto animation : properties["animations"])
				{
					anims.insert({
						animation["code"].as<char>(),
						std::make_tuple (
							Ogre::String(animation["action"].as<std::string>()),
							Ogre::String(animation["name"].as<std::string>()),
							animation["loop"].as<bool>()
						)
					});
				}

				Character* human = this
					->mesh(mesh)
					->speed(speed)
					->scale(scale)
					->mass(mass)
					->anims(anims)
					->position(position)
					->build();

				human->reflect("RUN", &Character::run);
				human->reflect("IDLE", &Character::idle);
				human->reflect("JUMP", &Character::jump);

				Epic::Physics::WCollisionListener* listener { new Epic::Physics::WCollisionListener() };
				human->rigidBody(dynamicsWorld->addRigidBody(40.0f, human->get(), Ogre::Bullet::CT_BOX, listener));
				human->idle();

				return human;
			}

			Character* getCharacter()
			{
				return this->human;
			}

			void setCharacter(Character* h)
			{
				this->human = h;
			}
		};
	};
}
} // namespace entity
} // namespace world

#endif