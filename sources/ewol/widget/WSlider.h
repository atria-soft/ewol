/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_W_SLIDER_H__
#define __EWOL_W_SLIDER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget {
	class WSlider :public ewol::Widget
	{
		public:
			WSlider(void);
			virtual ~WSlider(void);
			// Derived function
			virtual const char * const GetObjectType(void) { return "EwolWSlider"; };
		private:
			etk::Vector2D<bool> m_underExpend; // expend of the uner elements ...
		public:
			// Derived function
			virtual bool   CalculateSize(float availlableX, float availlableY);
			// Derived function
			virtual bool   CalculateMinSize(void);
			// Derived function
			virtual void   SetMinSise(float x=-1, float y=-1);
			// Derived function
			virtual bool   CanExpentX(void);
			// Derived function
			virtual bool   CanExpentY(void);
			void           LockExpendContamination(bool lockExpend=false);
		private:
			bool                           m_lockExpendContamination;
			etk::Vector<ewol::Widget*>     m_subWidget;
			int32_t                        m_windowsSources;     // widget source viewed
			int32_t                        m_windowsDestination; // widget destinated viewed
			int32_t                        m_slidingProgress;    // ratio progression of a sliding
		public:
			virtual void    SubWidgetRemoveAll(void);
			virtual void    SubWidgetAdd(ewol::Widget* newWidget);
			virtual void    SubWidgetRemove(ewol::Widget* newWidget);
			virtual void    SubWidgetUnLink(ewol::Widget* newWidget);
			        void    SubWidgetSelectSet(int32_t id);
			        int32_t SubWidgetSelectGet(void) { return (int32_t)m_slidingProgress; };
			        int32_t SubWidgetNumber(void) { return m_subWidget.Size(); };
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
			// Derived function
			virtual void PeriodicCall(int64_t localTime);
	};
	
};

#endif
