/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_LAYER_H__
#define __EWOL_LAYER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget {
	class Layer :public ewol::Widget
	{
		public:
			static void Init(void);
			static void UnInit(void);
		public:
			Layer(void);
			virtual ~Layer(void);
			void LockExpandContamination(bool lockExpend=false);
		private:
			bool m_lockExpandContamination;
			etk::Vector<ewol::Widget*> m_subWidget;
		public:
			virtual void SubWidgetRemoveAll(void);
			virtual void SubWidgetAdd(ewol::Widget* newWidget);
			virtual void SubWidgetAddFront(ewol::Widget* newWidget);
			virtual void SubWidgetRemove(ewol::Widget* newWidget);
			virtual void SubWidgetUnLink(ewol::Widget* newWidget);
		protected:
			// Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		public:
			// Derived function
			virtual void OnRegenerateDisplay(void);
			virtual ewol::Widget * GetWidgetAtPos(const vec2& pos);
			virtual void OnObjectRemove(ewol::EObject * removeObject);
			virtual void CalculateSize(const vec2& availlable);
			virtual void CalculateMinMaxSize(void);
			virtual void SetMinSize(const vec2& size);
			virtual bvec2 CanExpand(void);
			virtual const char * const GetObjectType(void) { return "EwolLayer"; };
	};
	
};

#endif
