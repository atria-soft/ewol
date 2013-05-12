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
#include <draw/Color.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>
#include <ewol/compositing/Drawing.h>


namespace widget {
	class ProgressBar : public ewol::Widget
	{
		public:
			static void Init(void);
			static void UnInit(void);
		private:
			ewol::Drawing m_draw; // basic drawing element
		public:
			ProgressBar(void);
			virtual ~ProgressBar(void);
			void ValueSet(float val);
			float ValueGet(void);
			void SetColor(draw::Color newColor) { m_textColorFg = newColor; };
		private:
			float m_value; //!< % used
			draw::Color m_textColorFg; //!< forder bar color
			draw::Color m_textColorBgOn; //!< bar color enable
			draw::Color m_textColorBgOff; //!< bar color disable
		protected: // Derived function
			virtual void OnDraw(void);
		public: // Derived function
			virtual void OnRegenerateDisplay(void);
			virtual const char * const GetObjectType(void) { return "EwolProgressBar"; };
			virtual void CalculateMinMaxSize(void);
	};
	
};

#endif
