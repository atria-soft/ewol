/**
 * @author Edouard DUPIN
 * 
 * @copyright 2010, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __TEST_LABEL_H__
#define __TEST_LABEL_H__

#include <appl/Debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/Spacer.h>
#include <ewol/widget/SizerVert.h>

class TestLabel : public widget::SizerVert
{
	private:
		widget::Label* m_testWidget;
		int32_t        m_labelId;
	public:
		/**
		 * @brief Constructeur
		 */
		TestLabel(void);
		/**
		 * @brief Destructor
		 */
		virtual ~TestLabel(void);
		// Derived function
		virtual const char * const GetObjectType(void) { return "TestButton"; };
		// Derived function
		virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
		// Derived function
		virtual void OnObjectRemove(ewol::EObject * removeObject);
};

#endif
