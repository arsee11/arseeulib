//ObjectSerialization.java

package mylib.rqh.java;

import java.lang.reflect.*;
import java.util.ArrayList;
import org.json.*;

public class ObjectSerialization{
	private static boolean isBaseType(String typeName){	
	
		//System.out.println(typeName);
		if(typeName.equals(int.class.getName())
		   ||typeName.equals(float.class.getName())
		   ||typeName.equals(double.class.getName())
	       ||typeName.equals(short.class.getName())
		   ||typeName.equals(char.class.getName())
		   ||typeName.equals(boolean.class.getName())
		   ||typeName.equals(long.class.getName())
		   ||typeName.equals(String.class.getName())
	    ){
			return true;
		}
		
		return false;
	}
	
	private static boolean isArrayListType(String typeName){
		
		//System.out.println(typeName);
		if(typeName.equals(ArrayList.class.getName())){
			return true;
		}
		
		return false;
	}
	
	private static <T>String SerializeField(T obj, Field f) throws Exception{
		
		f.setAccessible(true);
		String value="";
		if(isBaseType(f.getType().getName())){
			if( f.getType().getName().equals(int.class.getName())){
				value ="\""+ new Encoder().encode(Util.int2Bytes( f.getInt(obj)), 4)+"\"";
			}
			if( f.getType().getName().equals(float.class.getName())){
				value ="\""+ new Encoder().encode(Util.float2Bytes( f.getFloat(obj)), 4)+"\"";
			}
			else{
				value = "\""+f.get(obj).toString()+"\"";
			}
		}else if( isArrayListType(f.getType().getName())){			
			value = "{\"v\":[";
			ArrayList array = (ArrayList)f.get(obj);
			int i=0;
			for( i=0; i<array.size()-1; i++ ){
				Object o = array.get(i);
				if(isBaseType(o.getClass().getName())){
					value += "\""+o.toString()+"\",";
				}else{
					value+= Serialize(o)+",";
				}
			}
			Object o = array.get(i);
			if(isBaseType(o.getClass().getName())){
				value += "\""+o.toString()+"\"";
			}else{
				value+= Serialize(o);
			}
			value += "]}";
		}else{			
			value = Serialize(f.get(obj));
		}
		
		return value;
	}
	
	public static <T>String Serialize(T obj){
		String name = obj.getClass().getName();
		StringBuffer strbuf = new StringBuffer();
		strbuf.append("{\"className\":" ).append("\"").append(name ).append("\",");
		
		try{
			Field[] fields= obj.getClass().getDeclaredFields();
			int i;
			for (i = 0; i < fields.length-1; i++) {		
				strbuf.append("\""+fields[i].getName()+"\":" )
					  .append(SerializeField(obj,fields[i])).append(",");
			}
			strbuf.append("\""+fields[i].getName()+"\":" )
			      .append(SerializeField(obj,fields[i])).append("}");
		}catch(Exception e){
			return "";
		}
		
		return strbuf.toString();
	}
	
	private static void UnserializeField(Object obj, JSONObject jo, Field field) throws Exception{
		
		field.setAccessible(true);
		String type = field.getType().getName();
		if( !isBaseType(type) ){
			if(isArrayListType(type) ){
				JSONObject fjo = jo.getJSONObject(field.getName());
				ArrayList array = (ArrayList)field.get(obj);
				JSONArray jarray = fjo.getJSONArray("v");
				for(int i=0; i<jarray.length(); i++){
					Object o;
					//System.out.println(jarray.get(i).getClass().getName());
					if( isBaseType(jarray.get(i).getClass().getName()) ){
						o = jarray.getString(i);
					}else{
						o = Unserialize(jarray.getJSONObject(i) );
					}
					array.add(o);
				}
			}else{
				Object o = Unserialize(jo.getJSONObject(field.getName()) );
				field.set(obj, o);
			}
		}
		else if(type == int.class.getName()){
			int n = jo.getInt( field.getName() );
			field.set(obj, n);	
		}
		else if(type == String.class.getName()){
			String n = jo.getString( field.getName() );
			field.set(obj, n);
		}
	}
	
	public static Object Unserialize(JSONObject jo) throws Exception{
		String className = jo.getString("className");
		Class<?> c = Class.forName(className);
		Object obj = c.newInstance();
		Field[] fields = c.getDeclaredFields();
		for(int j=0; j<fields.length; j++){
			UnserializeField(obj, jo, fields[j]);
		}
		
		return obj;
	}
	
}