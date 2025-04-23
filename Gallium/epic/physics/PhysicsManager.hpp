#ifndef __PHYSICS_MANAGER__
#define __PHYSICS_MANAGER__

#include <Bullet/OgreBullet.h>
#include <memory>
#include <Ogre.h>

namespace world
{
namespace physics
{
	class PhysicsManager
	{
	private:
		std::unique_ptr<Ogre::Bullet::DynamicsWorld> dynamicsWorld_;
		std::unique_ptr<Ogre::Bullet::DebugDrawer> debug_;

	public:
		PhysicsManager(Ogre::Vector3&& gravity = Ogre::Vector3(0.0, -9.81 * 82, 0.0));
		Ogre::Bullet::DynamicsWorld* get();
		Ogre::Bullet::DebugDrawer* getDebugDrawer()
		{
			return debug_.get();
		}

		void setDebugDrawer(std::unique_ptr<Ogre::Bullet::DebugDrawer> debug)
		{
			debug_ = std::move(debug);
		}

		void update(float deltatime);
	};
} // namespace physics
} // namespace world

#endif