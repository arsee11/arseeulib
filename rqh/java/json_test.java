//JsonTest.java

import org.json.*;

class JsonTest{
	public static void main(String[] args){
		JSONObject head = new JSONObject();
		head.put("type", "request");
		head.put("id", 1);
		
		JSONArray params = new JSONArray("[{'name':'id', 'value':'001'},{'name':'id', 'value':'002'}]");
		
		JSONObject pack = new JSONObject();
		pack.put("head", head);
		pack.put("action", "login");
		pack.put("params", params);
		
		String str = pack.toString();
		System.out.println(str);

		JSONObject pack1 = new JSONObject(str);
		System.out.println("action="+pack1.getString("action"));
		System.out.println("head="+pack1.getJSONObject("head").toString());
		JSONArray arr = pack1.getJSONArray("params");
		for(int i=0; i<arr.length(); i++){
			System.out.println(arr.getJSONObject(i).toString() );
		}

	}
}
