package sample.atriasoft.ewol.simpleWindowsWithImage;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class SimpleWindowsWithImageMain {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("test-data", SimpleWindowsWithImageMain.class, "test-ewol/");
		Uri.setApplication(SimpleWindowsWithImageMain.class, "test-ewol/");
		Ewol.run(new Appl(), args);
	}
	
	private SimpleWindowsWithImageMain() {}
}
