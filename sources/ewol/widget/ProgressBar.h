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
#include <ewol/widget/Drawable.h>


namespace widget {
	class ProgressBar :public widget::Drawable
	{
		public:
			static void Init(void);
			static void UnInit(void);
		public:
			ProgressBar(void);
			virtual ~ProgressBar(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolProgressBar"; };
			virtual void CalculateMinSize(void);
			void ValueSet(float val);
			float ValueGet(void);
			void SetColor(draw::Color newColor) { m_textColorFg = newColor; };
		private:
			float m_value; //!< % used
			draw::Color m_textColorFg; //!< forder bar color
			draw::Color m_textColorBgOn; //!< bar color enable
			draw::Color m_textColorBgOff; //!< bar color disable
		public:
			// Derived function
			virtual void OnRegenerateDisplay(void);
	};
	
};

#endif
