package sample.atriasoft.ewol.simpleWindowsWithCheckBox;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class Main {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("test-data", SimpleWindowsWithImageMain.class, "test-ewol/");
		Uri.setApplication(Main.class, "test-ewol/");
		Ewol.run(new Appl(), args);
	}
	
	private Main() {}
}
