package test.atriasoft.ewol;

import org.atriasoft.etk.Configs;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.context.EwolApplication;
import org.atriasoft.ewol.context.EwolContext;
import org.atriasoft.gale.context.GaleContext;

/**
 * Test context for running unit tests without a graphical context.
 * This class initializes a minimal Ewol context that allows widget creation
 * in unit tests.
 */
public class EwolTestContext {

	private static boolean initialized = false;

	/**
	 * Initialize the test context. This method is idempotent - it can be called
	 * multiple times but will only initialize the context once.
	 */
	public static synchronized void init() {
		if (initialized) {
			return;
		}
		// Initialize ewol libraries (Uri, Gale, Esvg)
		Ewol.init();
		// Configure a default font for widgets that need text rendering
		Configs.getConfigFonts().set("FreeSherif", 12);
		// Create a test application
		EwolApplication testApp = new TestEwolApplication();
		// Create the EwolContext first
		EwolContext ewolContext = new EwolContext(testApp);
		// Create the test Gale context with the EwolContext as the application
		new EwolTestGaleContext(ewolContext);
		initialized = true;
	}

	/**
	 * Minimal Ewol application for testing.
	 */
	private static class TestEwolApplication implements EwolApplication {
		@Override
		public void onCreate(EwolContext context) {}

		@Override
		public void onDestroy(EwolContext context) {}

		@Override
		public void onPause(EwolContext context) {}

		@Override
		public void onResume(EwolContext context) {}

		@Override
		public void onStart(EwolContext context) {}

		@Override
		public void onStop(EwolContext context) {}
	}

	/**
	 * Minimal Gale context for testing.
	 * This creates a GaleContext that uses an EwolContext as its application.
	 */
	private static class EwolTestGaleContext extends GaleContext {

		public EwolTestGaleContext(EwolContext ewolContext) {
			super(ewolContext, new String[0]);
			// GaleContext.setContext(this) is already called in the super constructor
		}

		@Override
		public int run() {
			return 0;
		}
	}
}
