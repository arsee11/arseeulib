//Pack.java

package mylib.rqh.java;

import java.util.ArrayList;
import java.util.HashMap;
import java.lang.Exception;

public abstract class Pack{

	public static final byte[] HEAD = {(byte)0xFF, (byte)0xFF, (byte)0xFF, (byte)0xFF};
	
	public static class ParamTable extends ArrayList<HashMap<String, Object>>{
		public void AddColumn(String name, Object value){
		}
		
		public void AddItem(){
		}
	}
	
		
	public abstract class Serializer{
		public byte[] solve(){
			StringBuffer strbuf = new StringBuffer();			
			String str = buildBody();
			int len = str.length();
			byte[] buf = new byte[8+len];
			System.arraycopy(HEAD, 0, buf, 0, 4);
			try{
				Util.int2Bytes(len, buf, 4);	
			}catch(Exception e){
				return null;
			}

			System.arraycopy(str.getBytes(), 0, buf, 8, len);
			return buf;
		}
		
		abstract String buildBody();
	}
	
	public abstract class Unserializer{
		public int solve(byte[] buf) throws Exception{
			if( buf.length < 8 )
				throw new Exception("buf to small");

			int payloadLen=0; 
			try{
				if( Util.bytes2Int(buf, 0) != Util.bytes2Int(HEAD, 0) )
					throw new Exception("head failed");

				payloadLen = Util.bytes2Int(buf, 4);
				if( buf.length-8 < payloadLen )
				{
					setStatus(false);
					return 0;
				}
			}catch(Exception e){
				throw e;
			}

			if( !parseBody(buf, 8, payloadLen) )
				throw new Exception("payload invalid");
				
			return buf.length - (payloadLen+8);

		}

		public abstract boolean parseBody(byte[] buf, int offset, int len);
	}

	public Pack(){ this.status = false; }
	public Pack(String type, String act, String paramType, String paramEncoding ){
		this.type = type;
		this.paramt = paramType;
		this.parame = paramEncoding;
		this.act = act;
	}

	public void setSource		(String value){ src = value; }
	public void setTarget		(String value){ trgt= value; }
	public void setAction		(String value){ act = value; }
	public void setType			(String value){ type = value; }
	public void setContinue		(String value){ cont = value; }	
	public void setParamType	(String value){ paramt = value; }	
	public void setParamEncoding(String value){ parame = value; }	
	
	public String getSource()		{ return src;  }
	public String getTarget()		{ return trgt; }
	public String getAction()		{ return act;  }
	public String getType()			{ return type; }
	public String getContinue()		{ return cont; }
	public String getParamType()	{ return paramt; }
	public String getParamEncoding(){ return parame; }
	public Object getParam(int i, String name)
	{
		if( paramTable.size() > i )
			return paramTable.get(i).get(name);
			
		return new String();
	}
	
	public boolean getStatus(){ return status;  }
	public void setStatus(boolean value ){ status=value;  }
	
	public void  setParamTable(ParamTable table){ paramTable = table;  }
	public Pack.ParamTable getParamTable(){ return paramTable;    }

	public abstract Serializer getSerializer();
	public abstract Unserializer getUnserializer();
	
	protected boolean status = false;	
	protected String type="request";
	protected String cont="end";
	protected String src;
	protected String trgt;
	protected String act;
	protected String paramt="text";
	protected String parame="plain";	
	protected ParamTable paramTable =  new ParamTable();
}
