package sample.atriasoft.ewol.ComplexWindiows1;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class ComplexeWindows1Main {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("ne", MainCollisionTest.class, "testDataLoxelEngine/");
		Uri.setApplication(ComplexeWindows1Main.class);
		Ewol.run(new Appl(), args);
	}
	
	private ComplexeWindows1Main() {}
}
