//MVCReactor.java

package mylib.mvc.java;

import java.net.*;
import java.lang.*;
import java.util.*;
import java.io.*;


public class MVCReactor{
	
	public class ErrorHandler{
		void Handle(String errorString, Exception e){
			if( errorString != null)
			{
				System.out.println(errorString);
				if( errorString == "Connect Lost!")
				{
					stop = true;
				}				
			}
			
			if( e != null )
				e.printStackTrace();
		}
	}
	
	final int MAX_BUF_LEN=1024;

	public MVCReactor(){}

	public MVCReactor(ErrorHandler eh){ this.errorHandler = eh;}

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
		System.out.println("start...");
		Thread t = new Thread( new Runnable(){
			
			@Override
			public void run(){			
				while( !stop ){
					try{
						System.out.println("run..");
						byte[] buf = new byte[MAX_BUF_LEN];
						int len = 0;
						InputStream r =  s.getInputStream();
						len = r.read(buf,0, MAX_BUF_LEN);
						System.out.println("len:"+len);
						if( len <= 0 )
							errorHandler.Handle("Connect Lost!", null);
						else
						{
							System.out.println("recv("+len+"):"+new String(buf, 8, len-8));
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
				
				
			}
		});
		
		t.start();
	}
	
	
	HashMap<String, MVCView> views = new HashMap<String, MVCView>();
	Socket sock;
	boolean stop = false;
	Thread thread;
	ErrorHandler errorHandler = new ErrorHandler();
}
