//file: rsource.h

#ifndef SOURCE_H
#define SOURCE_H

#ifndef GLOBALDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN



template<class OBJ, class R>
class Operator
{
public:
	virtual bool operator()(OBJ *obj, R &r);

};


NAMESP_END;

#endif /*SOURCE_H*/
