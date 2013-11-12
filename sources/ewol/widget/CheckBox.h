/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_CHECK_BOX_H__
#define __EWOL_CHECK_BOX_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/WidgetManager.h>

extern const char* const ewolEventCheckBoxClicked;

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class CheckBox : public ewol::Widget {
		public:
			static void init(ewol::WidgetManager& _widgetManager);
		public:
			CheckBox(const std::string& newLabel = "No Label");
			virtual ~CheckBox(void);
			void setLabel(std::string newLabel);
			void setValue(bool val);
			bool getValue(void);
		private:
			ewol::Text m_oObjectText;
			ewol::Drawing m_oObjectDecoration;
			std::string m_label;
			bool m_value;
			etk::Color<> m_textColorFg; //!< Text color
			etk::Color<> m_textColorBg; //!< Background color
		protected: // Derived function
			virtual void onDraw(void);
		public: // Derived function
			virtual const char * const getObjectType(void) {
				return "widget::CheckBox";
			};
			virtual void calculateMinMaxSize(void);
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual bool onEventEntry(const ewol::EventEntry& _event);
	};
	
};

#endif
