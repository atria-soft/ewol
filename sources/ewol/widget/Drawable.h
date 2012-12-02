/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_DRAWABLE_H__
#define __EWOL_WIDGET_DRAWABLE_H__

#include <ewol/widget/Widget.h>
#include <ewol/compositing/Compositing.h>


namespace widget {
	
	class Drawable : public ewol::Widget {
		public:
			Drawable(void);
			virtual ~Drawable(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "widget::Drawable"; };
			
		private:
			etk::Vector<ewol::Compositing*> m_listOObject;   //!< generic element to display...
		public:
			void    AddOObject(ewol::Compositing* newObject, int32_t pos=-1);
			void    ClearOObjectList(void);
		protected:
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
	};
	
};

#endif

