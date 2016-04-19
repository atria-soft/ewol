/** @file
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#pragma once

namespace ewol {
	class Context;
	namespace context {
		class Application {
			public:
				Application();
				virtual ~Application();
			public:
				/**
				 * @brief The application is created.
				 * @param[in] _context Current ewol context.
				 */
				virtual void onCreate(ewol::Context& _context);
				/**
				 * @brief The application is started.
				 * @param[in] _context Current ewol context.
				 */
				virtual void onStart(ewol::Context& _context);
				/**
				 * @brief The application is resumed (now visible).
				 * @param[in] _context Current ewol context.
				 */
				virtual void onResume(ewol::Context& _context);
				/**
				 * @brief The application is Hide / not visible.
				 * @param[in] _context Current ewol context.
				 */
				virtual void onPause(ewol::Context& _context);
				/**
				 * @brief The application is stopped.
				 * @param[in] _context Current ewol context.
				 */
				virtual void onStop(ewol::Context& _context);
				/**
				 * @brief The application is removed (call destructor just adter it.).
				 * @param[in] _context Current ewol context.
				 */
				virtual void onDestroy(ewol::Context& _context);
				/**
				 * @brief The user request application removing.
				 * @param[in] _context Current ewol context.
				 */
				virtual void onKillDemand(ewol::Context& _context);
		};
	};
};
