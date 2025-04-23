#ifndef __EVENT_OBSERVER__
#define __EVENT_OBSERVER__

namespace Epic
{
namespace Event
{
	class EventObserver
	{
	public:
		virtual ~EventObserver() = default;
		virtual void onEvent(const int) = 0;
	};
}
}

#endif