//JpackTest.java

import mylib.mvc.java.JPack;
import mylib.mvc.java.Pack;

import java.util.HashMap;

class JpackTest{
	void testUnserializer(){
		String str = "{\"action\":\"registry\",\"source\":\"member\",\"target\":\"memberlist\",\"params\":[param:[{\"name\":\"id\",\"value\":\"Jim\"},{\"name\":\"pwd\",\"value\":\"123\"}],param:[{\"name\":\"id\",\"value\":\"Alex\"},{\"name\":\"pwd\",\"value\":\"123\"}]]}";
		byte[] buf = new byte[str.length()+8];
		System.arraycopy(Pack.HEAD, 0, buf, 0, 4);
		Pack.int2Bytes(str.length(), buf, 4);
		System.arraycopy(buf, 8, str.getBytes(), 0, str.length());
		JPack pck = new JPack();
		try{
			pck.getUnSerializer().solve(buf);
		}catch(Exception e){
			System.out.println(e.toString());
		}

		System.out.println(pck.getStatus());
		System.out.println(pck.getAction());
		System.out.println(pck.getSource());
		System.out.println(pck.getTarget());
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

	public static void main(String[] args){
		new JpackTest().testUnserializer();
	}

}
