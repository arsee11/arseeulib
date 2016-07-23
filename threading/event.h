//event.h

class Event
{
public:
	Event();
	~Event();

	int Wait(uint32_t msecondes);
	void Notify();
};
