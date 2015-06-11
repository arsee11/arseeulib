//MsgView.java

import mylib.mvc.java.MVCView;
import mylib.mvc.java.Pack;
import mylib.mvc.java.JPack;

class MsgView implements MVCView{
	
	@Override
	public String getName(){ return "msgview";}
	
	@Override
	public void invoke(Pack pck){
		for(int i=0; i<pck.getParamsCount(); i++)
		{
			Sytem.out.println("From:"+pck.getParam(i, "from"));
			Sytem.out.println("Msg :"+pck.getParam(i, "msg"));
		}
	}
}