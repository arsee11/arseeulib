
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
	public Unserializer getUnserializer(){ return new JUnserializer(); }
	
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
				strbuf.append("\"params\":[{");
				int count=0;
				for(HashMap<String, Object> i:getParamTable()){
					if( i.size() == 0 )
						break;
						
					Iterator j = i.entrySet().iterator();
					if( j.hasNext() ){
						if( count == 0)
							strbuf.append("\"param"+count+"\":[");
						else
							strbuf.append(",\"param"+count+"\":[");
							
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
				
			strbuf.append("}]");
			strbuf.append("}");		
	
			System.out.println(strbuf.toString());
			return strbuf.toString();
		}
	}

	class JUnserializer extends Unserializer{
		@Override
		public boolean parseBody(byte[] buf, int offset, int len){
			String str = new String(buf, offset, len);
			System.out.println(buf[9]);
			System.out.println(str);
			Pack pck = new JPack(); 
			try{
				JSONObject jb = new JSONObject(str);
				src = jb.getString("source");
				trgt = jb.getString("target");
				act = jb.getString("action");
						
				JSONArray ja = jb.getJSONArray("params");
				Pack.ParamTable pt = new Pack.ParamTable();
				for(int i=0; i<ja.length(); i++){
					JSONArray ja2 = ja.getJSONObject(i).getJSONArray("param"+i);
					HashMap<String, Object> p = new HashMap<String, Object>();
					for(int j=0; j<ja2.length(); j++){
						JSONObject param = ja2.getJSONObject(j);
						p.put(param.getString("name"), param.getString("value"));					
					}
					
					pt.add(p);
				}
				paramTable = pt;
			}catch(JSONException e){
				setStatus(false);
				System.out.println(e.toString());

				return false;
			}
			
			setStatus(true);
			return true;
		}
	}
	
}
	
