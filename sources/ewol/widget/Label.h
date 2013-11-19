/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LABEL_H__
#define __EWOL_LABEL_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/compositing/Text.h>
#include <ewol/widget/Widget.h>
#include <ewol/widget/WidgetManager.h>

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class Label : public ewol::Widget {
		public:
			// Event list of properties
			static const char * const eventPressed;
			/**
			 * @brief Main call of recording the widget on the List of "widget named creator"
			 */
			static void init(ewol::WidgetManager& _widgetManager);
		private:
			ewol::Text m_text; //!< Compositing text element.
			std::string m_label; //!< decorated text to display.
		public:
			/**
			 * @brief Constructor
			 * @param[in] _newLabel The displayed decorated text.
			 */
			Label(std::string _newLabel="---");
			/**
			 * @brief destructor
			 */
			virtual ~Label(void) {
				
			};
			/**
			 * @brief change the label displayed
			 * @param[in] _newLabel The displayed decorated text.
			 */
			void setLabel(const std::string& _newLabel);
			inline void setValue(const std::string& _newLabel) {
				setLabel(_newLabel);
			};
			/**
			 * @brief get the current displayed label
			 * @return The displayed decorated text.
			 */
			std::string getLabel(void);
			inline std::string getValue(void) {
				return getLabel();
			};
		protected: // Derived function
			virtual void onDraw(void);
		public: // Derived function
			virtual void calculateMinMaxSize(void);
			virtual void onRegenerateDisplay(void);
			virtual bool onEventInput(const ewol::EventInput& _event);
			virtual bool loadXML(exml::Element* _node);
	};
};

#endif
