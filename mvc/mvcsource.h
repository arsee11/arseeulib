//file: mvcrsource.h

#ifndef MVC_SOURCE_H
#define MVC_SOURCE_H

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

#endif /*MVC_SOURCE_H*/
