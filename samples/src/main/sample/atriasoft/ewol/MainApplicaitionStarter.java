package sample.atriasoft.ewol;

import java.util.logging.LogManager;

import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.Ewol;
import org.slf4j.bridge.SLF4JBridgeHandler;

public class MainApplicaitionStarter {
	public static void main(final String[] args) {
		// Loop-back of logger JDK logging API to SLF4J
		LogManager.getLogManager().reset();
		SLF4JBridgeHandler.install();
		Ewol.init();
		Uri.setApplication(MainApplicaitionStarter.class, "test-ewol/");
		Ewol.run(new Appl(), args);
	}
	
	private MainApplicaitionStarter() {}
}
