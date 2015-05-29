//JpackTest.java

import arsee.mvc.java.*;

class JapckTest{
	public static void main(String[] args){
		Pack pck("member", "memberlist", "registry");
		Pack.ParamTable pt = new Pack.ParamTable();
		HashMap h1 = new HashMap<String, Object>();
		h1.put("id", "Jim") );
		h1.put("pwd", "123") );
		pt.add(h1);
		
		HashMap h2 = new HashMap<String, Object>();
		h2.put("id", "Alex") );
		h2.put("pwd", "123") );
		pt.add(h2);
		
		pck.setParamTable(pt);
		string str = new JSerializer().solve(pck);
	}
}