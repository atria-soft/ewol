package sample.atriasoft.ewol.sampleEntry;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class SampleEntryMain {
	public static void main(final String[] args) {
		Ewol.init();
		//Uri.addLibrary("ne", MainCollisionTest.class, "testDataLoxelEngine/");
		Uri.setApplication(MainWindows.class);
		Ewol.run(new Appl(), args);
	}
	
	private SampleEntryMain() {}
}
