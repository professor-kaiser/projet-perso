#include "Character.hpp"
#include <iostream>
#include <chrono>

namespace Epic
{
namespace Entity
{
namespace Movable
{
	Character::Character(Ogre::String mesh, Ogre::SceneManager* sceneManager)
    : sceneManager_(sceneManager),
      mesh_(sceneManager_->createEntity(mesh + std::to_string(std::chrono::system_clock::now().time_since_epoch().count()), mesh)),
      point_(sceneManager_->getRootSceneNode()->createChildSceneNode()),
      isRuning_(false),
      isJumping_(false),
      mass_(1.0f)
	{
		bounds_ = mesh_->getMesh()->getBounds();
		height_ = bounds_.getMaximum().y; - bounds_.getMinimum().y;

		point_->attachObject(mesh_);
		setupCamera();
		point_->setPosition(5000, 200, -2000);
/*
		reflect("getAnimations", [](Character& human) {
			return human.animations_;
		});

		reflect("getAnimationKey", [](Character& human) {
			return human.animsKey_;
		});
*/
		/*
		reflect("addAnimation", [](Character* human, std::tuple<int, Ogre::String, Ogre::String> animInfo) {
			human->animations_.insert({
				std::get<0>(animInfo),
				{ .name  = std::get<1>(animInfo),
				  .state = human->mesh_->getAnimationState(std::get<2>(animInfo))
				}
			});
		});
		*/

		/*
		Ogre::SkeletonInstance* skeleton { mesh_->getSkeleton() };
		Ogre::TagPoint* moTagPoint = mesh_->attachObjectToBone("mixamorig:RightHand", mo);

		moTagPoint->setScale(20.0f, 20.0f, 20.0f);
		for(unsigned short i { 0 }; i < skeleton->getNumBones(); i++)
		{
			Ogre::Bone* bone = skeleton->getBone(i);
			std::cout << "Bone [" << i << "] :" << bone->getName() << std::endl;
		}
		*/
	}

	void 
	Character::setupCamera()
	{
		auto now { std::chrono::system_clock::now().time_since_epoch().count() };
		Ogre::Vector3 max { bounds_.getMaximum() };
		Ogre::Vector3 min { bounds_.getMinimum() };
		Ogre::Vector3 offsetZf(0, 0, max.z - min.z);
		Ogre::Vector3 offsetY(0, height_ * 2, 0);
		Ogre::Vector3 offsetZ(0, 0, -height_ * 2);
		Ogre::Vector3 humanPosition { point_->_getDerivedPosition() };
		
		Ogre::Camera* backCamera { sceneManager_->createCamera("Back_" + std::to_string(now)) };
		backCamera->setNearClipDistance(5);
		backCamera->setAutoAspectRatio(true);

		Ogre::Camera* frontCamera { sceneManager_->createCamera("Front_" + std::to_string(now)) };
		frontCamera->setNearClipDistance(5);
		frontCamera->setAutoAspectRatio(true);
		
		cameras_ = std::tuple<Ogre::Camera*, Ogre::Camera*>(backCamera, frontCamera);
		cameraNodes_ = std::tuple<Ogre::SceneNode*, Ogre::SceneNode*> (
			point_->createChildSceneNode(), 
			point_->createChildSceneNode()
		);
		Ogre::SceneNode* backCameraNode  { std::get<0>(cameraNodes_) };
		Ogre::SceneNode* frontCameraNode { std::get<1>(cameraNodes_) };

		backCameraNode->attachObject(std::get<0>(cameras_));
		backCameraNode->roll(Ogre::Radian(Ogre::Math::PI));
    	backCameraNode->setPosition(humanPosition + offsetZ + offsetY);
    	//backCameraNode->setPosition(humanPosition + Ogre::Vector3(-height_ * 2, 0, 0) + offsetY);
		backCameraNode->lookAt(humanPosition, Ogre::Node::TS_WORLD);

		frontCameraNode->attachObject(std::get<1>(cameras_));
		frontCameraNode->roll(Ogre::Radian(Ogre::Math::PI));
    	frontCameraNode->setPosition(humanPosition + offsetZf + offsetY);
		frontCameraNode->lookAt(Ogre::Vector3(-humanPosition - offsetZ), Ogre::Node::TS_WORLD);
	}

	void 
	Character::rigidBody(btRigidBody* body)
	{
		Ogre::Bullet::RigidBodyState* bodyState { new Ogre::Bullet::RigidBodyState(point_) };
		bounds_ = mesh_->getMesh()->getBounds();
		Ogre::Vector3 min  { bounds_.getMinimum() };
		Ogre::Vector3 max  { bounds_.getMaximum() };
		Ogre::Vector3 size { max - min };
		btVector3 boxSize(size.x / 4.0f, size.y / 2.0f, size.z / 4.0f);
		btBoxShape* boxShape { new btBoxShape(boxSize) };
		btCompoundShape* compoundShape { new btCompoundShape() };
		btTransform shapeTransform;
		rigidBody_ = std::shared_ptr<btRigidBody>(body);

		shapeTransform.setIdentity();
		shapeTransform.setOrigin(btVector3(0, size.y / 2, 0));
		compoundShape->addChildShape(shapeTransform, boxShape);
		rigidBody_->setCollisionShape(compoundShape);
		rigidBody_->setMotionState(bodyState);
		rigidBody_->setAngularFactor(btVector3(0, 0, 0));
		rigidBody_->setFriction(1.0f);
		rigidBody_->setRestitution(1.0f);
		rigidBody_->setActivationState(DISABLE_DEACTIVATION);
		updatePhysics();
	}

	void
	Character::run(bool loop)
	{
		stopAllAnimations();
		if(animations_.find(animsKey_.at("RUN")) != animations_.end())
		{
			animations_[animsKey_.at("RUN")].state->setEnabled(true);
			animations_[animsKey_.at("RUN")].state->setLoop(loop);
		}

		isRuning_ = true;
	}

	void 
	Character::jump(bool loop)
	{
		if(!animations_[animsKey_.at("JUMP")].state->getEnabled())
		{
			rigidBody_->applyCentralImpulse(btVector3(0, /*mass_ * height_ * 10*/ 3 * mass_ * sqrt(2 * 9.81 * 82 * height_), 0));
			stopAllAnimations();
			if(animations_.find(animsKey_.at("JUMP")) != animations_.end())
			{
				animations_[animsKey_.at("JUMP")].state->setEnabled(true);
				animations_[animsKey_.at("JUMP")].state->setLoop(loop);
				animations_[animsKey_.at("JUMP")].state->setTimePosition(0.0f);
			}	

			isJumping_ = true;
			std::cout << "Jump begin..." << std::endl;
		}

		rigidBody_->setFriction(0.01f);
		if(animations_[animsKey_.at("JUMP")].state->hasEnded())
		{
			std::cout << "Jump end..." << std::endl;

			isJumping_ = false;
			if(this->isRuning_) this->run();
			else this->idle();

			rigidBody_->setFriction(1.0f);
		}
	}

	void
	Character::idle(bool loop)
	{
		if(animations_[animsKey_.at("JUMP")].state->getEnabled() && !animations_[animsKey_.at("JUMP")].state->hasEnded())
		{
			return;
		}

		stopAllAnimations();
		if(animations_.find(animsKey_.at("IDLE")) != animations_.end())
		{
			animations_[animsKey_.at("IDLE")].state->setEnabled(true);
			animations_[animsKey_.at("IDLE")].state->setLoop(loop);
		}

		this->stop();
	}

	bool
	Character::allAnimationsEnded()
	{
		for(auto animation : animations_)
			if(animation.second.state->getEnabled())
				return false;

		return true;
	}

	void
	Character::stopAllAnimations()
	{
		for(auto animation : animations_)
		{
			if(animation.second.state->getEnabled())
			{
				animation.second.state->setEnabled(false);
				animation.second.state->setLoop(false);
			}
		}
	}

	void
	Character::move(Ogre::Vector3 position)
	{
		/*
		point_->translate(position, Ogre::Node::TS_LOCAL);
		updatePhysics();
		*/
		Ogre::Vector3 globalDir = point_->_getDerivedOrientation() * position.normalisedCopy();

	    btVector3 velocity(globalDir.x * speed_, rigidBody_->getLinearVelocity().y(), globalDir.z * speed_);
	    rigidBody_->setLinearVelocity(velocity);
	}

	void
	Character::stop()
	{
		btVector3 currentVelocity = rigidBody_->getLinearVelocity();
    	rigidBody_->setLinearVelocity(btVector3(0, currentVelocity.y(), 0));
    	isRuning_ = false;
	}

	void 
	Character::updatePhysics()
	{
	    btTransform transform;
	    rigidBody_->getMotionState()->getWorldTransform(transform);

	    btVector3 pos = transform.getOrigin();
	    btQuaternion rot = transform.getRotation();
	    Ogre::Vector3 position(pos.x(), pos.y(), pos.z());
	    Ogre::Quaternion orientation(rot.w(), rot.x(), rot.y(), rot.z());

	    point_->setPosition(position);
	    point_->setOrientation(orientation);

	    rigidBody_->activate(true);
	}

	void 
	Character::act(Ogre::Degree xd, Ogre::Degree yd)
	{
	    rigidBody_->setAngularVelocity(btVector3(0, xd.valueDegrees(), 0));
	    
	    std::get<0>(cameraNodes_)->pitch(yd);
	    std::get<1>(cameraNodes_)->pitch(yd);
	}

	Ogre::Camera* 
	Character::getBackCamera()
	{
		return std::get<0>(cameras_);
	}

	Ogre::Camera* 
	Character::getFrontCamera()
	{
		return std::get<1>(cameras_);
	}

	Ogre::SceneNode* 
	Character::getBackCameraSceneNode()
	{
		return std::get<0>(cameraNodes_);
	}

	Ogre::SceneNode* 
	Character::getFrontCameraSceneNode()
	{
		return std::get<1>(cameraNodes_);
	}

	Ogre::Entity*
	Character::get()
	{
		return mesh_;
	}

	btRigidBody*
	Character::getRigidBody()
	{
		return rigidBody_.get();
	}

	Ogre::Vector3
	Character::getPosition()
	{
		return point_->_getDerivedPosition();
	}

	Ogre::SceneNode* 
	Character::getSceneNode()
	{
		return point_;
	}

	void 
	Character::update(float deltatime)
	{
		updatePhysics();
		if(this->isRuning_)
		{
			this->move(Ogre::Vector3(0, 0, speed_ * deltatime));
		}
		if(this->isJumping_)
		{
			this->jump();
		}

		for(auto animation : this->animations_)
		{
			if(animation.second.state->getEnabled())
			{
				animation.second.state->addTime(deltatime);
			}
		}
	}

	void 
	Character::onEvent(const int event)
	{
		std::string method { 
			(animations_.find(event) != animations_.end()) ? 
			animations_.at(event).name : std::string("") 
		};
		if(this->exists<Character>(method))
		{
			this->invoke<void>(*this, animations_[event].name, animations_[event].loop);

			std::cout << "name: " << animations_[event].name << " => " << animations_[event].loop << std::endl;
		}
	}
}
}
}