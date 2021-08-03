package sample.atriasoft.ewol.simpleWindowsLabel;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class SimpleWindowsLabelMain {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("ne", MainCollisionTest.class, "testDataLoxelEngine/");
		Uri.setApplication(SimpleWindowsLabelMain.class);
		Ewol.run(new Appl(), args);
	}
	
	private SimpleWindowsLabelMain() {}
}
