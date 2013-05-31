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
	 * @param[in] _argc Standard argc
	 * @param[in] _argv Standard argv
	 * @return normal error int for the application error management
	 */
	int32_t Run(int32_t _argc, const char* _argv[]);
	/**
	 * @brief Request the stop of the program (teminate all the process) no more call at hte application without APP_UnInit();
	 */
	void Stop(void);
	/**
	 * @brief Set a windows to diaplay
	 * @param[in] _windows The requested windows that migt be use for the display
	 */
	void WindowsSet(ewol::Windows* _windows);
	/**
	 * @brief Add a PopUp at the current windows ==> this widget is display over the current element
	 * @param[in] _tmpWidget A pointer on the pop-up widget that might be displayed
	 */
	void WindowsPopUpAdd(ewol::Widget* _tmpWidget);
	/**
	 * @brief Change the windows size
	 * @note work only on computer
	 * @param[in] _size The new windows size
	 */
	void ChangeSize(const ivec2& _size);
	/**
	 * @brief Change the windows curent position
	 * @note work only on computer
	 * @param[in] _pos The new windows position
	 */
	void ChangePos(const ivec2& _pos);
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
	 * @param[in] _hide Status of the visibility of the keyboard
	 */
	void Keyboard(bool _hide);
	/**
	 * @brief Change the title display.
	 * @param[in] title the new title that might be displayed
	 */
	void SetTitle(const etk::UString& _title);
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
	 * @param _source the widget where the event came from
	 * @param _destination the widget where the event mitgh be generated now
	 */
	void InputEventTransfertWidget(ewol::Widget* _source, ewol::Widget* _destination);
	typedef enum {
		SCREEN_ORIENTATION_AUTO = 0,
		SCREEN_ORIENTATION_LANDSCAPE,
		SCREEN_ORIENTATION_PORTRAIT,
	} orientation_te;
	/**
	 * @brief Force a specific orientation for mobile devices
	 * @param[in] orientation the requested position.
	 */
	void ForceOrientation(ewol::orientation_te _orientation);
	/**
	 * @brief Set the Icon of the program
	 * @param[in] _icon new filename icon of the curent program.
	 * @note Does not work on Andoid
	 */
	void SetIcon(const etk::UString& _icon);
	/**
	 * @brief Select the position of the font folder (in the OS path or in the DATA: path)
	 * @param[in] _inOsSystem Set at true if you want to select the os system folder.
	 */
	void SetFontSourcesFolder(bool _inOsSystem);
};

#endif
