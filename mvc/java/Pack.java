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
	
	public static int bytes2Int(byte[] bytes, int offset){
		int value = 0;
		value = value + bytes[offset+0]&&0xFF);	
		value = value + ((int)bytes[offset+1]&0xFF)<<8;	
		value = value + ((int)bytes[offset+2]&0xFF)<<16;	
		value = value + ((int)bytes[offset+3]&0xFF)<<24;
		
		return value;
	}

	public static void int2Bytes(int nvalue, byte[] bytes, int offset){
		bytes[offset+0] = (byte)( nvalue     &0xFF); 	   	 //1st byte;
		bytes[offset+1] = (byte)((nvalue>>8 )&0xFF); //2nd byte;
		bytes[offset+2] = (byte)((nvalue>>16)&0xFF); //3nd byte;
		bytes[offset+3] = (byte)((nvalue>>24)&0xFF); //4nd byte;
	}

	public abstract class Serializer{
		public byte[] solve(){
			StringBuffer strbuf = new StringBuffer();			
			String str = buildBody();
			int len = str.length();
			byte[] buf = new byte[8+len];
			System.arraycopy(HEAD, 0, buf, 0, 4);
			buf[4] = (byte)(len&0xFF); 	   	 //1st byte;
			buf[5] = (byte)((len>>8 )&0xFF); //2nd byte;
			buf[6] = (byte)((len>>16)&0xFF); //3nd byte;
			buf[7] = (byte)((len>>24)&0xFF); //4nd byte;
			
			System.arraycopy(str.getBytes(), 0, buf, 8, len);
			return buf;
		}
		
		abstract String buildBody();
	}
	
	public abstract class UnSerializer{
		public void solve(byte[] buf) throws Exception{
			if( buf.length < 8 )
				throw new Exception("buf to small");

			if( bytes2Int(buf, 0) != bytes2Int(HEAD, 0) )
				throw new Exception("head failed");

			int payloadLen = bytes2Int(buf, 4);
			if( buf.length-8 < payloadLen )
			{
				setStatus(false);
				return;
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
	
	public String getSource(){ return src;  }
	public String getTarget(){ return trgt; }
	public String getAction(){ return act;  }
	public boolean getStatus(){ return status;  }
	public void setStatus(boolean value ){ status=value;  }
	
	public void  setParamTable(ParamTable table){ paramTable = table;  }
	public Pack.ParamTable getParamTable(){ return paramTable;    }

	public abstract Serializer getSerializer();
	public abstract UnSerializer getUnSerializer();
	
	String src;
	String trgt;
	String act;
	boolean status;
	ParamTable paramTable =  new ParamTable();
}
