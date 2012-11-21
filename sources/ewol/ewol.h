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
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>

namespace ewol
{
	/**
	 * @brief This is the only one things the User might done in his main();
	 * @note To answare you before you ask the question, this is really simple:
	 *       Due to the fect that the current system is multiple-platform, you "main"
	 *       Does not exist in the android platform, then ewol call other start 
	 *       and stop function, to permit to have only one code
	 * @note The main can not be in the ewol, due to the fact thet is an librairy
	 * @param[in] argc Standard argc
	 * @param[in] argv Standard argv
	 * @return normal error int for the application error management
	 */
	int32_t Run(int32_t argc, const char* argv[]);
	/**
	 * @brief Request the stop of the program (teminate all the process) no more call at hte application without APP_UnInit();
	 */
	void Stop(void);
	/**
	 * @brief Set a windows to diaplay
	 * @param[in] windows The requested windows that migt be use for the display
	 */
	void WindowsSet(ewol::Windows * windows);
	/**
	 * @brief Add a PopUp at the current windows ==> this widget is display over the current element
	 * @param[in] tmpWidget A pointer on the pop-up widget that might be displayed
	 */
	void WindowsPopUpAdd(ewol::Widget * tmpWidget);
	/**
	 * @brief Change the windows size
	 * @note work only on computer
	 * @param[in] size The new windows size
	 */
	void ChangeSize(etkVector2D<int32_t> size);
	/**
	 * @brief Change the windows curent position
	 * @note work only on computer
	 * @param[in] pos The new windows position
	 */
	void ChangePos(etkVector2D<int32_t> pos);
	/**
	 * @brief Generate the action of redrawing all the display.
	 */
	void ForceRedrawAll(void);
	/**
	 * @brief Force the calculation of all the size of the widgets
	 */
	void RequestUpdateSize(void);
	/**
	 * @brief Change the status of the Keyboard displat
	 * @note Specific for mobile platform
	 * @param[in] hide Status of the visibility of the keyboard
	 */
	void Keyboard(bool hide);
	/**
	 * @brief Change the title display.
	 * @param[in] title the new title that might be displayed
	 */
	void SetTitle(etk::UString title);
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
	 * @brief This is to transfert the event from one widget to another one
	 * @param source the widget where the event came from
	 * @param destination the widget where the event mitgh be generated now
	 * @return ---
	 */
	void InputEventTransfertWidget(ewol::Widget* source, ewol::Widget* destination);
	typedef enum {
		SCREEN_ORIENTATION_AUTO = 0,
		SCREEN_ORIENTATION_LANDSCAPE,
		SCREEN_ORIENTATION_PORTRAIT,
	} orientation_te;
	/**
	 * @brief 
	 * @param[in] 
	 */
	void ForceOrientation(ewol::orientation_te orientation);
	
};

#endif
