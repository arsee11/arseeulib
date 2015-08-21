//Tcpclient.java

import java.net.*;
import java.io.*;

public class Tcpclient{
	public static void main(String[] args){
		String ip="127.0.0.1";
		int port = 9999;
		
		try{
			Socket s = new Socket(ip, port);
			Writer w = new OutputStreamWriter( s.getOutputStream() );
			w.write(" Hello, can you hear me!");
			w.flush();
			Reader r = new InputStreamReader(s.getInputStream());
			char[] buf = new char[1024];
			int len = r.read(buf);
			System.out.println("recv:"+new String(buf, 0, len));
			
			w.close();
			r.close();
			s.close();
		}catch(IOException e){
			System.out.println(e.toString());
		}

	}
}
