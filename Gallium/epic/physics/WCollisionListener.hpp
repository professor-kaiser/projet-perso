#ifndef __WCOLLISION_LISTENER__
#define __WCOLLISION_LISTENER__

#include <Bullet/OgreBullet.h>
#include <Ogre.h>

namespace Epic
{
namespace Physics
{
	class WCollisionListener : public Ogre::Bullet::CollisionListener
	{
	public:
		void contact(const Ogre::MovableObject* other, const btManifoldPoint& manifoldPoint) override
		{
			const Ogre::String& objectName = other->getName();
			Ogre::Vector3 contactPoint = Ogre::Bullet::convert(manifoldPoint.getPositionWorldOnB());

			//if(objectName != Ogre::String("Ogre/MO2"))
				//Ogre::LogManager::getSingleton().logMessage("Collision avec " + objectName + " au point " + Ogre::StringConverter::toString(contactPoint));
		}
	};
}
}

#endif