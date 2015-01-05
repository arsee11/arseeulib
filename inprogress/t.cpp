
#include "queuebuf.h"

int main()
{
	QueueBuffer<CharPtrWrapper, mutex_guard_t> buf(10);
	return 0;
}
