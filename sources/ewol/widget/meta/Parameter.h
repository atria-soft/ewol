/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_PARAMETER_H__
#define __EWOL_WIDGET_PARAMETER_H__

#include <etk/types.h>
#include <etk/UString.h>
#include <ewol/debug.h>
#include <ewol/widget/PopUp.h>
#include <ewol/widget/Button.h>
#include <ewol/widget/Entry.h>
#include <ewol/widget/Label.h>
#include <ewol/widget/CheckBox.h>
#include <ewol/widget/WSlider.h>
#include <ewol/widget/meta/ParameterList.h>

extern const char * const ewolEventParameterClose;

namespace widget {
	class Parameter : public widget::PopUp
	{
		public:
			Parameter(void);
			~Parameter(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolParameter"; };
			// Derived function
			virtual void OnReceiveMessage(ewol::EObject * CallerObject, const char * eventId, etk::UString data);
			// Derived function
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			
			void SetTitle(etk::UString label);
			void MenuAdd(etk::UString label, etk::UString image, ewol::Widget* associateWidget);
			void MenuAddGroup(etk::UString label);
			void MenuClear(void);
			void MenuSeparator(void);
		private:
			int32_t                 m_currentIdList;
			widget::Label*          m_widgetTitle;
			widget::Button*         m_widgetCancel;
			widget::ParameterList*  m_paramList;
			widget::WSlider*        m_wSlider;
	};
	
};

#endif
