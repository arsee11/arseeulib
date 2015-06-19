// Client.java

import java.net.*;
import java.io.*;
import mylib.mvc.java.MVCReactor;
import mylib.mvc.java.MVCRequester;
import mylib.mvc.java.JPack;

class Client{
	final static int REMOTE_PORT = 11111;
	final static String REMOTE_IP = "152.29.178.221";
	
	public static void main(String[] args){
		try{
			Socket s = new Socket(REMOTE_IP, REMOTE_PORT);
			MVCReactor reactor = new MVCReactor();
			reactor.register(new MsgView());
			reactor.listen(s);
			
			BufferedReader stdin = new BufferedReader( new InputStreamReader(System.in) );
			System.out.print("From:");
			String from = stdin.readLine();
			System.out.print("To:");
			String to = stdin.readLine();
			System.out.print("Msg:");
			String msg = stdin.readLine();
			MVCRequester rqt = new MVCRequester(new JPack());
			rqt.setSource("msgview");
			rqt.setTarget("msg");
			rqt.setAction("trans_msg");
			rqt.addParam("from", from);
			rqt.addParam("to", from);
			rqt.addParam("msg", from);
			rqt.appnedParam();
			rqt.post(s);
			
		}catch(IOException e){
			e.printStackTrace();
		}
	}
}
