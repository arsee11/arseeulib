
//JPack.java

package mylib.mvc.java;

import org.json.*;
import java.util.*;

public class JPack extends Pack{
	
	public JPack( String src, String trgt, String act ){
		super(src, trgt, act);
	}

	public class JSerializer extends Serializer{
	
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
							strbuf.append("[");
						else
							strbuf.append(",[");
							
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
}
	
