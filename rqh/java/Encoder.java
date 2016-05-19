package mylib.rqh.java;

public class Encoder {
	
	public String encode(byte[] buf, int len){
		
		String str = "";
		for(int i=0; i<len; i++){
			char[] ch = new char[2];
			int h = buf[i] >> 4;
			ch[0] = itox(h);
			int l = buf[i] & 0x0F;
			ch[1] = itox(l);
			str+= ch[0];
			str+= ch[1];
		}
		return str;
	}
	
	private char itox(int val){
		if(val>=0 && val < 10 ){
			char[] ch = new char[1];
			Integer.toString(val).getChars(0, 1, ch, 0);
			return ch[0];
		}
		
		if( val == 10)
			return 'A';
		
		if( val == 11)
			return 'B';
		
		if( val == 12)
			return 'C';
		
		if( val == 13)
			return 'D';
		
		if( val == 14)
			return 'E';
		
		if( val == -1)
			return 'F';
		
		return 0;
	}
}
