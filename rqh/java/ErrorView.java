//ErrorView.java

import mylib.rqh.java.MVCView;
import mylib.rqh.java.Pack;
import mylib.rqh.java.JPack;

class ErrorView implements View{
	
	@Override
	public String getName(){ return "errorview";}
	
	@Override
	public boolean invoke(Pack pck){
		for(int i=0; i<pck.getParamTable().size(); i++)
		{
			String code = (String)pck.getParam(i, "code");
			if( code.equals("0") )
				System.out.println("Request OK");
			else
				System.out.println("Request failed");
				
			System.out.println("Action:"+pck.getAction());
		}

		return true;
	}
}
