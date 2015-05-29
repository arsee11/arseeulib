//Pack.java

public class Pack{

	static const byte[] HEAD = {0xFF, 0xFF, 0xFF, 0xFF};
	
	public class ParamTable:extends ArrayList<HashMap<String, Object>>{
		public void AddColumn(String name, Object value){
		}
		
		public void AddItem(){
		}
	}
	
	public abstract class Serialzer{
		public byte[] solve(Pack pck){
			StringBuffer strbuf = new StringBuffer();			
			String str = buildBody(pck);
			int len = str.length();
			byte[] buf = new byte[8+len];
			System.arraycopy(HEAD, 0, buf, 0, 4);
			buf[4] = (byte)(len&0xFF); 	   	 //1st byte;
			buf[5] = (byte)((len>>8 )&&0xFF) //2nd byte;
			buf[6] = (byte)((len>>16)&&0xFF) //3nd byte;
			buf[7] = (byte)((len>>24)&&0xFF) //4nd byte;
			
			System.arraycopy(str.getBytes(), 0, buf, 8, len);
			return buf;
		}
		
		abstract String buildBody(Pack pck);
	}
	
	public Pack( String src, String trgt, String act ){
		this.src = src;
		this.trgt = trgt;
		this.act = act;
	}
	
	public String getSource(){ return src;  }
	public String getTarget(){ return trgt; }
	public String getAction(){ return act;  }
	
	public void  setParamTable(ParamTable table){ paramTable = table;  }
	public ParamTable getParamTable(String name){ return pramTable;    }

	
	String src;
	String trgt;
	String act;
	ParamTable paramTable;
}
