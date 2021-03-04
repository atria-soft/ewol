package org.atriasoft.ewol.context;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public interface EwolApplication {
	/**
	 * @brief The application is created.
	 * @param[in] _context Current ewol context.
	 */
	void onCreate(EwolContext _context);
	
	/**
	 * @brief The application is removed (call destructor just adter it.).
	 * @param[in] _context Current ewol context.
	 */
	void onDestroy(EwolContext _context);
	
	/**
	 * @brief The user request application removing.
	 * @param[in] _context Current ewol context.
	 */
	default void onKillDemand(final EwolContext _context) {
		_context.exit(0);
	}
	
	/**
	 * @brief The application is Hide / not visible.
	 * @param[in] _context Current ewol context.
	 */
	void onPause(EwolContext _context);
	
	/**
	 * @brief The application is resumed (now visible).
	 * @param[in] _context Current ewol context.
	 */
	void onResume(EwolContext _context);
	
	/**
	 * @brief The application is started.
	 * @param[in] _context Current ewol context.
	 */
	void onStart(EwolContext _context);
	
	/**
	 * @brief The application is stopped.
	 * @param[in] _context Current ewol context.
	 */
	void onStop(EwolContext _context);
}