//Util.java

package mylib.rqh.java;

import java.lang.Exception;

public class Util{
	//low order
	public  static byte[] int2Bytes(int i, byte[] bytes, int offset)throws Exception  {   
		if( bytes==null || bytes.length-offset < 4 )
			throw new Exception("bytes==null||bytes.length-offset<4");

		bytes[0+offset] = (byte)(i & 0xFF);
		bytes[1+offset] = (byte)((i >> 8) & 0xFF);
		bytes[2+offset] = (byte)((i >> 16) & 0xFF); 
		bytes[3+offset] = (byte)((i >> 24)& 0xFF);
		return bytes;
	}

		
	public  static byte[] int2Bytes(int i)throws Exception  {   
		byte[] bytes = new byte[4];   
		int2Bytes(i, bytes, 0);
		return bytes;
	}
	
			
	public  static byte[] float2Bytes(float i)throws Exception  {   
		byte[] bytes = new byte[4];  
		int ival = Float.floatToIntBits(i);
		int2Bytes(ival, bytes, 0);
		return bytes;
	}
	
	
	//low order
	public static int bytes2Int(byte[] bytes, int offset)throws Exception {
		if( bytes==null || bytes.length-offset < 4 )
			throw new Exception("bytes==null||bytes.length-offset<4");

		int value= 0;
		for (int i = 0; i < 4; i++) {
			int shift= (i) * 8;
			value +=(bytes[i+offset] & 0x000000FF) << shift;
		}

		return value;
	}

	public static int bytes2Int(byte[] bytes)throws Exception  {
		return bytes2Int(bytes, 0);
	}

	public static void main(String[] args){
		try{
			byte[] bytes = int2Bytes(9999);
			int n = bytes2Int(bytes);
			System.out.println(n);

			byte[] bytes2 = new byte[8];
			int2Bytes(240, bytes2, 4);
			n = bytes2Int(bytes2,4);
			System.out.println(n);
		}catch(Exception e){
			System.out.println(e.toString());
		}

	}
}
