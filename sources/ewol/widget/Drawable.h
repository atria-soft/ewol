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
			virtual const char * const getObjectType(void) { return "widget::drawable"; };
			
		private:
			etk::Vector<ewol::Compositing*> m_listOObject;   //!< generic element to display...
		public:
			void addOObject(ewol::Compositing* newObject, int32_t pos=-1);
			void clearOObjectList(void);
		protected: // Derived function
			virtual void onDraw(void);
	};
	
};

#endif

