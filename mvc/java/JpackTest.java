//JpackTest.java

import mylib.mvc.java.JPack;
import mylib.mvc.java.Pack;

import java.util.HashMap;

class JpackTest{
	public static void main(String[] args){
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
		byte[] buf = pck.new JSerializer().solve();
		
		String str = new String(buf, 8, buf.length-8);
		System.out.println(str);
	}
}
