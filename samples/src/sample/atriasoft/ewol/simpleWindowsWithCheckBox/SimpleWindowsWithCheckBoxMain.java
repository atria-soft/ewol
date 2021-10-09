package sample.atriasoft.ewol.simpleWindowsWithCheckBox;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class SimpleWindowsWithCheckBoxMain {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("test-data", SimpleWindowsWithImageMain.class, "test-ewol/");
		Uri.setApplication(SimpleWindowsWithCheckBoxMain.class, "test-ewol/");
		Ewol.run(new Appl(), args);
	}
	
	private SimpleWindowsWithCheckBoxMain() {}
}
