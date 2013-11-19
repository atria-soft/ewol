/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_PROGRESS_BAR_H__
#define __EWOL_PROGRESS_BAR_H__

#include <etk/types.h>
#include <etk/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>
#include <ewol/widget/WidgetManager.h>

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 */
	class ProgressBar : public ewol::Widget {
		public:
			static void init(ewol::WidgetManager& _widgetManager);
			// Config list of properties
			static const char* const configColorBg;
			static const char* const configColorFgOn;
			static const char* const configColorFgOff;
			static const char* const configValue;
		private:
			ewol::Drawing m_draw; // basic drawing element
		public:
			ProgressBar(void);
			virtual ~ProgressBar(void);
			void setValue(float _val);
			float getValue(void) const {
				return m_value;
			};
			void setColor(etk::Color<> _newColor) {
				m_textColorFg = _newColor;
			};
		private:
			float m_value; //!< % used
			etk::Color<> m_textColorFg; //!< forder bar color
			etk::Color<> m_textColorBgOn; //!< bar color enable
			etk::Color<> m_textColorBgOff; //!< bar color disable
		protected: // Derived function
			virtual void onDraw(void);
			virtual bool onSetConfig(const ewol::EConfig& _conf);
			virtual bool onGetConfig(const char* _config, std::string& _result) const;
		public: // Derived function
			virtual void onRegenerateDisplay(void);
			virtual void calculateMinMaxSize(void);
	};
	
};

#endif
