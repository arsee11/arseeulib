//MsgView.java

import mylib.mvc.java.MVCView;
import mylib.mvc.java.Pack;
import mylib.mvc.java.JPack;
import java.util.Base64;

class MsgView implements MVCView{
	
	@Override
	public String getName(){ return "msgview";}
	
	@Override
	public boolean invoke(Pack pck){
		System.out.println("You got a message:");
		for(int i=0; i<pck.getParamTable().size(); i++)
		{
			System.out.println("From:"+pck.getParam(i, "from"));
			System.out.println("Msg :"+new String( Base64.getDecoder().decode( (String)pck.getParam(i, "msg"))));
		}

		return true;
	}
}
