/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_SIZER_HORI_H__
#define __EWOL_SIZER_HORI_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget {
	class SizerHori :public ewol::Widget
	{
		public:
			SizerHori(void);
			virtual ~SizerHori(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolSizerHori"; };
		public:
			// Derived function
			virtual bool   CalculateSize(float availlableX, float availlableY);
			// Derived function
			virtual bool   CalculateMinSize(void);
			// Derived function
			virtual void   SetMinSize(float x=-1, float y=-1);
			// Derived function
			virtual void   SetExpendX(bool newExpend=false);
			// Derived function
			virtual bool   CanExpentX(void);
			// Derived function
			virtual void   SetExpendY(bool newExpend=false);
			// Derived function
			virtual bool   CanExpentY(void);
			void           LockExpendContamination(bool lockExpend=false);
		private:
			bool                           m_lockExpendContamination;
			etk::Vector<ewol::Widget*> m_subWidget;
		public:
			virtual void   SubWidgetRemoveAll(void);
			virtual void   SubWidgetAdd(ewol::Widget* newWidget);
			virtual void   SubWidgetRemove(ewol::Widget* newWidget);
			virtual void   SubWidgetUnLink(ewol::Widget* newWidget);
		protected:
			// Derived function
			virtual void   OnDraw(ewol::DrawProperty& displayProp);
		public:
			// Derived function
			virtual void   OnRegenerateDisplay(void);
			// Derived function
			virtual ewol::Widget * GetWidgetAtPos(etk::Vector2D<float>  pos);
			// Derived function
			virtual void OnObjectRemove(ewol::EObject * removeObject);
	};
	
};

#endif
