//Pack.java

package mylib.mvc.java;

import java.util.ArrayList;
import java.util.HashMap;

public abstract class Pack{

	static final byte[] HEAD = {(byte)0xFF, (byte)0xFF, (byte)0xFF, (byte)0xFF};
	
	public static class ParamTable extends ArrayList<HashMap<String, Object>>{
		public void AddColumn(String name, Object value){
		}
		
		public void AddItem(){
		}
	}
	
	public static int bytes2Int(byte[] bytes, int offset)
	{
		if( bytes.length-offset < 4 )
			return 0;
			
		int value=0;
		value = value & ((int)buf[0+offset]);
		value = value & ((int)buf[1+offset])<<8;
		value = value & ((int)buf[2+offset])<<16;
		value = value & ((int)buf[3+offset])<<24;
	
		return value;
	}
	
	public static void int2Bytes(int value, byte[] bytes, int offset)
	{
		if( bytes != null && bytes.length-offset>4)
		{
			bytes[offset+0] = (byte)(value&0xFF); 	   	 //1st byte;
			bytes[offset+1] = (byte)((value>>8 )&0xFF); //2nd byte;
			bytes[offset+2] = (byte)((value>>16)&0xFF); //3nd byte;
			bytes[offset+3] = (byte)((value>>24)&0xFF); //4nd byte;
		}
	}
	
	public abstract class Serializer{
		public byte[] solve(){
			StringBuffer strbuf = new StringBuffer();			
			String str = buildBody();
			int len = str.length();
			byte[] buf = new byte[8+len];
			System.arraycopy(HEAD, 0, buf, 0, 4);
			int2Bytes(len, buf, 4);
			
			System.arraycopy(str.getBytes(), 0, buf, 8, len);
			return buf;
		}
		
		abstract String buildBody();
	}
	
	public abstract class UnSerializer{
		public boolean solve(byte[] buf){
			for(int i=0; i<4; i++){
				if(buf[i] != (byte)0xFF)
					return false;
			}
					
			int len = bytes2Int(buf, 4);
			return parseBody(buf, 8, len);
		}
		
		abstract boolean parseBody(byte[] buf, int offset, int len);
	}
	
	public Pack( String src, String trgt, String act ){
		this.src = src;
		this.trgt = trgt;
		this.act = act;
	}
	
	public abstract Serializer getSerializer();
	public abstract UnSerializer getUnSerializer();
	
	public String getSource(){ return src;  }
	public String getTarget(){ return trgt; }
	public String getAction(){ return act;  }
	
	public void  setParamTable(ParamTable table){ paramTable = table;  }
	public Pack.ParamTable getParamTable(){ return paramTable;    }

	
	String src;
	String trgt;
	String act;
	ParamTable paramTable =  new ParamTable();
}
