//file: jpack.h
//copyright	: Copyright (c) 2014 arsee.
//license	: GNU GPL v2.
//author	: arsee

//****************************
//modify	
//data	: 2014-12-10
//log	: modify 
//****************************

#ifndef JPACK_H
#define JPACK_H

#include <utility>


#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

#ifndef PACK_H
#include "pack_2.h"
#endif



NAMESP_BEGIN

//Json pack defines
//{"source":"src","target":"t","action":"response", "params":[{"name":"name1","value":"value1"},{"name":"name2","value":"value2"}]}
class JUnSerializer;

/////////////////////////////////////////////////////////
//JSerializer
class JSerializer:
	public SerializerAbstr<JSerializer, JUnSerializer>
{
private:
	stream_t Resolve(const pack_t &pck)override;	
	stream_t Resolve(const pack_ptr_t &pck)override;	
	size_t Header()override;
};

////////////////////////////////////////////////////////
//JUnSerializer
class JUnSerializer:
	public UnSerializerAbstr<JSerializer, JUnSerializer>
{
public:
	JUnSerializer(size_t buf_len)
		:UnSerializerAbstr<JSerializer, JUnSerializer>(buf_len)
	{
	}

	int Parse(pack_t &pck, stream_t &stream)override;	
	int Parse(pack_ptr_t &pck, stream_t &stream)override;
	bool Header(BufferWrapper& buf, size_t *head_len);
};

typedef Pack<JSerializer, JUnSerializer> Jpack;

NAMESP_END

#endif /*JPACK_H*/
