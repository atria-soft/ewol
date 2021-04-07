package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class SimpleWindowsWithImageMain {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("ne", MainCollisionTest.class, "testDataLoxelEngine/");
		Uri.setApplication(SimpleWindowsWithImageMain.class);
		Ewol.run(new Appl(), args);
	}
	
	private SimpleWindowsWithImageMain() {}
}
