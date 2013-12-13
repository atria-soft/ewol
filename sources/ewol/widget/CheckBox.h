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
#include <ewol/widget/Manager.h>

extern const char* const ewolEventCheckBoxClicked;

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 */
		class CheckBox : public ewol::Widget {
			public:
				static void init(ewol::widget::Manager& _widgetManager);
			public:
				CheckBox(const std::string& newLabel = "No Label");
				virtual ~CheckBox(void);
				void setLabel(std::string newLabel);
				void setValue(bool val);
				bool getValue(void);
			private:
				ewol::compositing::Text m_oObjectText;
				ewol::compositing::Drawing m_oObjectDecoration;
				std::string m_label;
				bool m_value;
				etk::Color<> m_textColorFg; //!< Text color
				etk::Color<> m_textColorBg; //!< Background color
			protected: // Derived function
				virtual void onDraw(void);
			public: // Derived function
				virtual void calculateMinMaxSize(void);
				virtual void onRegenerateDisplay(void);
				virtual bool onEventInput(const ewol::event::Input& _event);
				virtual bool onEventEntry(const ewol::event::Entry& _event);
		};
	};
};

#endif
