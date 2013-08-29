/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_H__
#define __EWOL_H__

#include <etk/types.h>
#include <etk/UString.h>

namespace ewol
{
	/**
	 * @brief This is the only one things the User might done in his main();
	 * @note To answare you before you ask the question, this is really simple:
	 *       Due to the fect that the current system is multiple-platform, you "main"
	 *       Does not exist in the android platform, then ewol call other start 
	 *       and stop function, to permit to have only one code
	 * @note The main can not be in the ewol, due to the fact thet is an librairy
	 * @param[in] _argc Standard argc
	 * @param[in] _argv Standard argv
	 * @return normal error int for the application error management
	 */
	int32_t Run(int32_t _argc, const char* _argv[]);
	/**
	 * @brief Get EWOL version
	 * @return The string that describe ewol version
	 */
	etk::UString GetVersion(void);
	/**
	 * @brief Get current time in us...
	 * @return The current time
	 */
	int64_t GetTime(void);
	/**
	 * @brief Get compilation mode (release/debug)
	 * @return the string of the mode of commpilation
	 */
	etk::UString GetCompilationMode(void);
	/**
	 * @brief Get the board type ()
	 * @return the string of the mode of commpilation
	 */
	etk::UString GetBoardType(void);
	
	typedef enum {
		SCREEN_ORIENTATION_AUTO = 0,
		SCREEN_ORIENTATION_LANDSCAPE,
		SCREEN_ORIENTATION_PORTRAIT,
	} orientation_te;
	/*
	/ **
	 * @brief Force a specific orientation for mobile devices
	 * @param[in] orientation the requested position.
	 * /
	void ForceOrientation(ewol::orientation_te _orientation);
	*/
};

#endif
