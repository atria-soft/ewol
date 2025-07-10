package sample.atriasoft.ewol;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;

public class MainApplicaitionStarter {
	public static void main(final String[] args) {
		Ewol.init();
		Uri.setApplication(MainApplicaitionStarter.class, "test-ewol/");
		Ewol.run(new Appl(), args);
	}

	private MainApplicaitionStarter() {
	}
}
