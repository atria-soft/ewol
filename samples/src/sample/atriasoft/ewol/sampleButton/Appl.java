package sample.atriasoft.ewol.sampleButton;

import org.atriasoft.etk.Configs;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.context.EwolApplication;
import org.atriasoft.ewol.context.EwolContext;

public class Appl implements EwolApplication {
	
	//! [ewol_sample_HW_main_application]
	private void localCreate(final EwolContext context) {
		//! [ewol_sample_HW_main_parse_arguments]
		// parse all the argument of the application
		for (int iii = 0; iii < context.getCmd().size(); iii++) {
			String tmpppp = context.getCmd().get(iii);
			if (tmpppp == "-h" || tmpppp == "--help") {
				Log.print("  -h/--help display this help");
				System.exit(0);
			}
		}
		//! [ewol_sample_HW_main_parse_arguments]
		//! [ewol_sample_HW_main_set_windows_size]
		// TODO : Remove this: Move if in the windows properties
		context.setSize(new Vector2f(800, 600));
		//! [ewol_sample_HW_main_set_windows_size]
		//! [ewol_sample_HW_main_set_font_property]
		// select font preference of der with a basic application size
		Configs.getConfigFonts().set("FreeSherif", 48);
		//! [ewol_sample_HW_main_set_font_property]
		//! [ewol_sample_HW_main_set_windows]
		// Create the windows
		MainWindows basicWindows = new MainWindows();
		// configure the ewol context to use the new windows
		context.setWindows(basicWindows);
		//! [ewol_sample_HW_main_set_windows]
	}
	
	@Override
	public void onCreate(final EwolContext context) {
		Log.info("Application onCreate: [BEGIN]");
		localCreate(context);
		Log.info("Application onCreate: [ END ]");
	}
	
	@Override
	public void onDestroy(final EwolContext context) {
		Log.info("Application onDestroy: [BEGIN]");
		
		Log.info("Application onDestroy: [ END ]");
	}
	
	@Override
	public void onPause(final EwolContext context) {
		Log.info("Application onPause: [BEGIN]");
		
		Log.info("Application onPause: [ END ]");
	}
	
	@Override
	public void onResume(final EwolContext context) {
		Log.info("Application onResume: [BEGIN]");
		
		Log.info("Application onResume: [ END ]");
	}
	
	@Override
	public void onStart(final EwolContext context) {
		Log.info("Application onStart: [BEGIN]");
		
		Log.info("Application onStart: [ END ]");
	}
	
	@Override
	public void onStop(final EwolContext context) {
		Log.info("Application onStop: [BEGIN]");
		
		Log.info("Application onStop: [ END ]");
	}
	
}