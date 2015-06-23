//Pack.java

package mylib.mvc.java;

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
		public void solve(byte[] buf) throws Exception{
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
					return;
				}
			}catch(Exception e){
				throw e;
			}

			if( !parseBody(buf, 8, payloadLen) )
				throw new Exception("payload invalid");

		}

		public abstract boolean parseBody(byte[] buf, int offset, int len);
	}

	public Pack(){ this.status = false; }
	public Pack( String src, String trgt, String act ){
		this.src = src;
		this.trgt = trgt;
		this.act = act;
	}

	public void setSource(String value){ src = value; }
	public void setTarget(String value){ trgt= value; }
	public void setAction(String value){ act = value; }
	
	public String getSource(){ return src;  }
	public String getTarget(){ return trgt; }
	public String getAction(){ return act;  }
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
	
	protected String src;
	protected String trgt;
	protected String act;
	protected boolean status;
	protected ParamTable paramTable =  new ParamTable();
}
