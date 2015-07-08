
//JPack.java

package mylib.rqh.java;

import org.json.*;
import java.util.*;

public class JPack extends Pack{
	
	public JPack(){ super(); }
	public JPack( String type, String act, String paramType, String paramEncoding ){
		super(type, act, paramType, paramEncoding);
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
			strbuf.append("\"type\":"    		).append("\"").append(getType()    		).append("\",");
			if( getContinue().length() > 0 )
				strbuf.append("\"continue\":"	).append("\"").append(getContinue()		).append("\",");
				
			strbuf.append("\"action\":"  		).append("\"").append(getAction()		).append("\",");
			if( getSource().length() > 0 )
				strbuf.append("\"source\":"  	).append("\"").append(getSource()		).append("\",");
				
			if( getTarget().length() > 0 )
				strbuf.append("\"target\":"     ).append("\"").append(getTarget()   	).append("\",");
			
			strbuf.append("\"paramType\":"  	).append("\"").append(getParamType()	).append("\",");
			strbuf.append("\"paramEncoding\":"  ).append("\"").append(getParamEncoding()).append("\"");
			
			if( getParamTable().size() > 0 )
			{
				strbuf.append(",\"params\":{");
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
				strbuf.append("}");
			}
				
			strbuf.append("}");		
			
			return strbuf.toString();
		}
	}

	class JUnserializer extends Unserializer{
		@Override
		public boolean parseBody(byte[] buf, int offset, int len){
			String str = new String(buf, offset, len);
			Pack pck = new JPack(); 
			try{
				JSONObject jb = new JSONObject(str);
				try{
					src  	= jb.getString("source");
					trgt 	= jb.getString("target");
					cont 	= jb.getString("continue");
				}catch(JSONException e){}
				
				act  	= jb.getString("action");
				type  	= jb.getString("type");				
				paramt  = jb.getString("paramType");
				parame  = jb.getString("paramEncoding");
						
				//JSONArray ja = jb.getJSONArray("params");
				JSONObject params = jb.getJSONObject("params");
				Pack.ParamTable pt = new Pack.ParamTable();
				String[] names = JSONObject.getNames(params);
				for(int i=0; i<names.length; i++){
					JSONArray ja2 = params.getJSONArray(names[i]);
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
	
