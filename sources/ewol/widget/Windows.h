/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WINDOWS_H__
#define __EWOL_WINDOWS_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace ewol {
	
	class Windows :public ewol::Widget
	{
		public:
			Windows(void);
			virtual ~Windows(void);
		// internal event at ewol system : 
		public:
			void SysDraw(void);
			void SysOnShow(void) {};
			void SysOnHide(void) {};
			void SysOnKill(void) {};
		public:
			virtual void OnShow(void) { };
			virtual void OnHide(void) { };
			virtual bool OnKill(void) { return true; };
			virtual void OnReduce(void) { };
			virtual void On(void) { };
		private:
			bool m_hasDecoration;
		public:
			void SetDecorationDisable(void)
			{
				m_hasDecoration = false;
			}
			
			void SetDecorationEnable(void)
			{
				m_hasDecoration = true;
			}
		private:
			ewol::Widget* m_subWidget;
			etk::Vector<ewol::Widget*> m_popUpWidgetList;
		public:
			void SetSubWidget(ewol::Widget * widget);
			void PopUpWidgetPush(ewol::Widget * widget);
		protected: // Derived function
			virtual void SystemDraw(const ewol::DrawProperty& _displayProp);
		public: // Derived function
			virtual const char * const GetObjectType(void) { return "ewol::Windows"; };
			virtual void OnRegenerateDisplay(void);
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			virtual void CalculateSize(const vec2& availlable);
			virtual ewol::Widget * GetWidgetAtPos(const vec2& pos);
	};
	
};

#endif

