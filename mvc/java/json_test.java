//JsonTest.java

import org.json.*;

class JsonTest{
	public static void main(String[] args){
		JSONObject head;
		head.put("type", "request");
		head.put("id", 1);
		
		JSONObject action("{'action':'login'}");
		JSONArray params("[{'name':'id', 'value':'001'},{'name':'id', 'value':'002'}]");
		JSONObject param;
		params.put("params", params);
		
		JSONObject pack;
		pack.put(head);
		pack.put(action);
		pack.put(param);
		
		System.out.println(pack.toString());
	}
}