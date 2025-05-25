package sample.atriasoft.ewol.simpleWindowsWithBox;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class SimpleWindowsWithBoxMain {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("test-data", SimpleWindowsWithImageMain.class, "test-ewol/");
		Uri.setApplication(SimpleWindowsWithBoxMain.class, "test-ewol/");
		Ewol.run(new Appl(), args);
	}
	
	private SimpleWindowsWithBoxMain() {}
}
