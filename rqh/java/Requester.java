//Requester.java

package mylib.rqh.java;

import java.net.*;
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
	
    public <T>void addParam (T obj){
		pack.addObject(obj);
	}
	

	
    public void post(Socket s){
		try{
			pack.setStatus(true);
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
}
