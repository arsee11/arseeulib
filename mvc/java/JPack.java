
//JPack.java

package mylib.mvc.java;

import org.json.*;
import java.util.*;

public class JPack extends Pack{
	
	public JPack(){ super(); }
	public JPack( String src, String trgt, String act ){
		super(src, trgt, act);
	}

	@Override
	public Serializer getSerializer(){ return new JSerializer(); }
	
	@Override
	public UnSerializer getUnSerializer(){ return new JUnSerializer(); }
	
	class JSerializer extends Serializer{
	
		@Override
		public String buildBody(){
			StringBuffer strbuf = new StringBuffer();
			strbuf.append("{");
			strbuf.append("\"action\":").append("\"").append(getAction()).append("\",");
			strbuf.append("\"source\":").append("\"").append(getSource()).append("\",");
			strbuf.append("\"target\":").append("\"").append(getTarget()).append("\",");
			
			if( getParamTable().size() > 0 )
			{
				strbuf.append("\"params\":[");
				int count=0;
				for(HashMap<String, Object> i:getParamTable()){
					if( i.size() == 0 )
						break;
						
					Iterator j = i.entrySet().iterator();
					if( j.hasNext() ){
						if( count == 0)
							strbuf.append("param:[");
						else
							strbuf.append(",param:[");
							
						Map.Entry e = (Map.Entry)j.next();
						strbuf.append("{\"name\":" ).append("\"").append(e.getKey()  ).append("\",");
						strbuf.append("\"value\":").append("\"").append(e.getValue()).append("\"}");
					}
					
					while(j.hasNext()){
						Map.Entry e = (Map.Entry)j.next();
						strbuf.append(",{\"name\":" ).append("\"").append(e.getKey()  ).append("\",");
						strbuf.append("\"value\":").append("\"").append(e.getValue()).append("\"}");
					}
					count++;
					strbuf.append("]");
				}
			}
				
			strbuf.append("]");
			strbuf.append("}");		
	
			System.out.println(strbuf.toString());
			return strbuf.toString();
		}
	}

	class JUnSerializer extends UnSerializer{
		@Override
		public boolean parseBody(byte[] buf, int offset, int len){
			String str = new String(buf, offset, len);
			Pack pck = new JPack(); 
			try{
				JSONObject jb = new JSONObject(str.toString());
				pck = new JPack(
					jb.getString("source"),
					jb.getString("target"),
					jb.getString("action")
				);
						
				JSONArray ja = jb.getJSONArray("params");
				Pack.ParamTable pt = new Pack.ParamTable();
				for(int i=0; i<ja.length(); i++){
					JSONArray ja2 = ja.getJSONObject(i).getJSONArray("param");
					HashMap<String, Object> p = new HashMap<String, Object>();
					for(int j=0; j<ja2.length(); j++){
						JSONObject param = ja2.getJSONObject(j);
						p.put(param.getString("name"), param.getString("value"));					
					}
					
					pt.add(p);
				}
			}catch(JSONException e){
				pck.setStatus(false);
				return false;
			}
			
			pck.setStatus(true);
			return true;
		}
	}
	
}
	
