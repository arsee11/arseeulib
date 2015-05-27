//pack.java

public class Pack{
	public Pack( String src, String trgt, String act ){
		this.src = src;
		this.trgt = trgt;
		this.act = act;
	}

	public String getSource(){ return src;  }
	public String getTarget(){ return trgt; }
	public String getAction(){ return act;  }


	String src;
	String trgt;
	String act;
}
