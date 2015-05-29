//JUnSerializer.java

package arsee.mvc.java;

import org.json.*;
import java.util.*;

public class JUnSerializer:extends Pack.UnSerializer{
	@override
	public Pack parseBody(byte[] buf, int offset, int len){
			StringBuffer sb = new StringBuffer(buf, offset, len);
			try{
				JSONObject jb = new JSONObject(sb.toString());
				Pack pck = new Pack(
					jb.getString("source"),
					jb.getString("target"),
					jb.getString("action")
				);
						
				JSONArray ja = jb.getJSONArray("params");
				Pack.ParamTable pt = new Pack.ParamTable();
				for(int i=0; i<ja.getLength(); i++){
					JSONArray ja2 = ja.getJSONObject(i).getJSONArray();
					HashMap<String, Object> p = new HashMap<String, Object>();
					for(int j=0; j<ja2.getLength(); j++){
						JSONObject param = ja2.getJSONObject(j);
						p.put(param.getString("name"), param.getString("value"));					
					}
					
					pt.add(p);
				}
			}catch(JSONException e){
				pck.setStatus(false);
			}
			
			pck.setStatus(true);
			return pck;
	}
	
	return strbuf.toString();
}
