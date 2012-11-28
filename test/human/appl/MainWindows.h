/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __MAIN_WINDOWS_H__
#define __MAIN_WINDOWS_H__

#include <appl/Debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Windows.h>
#include <ewol/widget/Button.h>

class MainWindows : public ewol::Windows
{
	private:
		widget::Button*  m_button;
	public:
		// Constructeur
		MainWindows(void);
		~MainWindows(void);
		// Derived function
		const char * const GetObjectType(void) { return "MainWindows"; };
		// Derived function
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		// Derived function
		virtual void OnObjectRemove(ewol::EObject * removeObject);
};

#endif


