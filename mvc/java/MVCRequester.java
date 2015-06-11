//MVCRequester.java

package mylib.mvc.java;

import java.net.*;

public class MVCRequester{
	public MVCRequester(Socket s, Pack pck){
		this.sock = s;
		this pack = pck;
	}
	
	public MVCRequester(Pack pck){
		this pack = pck;
	}
	
	void setSource(String value){
		pack.setSource(vlaue);
	}
	
	void setTarget(String value){
		pack.setTarget(vlaue);
	}
	
    void setAction(String value){
		pack.setAction(vlaue);
	}
	
    void addParam (String name, Object value){
		params.put(name, value);
	}
	
    void appnedParam(){
		packTable.add(params);
		params.clear();
	}
	
    void post(s){
		byte[] buf = pack.getSerializer().solve();
		Writer w = new OutputStreamWriter( s.getOutputStream() );
		w.flush();
	}
	
	void post(){
		post(sock);
	}
	
	protected Socket sock;
	protected Pack pack;
	protected Pack.ParamTable packTable;
	protected HashMap<String, Object> params;
}