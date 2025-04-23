#ifndef __BASE_ENTITY__
#define __BASE_ENTITY__

#include <memory>
#include "EventObserver.hpp"

namespace Epic
{
namespace Entity
{
namespace Movable
{
	class BaseCharacter : public Epic::Event::EventObserver
	{
	public:
		virtual void onEvent(const int) = 0;
		virtual void update(float deltatime) = 0;
		virtual ~BaseCharacter() = default;
	};
}
} // entity
} // epic

#endif