/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __TEST_BUTTON_COLOR_H__
#define __TEST_BUTTON_COLOR_H__

#include <appl/Debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/ButtonColor.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/SizerVert.h>

class TestButtonColor : public widget::SizerVert
{
	private:
		widget::ButtonColor* m_testWidget;
	public:
		// Constructeur
		TestButtonColor(void);
		virtual ~TestButtonColor(void);
		// Derived function
		virtual const char * const GetObjectType(void) { return "TestButton"; };
		// Derived function
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		// Derived function
		virtual void OnObjectRemove(ewol::EObject * removeObject);
};

#endif
