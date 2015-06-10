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
			
			@Override
			public void run(){
				final int MAX = 1024;
				while( !stop ){
					byte[] buf = new byte[MAX];
					int len = 0;
					Reader r = new InputStreamReader( sock.getInputStream() );
					len = r.read(buf, 0, MAX);
					Pack pck = new JPack();
					pck.getUnserializer().solve(buf);
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