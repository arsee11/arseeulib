//JpackTest.java

import mylib.mvc.java.JPack;
import mylib.mvc.java.Pack;
import mylib.mvc.java.Util;

import java.util.*;

class JpackTest{
	void testUnserializer(){
		String str = "{\"action\":\"registry\",\"source\":\"member\",\"target\":\"memberlist\",\"params\":{\"param0\":[{\"name\":\"id\",\"value\":\"Jim\"},{\"name\":\"pwd\",\"value\":\"123\"}],\"param1\":[{\"name\":\"id\",\"value\":\"Alex\"},{\"name\":\"pwd\",\"value\":\"123\"}]}}";
		byte[] buf = new byte[str.length()+8];
		System.arraycopy(Pack.HEAD, 0, buf, 0, 4);
		JPack pck = new JPack();
		try{
			Util.int2Bytes(str.length(), buf, 4);
			System.arraycopy(str.getBytes(), 0, buf, 8, str.length());
			pck.getUnserializer().solve(buf);
		}catch(Exception e){
			System.out.println(e.toString());
		}

		System.out.println("status:"+pck.getStatus());
		System.out.println("action:"+pck.getAction());
		System.out.println("source:"+pck.getSource());
		System.out.println("target:"+pck.getTarget());
		for(HashMap<String, Object> p:pck.getParamTable()){
			Iterator it = p.entrySet().iterator();
			while(it.hasNext()){
				Map.Entry e = (Map.Entry)it.next();
				System.out.println(e.getKey()+"="+ e.getValue());
			}
		}
	}

	void testSerializer(){
		JPack pck = new JPack("member", "memberlist", "registry");
		Pack.ParamTable pt = new Pack.ParamTable();
		HashMap<String, Object> h1 = new HashMap<String, Object>();
		h1.put("id", "Jim");
		h1.put("pwd", "123");
		pt.add(h1);
		
		HashMap<String, Object> h2 = new HashMap<String, Object>();
		h2.put("id", "Alex");
		h2.put("pwd", "123");
		pt.add(h2);
		
		pck.setParamTable(pt);
		byte[] buf = pck.getSerializer().solve();
		
		String str = new String(buf, 8, buf.length-8);
		System.out.println(str);
	}
	
	void testSerializer_Notparams(){
		JPack pck = new JPack("member", "memberlist", "registry");
		byte[] buf = pck.getSerializer().solve();
		
		String str = new String(buf, 8, buf.length-8);
		System.out.println(str);
	}

	public static void main(String[] args){
		new JpackTest().testUnserializer();
		new JpackTest().testSerializer();
		new JpackTest().testSerializer_Notparams();
	}

}
