package sample.atriasoft.ewol.simpleWindowsWithButton;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class SimpleWindowsWithButtonMain {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("test-data", SimpleWindowsWithImageMain.class, "test-ewol/");
		Uri.setApplication(SimpleWindowsWithButtonMain.class, "test-ewol/");
		Ewol.run(new Appl(), args);
	}

	private SimpleWindowsWithButtonMain() {}
}
