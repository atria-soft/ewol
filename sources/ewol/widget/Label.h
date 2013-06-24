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

namespace widget {
	class Label : public ewol::Widget
	{
		public:
			// Event list of properties
			static const char * const eventPressed;
			/**
			 * @brief Main call of recording the widget on the List of "widget named creator"
			 */
			static void Init(void);
			/**
			 * @brief Main call to unrecord the widget from the list of "widget named creator"
			 */
			static void UnInit(void);
		private:
			ewol::Text m_text; //!< Compositing text element.
			etk::UString m_label; //!< decorated text to display.
		public:
			/**
			 * @brief Constructor
			 * @param[in] _newLabel The displayed decorated text.
			 */
			Label(etk::UString _newLabel="---");
			/**
			 * @brief destructor
			 */
			virtual ~Label(void) { };
			/**
			 * @brief Change the label displayed
			 * @param[in] _newLabel The displayed decorated text.
			 */
			void SetLabel(const etk::UString& _newLabel);
			inline void SetValue(const etk::UString& _newLabel) { SetLabel(_newLabel); };
			/**
			 * @brief Get the current displayed label
			 * @return The displayed decorated text.
			 */
			etk::UString GetLabel(void);
			inline etk::UString GetValue(void) { return GetLabel(); };
		protected: // Derived function
			virtual void OnDraw(void);
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::Label"; };
			virtual void CalculateMinMaxSize(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual bool LoadXML(exml::Element* _node);
	};
	
};

#endif
