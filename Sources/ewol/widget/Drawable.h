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


namespace ewol {
	
	class Drawable : public ewol::Widget {
		public:
			Drawable(void);
			virtual ~Drawable(void);
			
			/**
			 * @brief Get the current Object type of the EObject
			 * @note In Embended platforme, it is many time no -rtti flag, then it is not possible to use dynamic cast ==> this will replace it
			 * @param[in] objectType type description
			 * @return true if the object is compatible, otherwise false
			 */
			virtual const char * const GetObjectType(void) { return "EwolDrawable"; };
			
		private:
			etk::Vector<ewol::OObject*> m_listOObject;   //!< generic element to display...
		public:
			void    AddOObject(ewol::OObject* newObject, int32_t pos=-1);
			void    ClearOObjectList(void);
		protected:
			virtual void OnDraw(DrawProperty& displayProp);
	};
	
};

#endif

