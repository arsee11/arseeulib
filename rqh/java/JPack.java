
//JPack.java

package mylib.rqh.java;

import org.json.*;

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
			
			if( getObjects().size() > 0 )
			{
				strbuf.append(",\"params\":{");
				int count=0;
				for(Object i:getObjects()){
											
					if( count == 0)
						strbuf.append("\"param"+count+"\":");
					else
						strbuf.append(",\"param"+count+"\":");
						
					strbuf.append( ObjectSerialization.Serialize(i) );
					count++;					
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
						
				JSONObject params = jb.getJSONObject("params");
				String[] names = JSONObject.getNames(params);
				for(int i=0; names!=null&&i<names.length; i++){
					JSONObject jo = params.getJSONObject(names[i]);					
					Object obj = ObjectSerialization.Unserialize(jo);
					addObject(obj);
				}
			}catch(JSONException e){
				setStatus(false);
				e.printStackTrace();

				return false;
			}catch(Exception e){
				setStatus(false);
				e.printStackTrace();

				return false;
			}
			
			setStatus(true);
			
			return true;
		}
	}
	
}
	
