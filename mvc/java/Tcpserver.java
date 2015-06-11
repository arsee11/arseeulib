//Tcpserver.java

import java.net.*;
import java.io.*;

public class Tcpserver{
	public static void main(String[] args){
		int port = 9999;

		try{
			ServerSocket s = new ServerSocket(port);
			Socket c = s.accept();
			Reader r = new InputStreamReader(c.getInputStream());
			char[] buf = new char[1024];
			StringBuilder sb = new StringBuilder();
			int len=r.read(buf);
			sb.append(new String(buf, 0, len));
			System.out.println("len:"+len);

			System.out.println("recv:"+sb);
			Writer w = new OutputStreamWriter(c.getOutputStream());
			w.write(sb.toString());
			w.flush();
			r.close();
			w.close();
			c.close();
			s.close();

		}catch(IOException e){
			System.out.println(e.toString());
		}

	}
}
