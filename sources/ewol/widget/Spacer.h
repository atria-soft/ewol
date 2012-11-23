/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SPACER_H__
#define __EWOL_SPACER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Drawable.h>

namespace ewol {
	class Spacer :public ewol::Drawable
	{
		public:
			Spacer(void);
			virtual ~Spacer(void);
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolSpacer"; };
			virtual bool   CalculateMinSize(void);
			void SetSize(float size);
			/**
			 * @brief Get the widget at the specific windows absolute position
			 * @note the sizer return NULL, because nothing can be done inside nothing
			 * @param[in] pos gAbsolute position of the requested widget knowledge
			 * @return NULL No widget found
			 * @return pointer on the widget found
			 */
			virtual ewol::Widget * GetWidgetAtPos(etk::Vector2D<float>  pos) { return NULL; };
			virtual void   OnRegenerateDisplay(void);
			void SetColor(draw::Color newColor) { m_color = newColor; MarkToRedraw(); };
		private:
			float     m_localSize;
			draw::Color  m_color;
	};
	
};

#endif
