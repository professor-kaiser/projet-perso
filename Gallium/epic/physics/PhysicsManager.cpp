#include "PhysicsManager.hpp"

namespace world
{
namespace physics
{
	PhysicsManager::PhysicsManager(Ogre::Vector3&& gravity)
	{
		dynamicsWorld_ = std::make_unique<Ogre::Bullet::DynamicsWorld>(gravity);
	}

	Ogre::Bullet::DynamicsWorld* 
	PhysicsManager::get()
	{
		return dynamicsWorld_.get();
	}

	void 
	PhysicsManager::update(float deltatime)
	{
		//dynamicsWorld_->getBtWorld()->stepSimulation(std::min(deltatime, 1.0f / 60.0f), 10);
		dynamicsWorld_->getBtWorld()->stepSimulation(deltatime, 10);
		if(debug_) debug_->update();
	}

} // namespace physics
} // namespace world