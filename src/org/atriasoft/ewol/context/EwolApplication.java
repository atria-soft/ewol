package org.atriasoft.ewol.context;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public interface EwolApplication {
	/**
	 * The application is created.
	 * @param context Current ewol context.
	 */
	void onCreate(EwolContext context);
	
	/**
	 * The application is removed (call destructor just adter it.).
	 * @param context Current ewol context.
	 */
	void onDestroy(EwolContext context);
	
	/**
	 * The user request application removing.
	 * @param context Current ewol context.
	 */
	default void onKillDemand(final EwolContext context) {
		context.exit(0);
	}
	
	/**
	 * The application is Hide / not visible.
	 * @param context Current ewol context.
	 */
	void onPause(EwolContext context);
	
	/**
	 * The application is resumed (now visible).
	 * @param context Current ewol context.
	 */
	void onResume(EwolContext context);
	
	/**
	 * The application is started.
	 * @param context Current ewol context.
	 */
	void onStart(EwolContext context);
	
	/**
	 * The application is stopped.
	 * @param context Current ewol context.
	 */
	void onStop(EwolContext context);
}