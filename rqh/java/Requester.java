//Requester.java

package mylib.rqh.java;

import java.net.*;
import java.util.*;
import java.io.*;

public class Requester{
	public Requester(Socket s, Pack pck){
		this.sock = s;
		this.pack = pck;	
	}
	
	public Requester(Pack pck){
		this.pack = pck;
		pck.setType("request");
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
	
	public void setContinue(String value){
		pack.setContinue(value);
	}
	
	public void setParamType(String value){
		pack.setParamType(value);
	}
	
	public void setParamEncoding(String value){
		pack.setParamEncoding(value);
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
			pack.setStatus(true);
			pack.setParamTable( (Pack.ParamTable)packTable.clone());
			packTable.clear();
			byte[] buf = pack.getSerializer().solve();
			OutputStream w = s.getOutputStream();
			w.write(buf);
			w.flush();
			pack.setStatus(false);
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
