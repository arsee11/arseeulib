//JSerializer.java

package arsee.mvc.java;

import org.json.*;
import java.util.*;

public class JSerializer:extends Pack.Serializer{
	@override
	public String buildBody(Pack pck){
		StringBuffer strbuf = new StringBuffer();
		strbuf.append("{");
		strbuf.append("\"action\":").append("\"").append(pck.getAction()).append("\",");
		strbuf.append("\"source\":").append("\"").append(pck.getSource()).append("\",");
		strbuf.append("\"target\":").append("\"").append(pck.getTarget()).append("\",");
		
		if( pck.getParamTable().size() > 0 )
		{
			strbuf.append("\"params\":[")
			int count=0;
			for(HashMap<String, Object> i:pck.getParamTable()){
				if( i.size() == 0 )
					break;
					
				Iterator j = Map.EntrySet().iterator();
				if( j.hasNext() ){
					if( count == 0)
						strbuf.append("\"params\":[");
					else
						strbuf.append("\"params\":,[");
						
					Map.Entry e = (Map.Entry)j.next();
					strbuf.append("{\"name\":" ).append("\"").append(e.getKey()  ).append("\"");
					strbuf.append("\"value\":").append("\"").append(e.getValue()).append("\"}");
				}
				
				while(j.hasNext()){
					Map.Entry e = (Map.Entry)j.next();
					strbuf.append(",{\"name\":" ).append("\"").append(e.getKey()  ).append("\"");
					strbuf.append("\"value\":").append("\"").append(e.getValue()).append("\"}");
				}
				count++;
				strbuf.append("]")
			}
			
			strbuf.append("]");
		}
		
		strbuf.append("}");		
	}
	
	return strbuf.toString();
}
