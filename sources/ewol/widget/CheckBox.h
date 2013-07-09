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

extern const char* const ewolEventCheckBoxClicked;

namespace widget {
	class CheckBox : public ewol::Widget
	{
		public:
			static void Init(void);
			static void UnInit(void);
		public:
			CheckBox(const etk::UString& newLabel = "No Label");
			virtual ~CheckBox(void);
			void SetLabel(etk::UString newLabel);
			void SetValue(bool val);
			bool GetValue(void);
		private:
			ewol::Text m_oObjectText;
			ewol::Drawing m_oObjectDecoration;
			etk::UString m_label;
			bool m_value;
			etk::Color<> m_textColorFg; //!< Text color
			etk::Color<> m_textColorBg; //!< Background color
		protected: // Derived function
			virtual void OnDraw(void);
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "Ewol::CheckBox"; };
			virtual void CalculateMinMaxSize(void);
			virtual void OnRegenerateDisplay(void);
			virtual bool OnEventInput(const ewol::EventInput& _event);
			virtual bool OnEventEntry(const ewol::EventEntry& _event);
	};
	
};

#endif
