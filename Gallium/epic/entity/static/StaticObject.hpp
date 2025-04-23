#ifndef __STATIC_OBJECT__
#define __STATIC_OBJECT__

#include <Ogre.h>
#include <Bullet/OgreBullet.h>
#include <memory>

namespace Epic
{
namespace Entity
{
namespace Static
{
	class StaticObject
	{
	private:
		Ogre::SceneManager* sceneManager;
		Ogre::Entity* entity;
		Ogre::SceneNode* sceneNode;
		Ogre::String name;

	public:
		StaticObject() = default;

		class Builder
		{
		private:
			StaticObject* object;
			std::shared_ptr<Ogre::Bullet::DynamicsWorld> dynamicsWorld;

		public:
			Builder(Ogre::SceneManager* sceneManager, Ogre::Bullet::DynamicsWorld* dw)
			{
				object = new StaticObject();
				object->sceneManager = sceneManager;
				dynamicsWorld = std::shared_ptr<Ogre::Bullet::DynamicsWorld>(dw);
			}

			Builder* mesh(Ogre::String name, Ogre::String id)
			{
				object->entity = object->sceneManager->createEntity(id, name);
				return this;
			}

			Builder* position(Ogre::Vector3 objectPosition)
			{
				object->sceneNode = object->sceneManager->getRootSceneNode()->createChildSceneNode();
				object->sceneNode->attachObject(object->entity);
				object->sceneNode->setPosition(objectPosition);
				//object->sceneNode->setScale(1000, 1000, 1000);
				return this;
			}

			Builder* body(Ogre::Bullet::ColliderType collider)
			{
				dynamicsWorld->addRigidBody(0, object->entity, collider);
				return this;
			}

			StaticObject* build()
			{
				return object;
			}
		};
	};
} // Static
} // Entity
} // Epic

#endif