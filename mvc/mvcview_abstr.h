//file: mvcview_abstr.h

#ifndef MVCVIEW_ABSTR_H
#define MVCVIEW_ABSTR_H

#ifndef GLOBALDEF_H
#include "globaldef.h"
#endif

NAMESP_BEGIN

class IView
{
public:
	virtual void Output()=0;
	virtual ~IView(){}
};

NAMESP_END;

#endif /*MVCVIEW_ABSTR_H*/