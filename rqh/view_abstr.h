//file: view_abstr.h

#ifndef VIEW_ABSTR_H
#define VIEW_ABSTR_H


#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN

class IView
{
public:
	virtual void Output()=0;
	virtual ~IView(){}
};

NAMESP_END;

#endif /*VIEW_ABSTR_H*/
