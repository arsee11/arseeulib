//MVCRactor.java

package arsee.mvc.java;

import java.net.*;

class MVCRactor{
	
	MVCRactor(Socket sock)
	{
		this.sock = sock;
	}
	
	public boolean listen(){
		start();		
	}
	
	public void stop(){ stop = true; thread.join(); }
	
	public void register(View view){
		views.put(view->getName(), view):
	}
	
	public void unregister(View view){
		views.remove(view->getName()):
	}
	
	void start(){
		Thread t = new Thread( new Runable(){
			
			@override
			public void run(){			
				while( !stop ){
					byte[] buf = new byte[1024];
					int len = 0;
					sock.read(buf, len);
					JPack pck(buf);
					if( pck.getStatus() )
					{
						View v = views.find(pck->getTarget());
						if( v != null)
							v.execute(pck);
					}
				}
			}
		}
	}
	
	
	HashMap<String, View> views = new HashMap<string, View>();;
	Socket sock;
	boolean stop = false;
	Thread thread;
}