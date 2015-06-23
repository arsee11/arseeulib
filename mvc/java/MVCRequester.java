//MVCRequester.java

package mylib.mvc.java;

import java.net.*;
import java.util.*;
import java.io.*;

public class MVCRequester{
	public MVCRequester(Socket s, Pack pck){
		this.sock = s;
		this.pack = pck;	
	}
	
	public MVCRequester(Pack pck){
		this.pack = pck;
	}
	
	public void setSource(String value){
		pack.setSource(value);
	}
	
	public void setTarget(String value){
		pack.setTarget(value);
	}
	
    	public void setAction(String value){
		pack.setAction(value);
	}
	
    	public void addParam (String name, Object value){
		params.put(name, value);
	}
	
    	public void appnedParam(){
		packTable.add((HashMap<String, Object>)params.clone());
		params.clear();
	}
	
    	public void post(Socket s){
		try{
			pack.setParamTable( (Pack.ParamTable)packTable.clone());
			packTable.clear();
			byte[] buf = pack.getSerializer().solve();
			OutputStream w = s.getOutputStream();
			w.write(buf);
			w.flush();
		}catch(IOException e){
			e.printStackTrace();
		}
	}
	
	public void post(){
		post(sock);
	}
	
	protected Socket sock;
	protected Pack pack;
	protected Pack.ParamTable packTable = new Pack.ParamTable();
	protected HashMap<String, Object> params = new HashMap<String, Object>();
}
