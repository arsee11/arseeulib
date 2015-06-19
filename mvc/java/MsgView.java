//MsgView.java

import mylib.mvc.java.MVCView;
import mylib.mvc.java.Pack;
import mylib.mvc.java.JPack;

class MsgView implements MVCView{
	
	@Override
	public String getName(){ return "msgview";}
	
	@Override
	public boolean invoke(Pack pck){
		for(int i=0; i<pck.getParamTable().size(); i++)
		{
			System.out.println("From:"+pck.getParam(i, "from"));
			System.out.println("Msg :"+pck.getParam(i, "msg"));
		}

		return true;
	}
}
