//Reactor.java

package mylib.rqh.java;

import java.net.*;
import java.util.*;
import java.io.*;


public class Reactor{
	
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
	
	final int MAX_BUF_LEN=1500;

	public Reactor(){}

	public Reactor(ErrorHandler eh){ this.errorHandler = eh;}

	public Reactor(Socket sock)
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
	
	public void register(View view){
		views.put(view.getName(), view);
	}
	
	public void unregister(View view){
		views.remove(view.getName());
	}
	
		
	void start(Socket s){
		System.out.println("start...");
		Thread t = new Thread( new Runnable(){
			
			@Override
			public void run(){			
				while( !stop ){
					try{
						byte[] buf = new byte[MAX_BUF_LEN];
						int len = 0;
						InputStream r =  s.getInputStream();
						len = r.read(buf,0, MAX_BUF_LEN);
						if( len <= 0 )
							errorHandler.Handle("Connect Lost!", null);
						else
						{
							System.out.println("recv("+len+"):"+new String(buf, 8, len-8));
							ArrayList<Pack> pcks = readPack(buf, len);
							for( Pack pck : pcks ){
								View v = views.get(pck.getTarget());
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
	
	ArrayList<Pack> readPack(byte[] buf, int len) throws Exception
	{
		ArrayList<Pack> pcks = new ArrayList<Pack>();
		int offset=0;
		while(len-offset > 0 ){
			Pack pck = new JPack();
			byte[] bufTmp = new byte[len-offset];
			System.arraycopy(buf, offset, bufTmp, 0, len-offset);
			//System.out.println("buftem("+(len-offset)+")"+new String(bufTmp, 8, len-offset-8));
			
			int bufLenTmp=pck.getUnserializer().solve(bufTmp);
			//System.out.println("bufLenTmp="+bufLenTmp);
			offset += (len-offset-bufLenTmp);
			//System.out.println("offset="+offset);
			
			if( pck.getStatus() )
				pcks.add(pck);
		}

		return pcks;
	
	}
	
	HashMap<String, View> views = new HashMap<String, View>();
	Socket sock;
	boolean stop = false;
	Thread thread;
	ErrorHandler errorHandler = new ErrorHandler();
}
