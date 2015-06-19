//MVCReactor.java

package mylib.mvc.java;

import java.net.*;
import java.lang.*;
import java.util.*;
import java.io.*;


public class MVCReactor{
	
	final int MAX_BUF_LEN=1024;

	public MVCReactor(){}

	public MVCReactor(Socket sock)
	{
		this.sock = sock;
	}
	
	public void listen(Socket s){
		start(s);		
	}
	
	public void stop()
	{
		stop = true; 
		try{	
			thread.join(); 
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void register(MVCView view){
		views.put(view.getName(), view);
	}
	
	public void unregister(MVCView view){
		views.remove(view.getName());
	}
	
	void start(Socket s){
		Thread t = new Thread( new Runnable(){
			
			@Override
			public void run(){			
				try{
					while( !stop ){
						byte[] buf = new byte[MAX_BUF_LEN];
						int len = 0;
						InputStream r =  sock.getInputStream();
						len = r.read(buf,0, MAX_BUF_LEN);
						Pack pck = new JPack();
						pck.getUnserializer().solve(buf);
						if( pck.getStatus() )
						{
							MVCView v = views.get(pck.getTarget());
							if( v != null)
								v.invoke(pck);
						}
					}
				}catch(Exception e){
					e.printStackTrace();
				}
				
			}
		});
	}
	
	
	HashMap<String, MVCView> views = new HashMap<String, MVCView>();
	Socket sock;
	boolean stop = false;
	Thread thread;
}
