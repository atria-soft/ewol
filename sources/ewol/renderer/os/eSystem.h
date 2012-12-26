/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SYSTEM_H__
#define __EWOL_SYSTEM_H__

#include <etk/types.h>
#include <ewol/key.h>
#include <ewol/clipBoard.h>
#include <ewol/widget/Windows.h>

namespace eSystem
{
	void Init(void);
	void UnInit(void);
	
	void SetArchiveDir(int mode, const char* str);
	
	void Resize(int w, int h );
	void Move(int x, int y );
	void Hide(void);
	void Show(void);
	
	void SetInputMotion(int pointerID, float x, float y);
	void SetInputState(int pointerID, bool isDown, float x, float y);
	
	void SetMouseMotion(int pointerID, float x, float y);
	void SetMouseState(int pointerID, bool isDown, float x, float y);
	
	void SetKeyboard(ewol::SpecialKey& special,
	                 uniChar_t myChar,
	                 bool isDown);
	void SetKeyboardMove(ewol::SpecialKey& special,
	                     ewol::keyEvent::keyboard_te move,
	                     bool isDown);
	
	void ClipBoardArrive(ewol::clipBoard::clipboardListe_te clipboardID);
	
	void RequestUpdateSize(void);
	
	// return true if a flush is needed
	bool Draw(bool displayEveryTime);
	/**
	 * @brief Inform object that an other object is removed ...
	 * @param[in] removeObject Pointer on the EObject remeved ==> the user must remove all reference on this EObject
	 * @note : Sub classes must call this class
	 */
	void OnObjectRemove(ewol::EObject * removeObject);
	/**
	 * @brief reset event management for the IO like Input ou Mouse or keyborad
	 */
	void ResetIOEvent(void);
	/**
	 * @brief Inform the system that the OpenGL constext has been destroy ==> use to automaticly reload the texture and other thinks ...
	 */
	void OpenGlContextDestroy(void);
	/**
	 * @brief Inform the system that the Application has been killed
	 */
	void OnKill(void);
	/**
	 * @brief set the current windows to display :
	 * @param windows windows that might be displayed
	 */
	void SetCurrentWindows(ewol::Windows * windows);
	/**
	 * @brief Get the current windows that is displayed
	 * @return the current handle on the windows (can be null)
	 */
	ewol::Windows* GetCurrentWindows(void);
	/**
	 * @brief Get the current windows size
	 * @return the current size ...
	 */
	ivec2 GetSize(void);
	/**
	 * @brief Redraw all the windows
	 */
	void ForceRedrawAll(void);
	/**
	 * @brief This is to transfert the event from one widget to another one
	 * @param source the widget where the event came from
	 * @param destination the widget where the event mitgh be generated now
	 */
	void InputEventTransfertWidget(ewol::Widget* source, ewol::Widget* destination);
	/**
	 * @brief This fonction lock the pointer properties to move in relative instead of absolute
	 * @param[in] widget The widget that lock the pointer events
	 */
	void InputEventGrabPointer(ewol::Widget* widget);
	/**
	 * @brief This fonction un-lock the pointer properties to move in relative instead of absolute
	 */
	void InputEventUnGrabPointer(void);
};


#endif

