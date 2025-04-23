#ifndef __PHYSICS_FRAME_LISTENER__
#define __PHYSICS_FRAME_LISTENER__

#include <Ogre.h>
#include "PhysicsManager.hpp"

namespace world
{
namespace physics
{
	class PhysicsFrameListener : public Ogre::FrameListener
	{
	private:
		PhysicsManager* physics_;

	public:
		PhysicsFrameListener(PhysicsManager* physics) : physics_(physics) {}

		bool frameStarted(const Ogre::FrameEvent& evt) override
		{
			physics_->update(evt.timeSinceLastFrame);
			return true;
		}
	};
}
}

#endif