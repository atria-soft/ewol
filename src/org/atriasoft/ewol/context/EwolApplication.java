package org.atriasoft.ewol.context;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public interface EwolApplication {
	/**
	 * The application is created.
	 * @param _context Current ewol context.
	 */
	void onCreate(EwolContext _context);
	
	/**
	 * The application is removed (call destructor just adter it.).
	 * @param _context Current ewol context.
	 */
	void onDestroy(EwolContext _context);
	
	/**
	 * The user request application removing.
	 * @param _context Current ewol context.
	 */
	default void onKillDemand(final EwolContext _context) {
		_context.exit(0);
	}
	
	/**
	 * The application is Hide / not visible.
	 * @param _context Current ewol context.
	 */
	void onPause(EwolContext _context);
	
	/**
	 * The application is resumed (now visible).
	 * @param _context Current ewol context.
	 */
	void onResume(EwolContext _context);
	
	/**
	 * The application is started.
	 * @param _context Current ewol context.
	 */
	void onStart(EwolContext _context);
	
	/**
	 * The application is stopped.
	 * @param _context Current ewol context.
	 */
	void onStop(EwolContext _context);
}