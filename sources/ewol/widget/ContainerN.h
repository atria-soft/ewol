/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_CONTAINER_N_H__
#define __EWOL_WIDGET_CONTAINER_N_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget
{
	/**
	 * @brief the Cotainer widget is a widget that have an only one subWidget
	 */
	class ContainerN : public ewol::Widget
	{
		protected:
			etk::Vector<ewol::Widget*> m_subWidget;
		public:
			/**
			 * @brief Constructor
			 */
			ContainerN(void);
			/**
			 * @brief Destructor
			 */
			~ContainerN(void);
		protected:
			bvec2 m_lockExpand; //!< Lock the expend of the sub widget to this one ==> this permit to limit bigger subWidget
			bvec2 m_subExpend; //!< reference of the sub element expention requested.
		public:
			/**
			 * @brief Limit the expend properties to the current widget (no contamination)
			 * @param[in] lockExpend Lock mode of the expend properties
			 */
			void LockExpand(const bvec2& lockExpand);
			// herited function
			virtual bvec2 CanExpand(void);
		public:
			/**
			 * @brief Remove all sub element from the widget.
			 */
			virtual void SubWidgetRemoveAll(void);
			/**
			 * @brief Add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
			 * @param[in] newWidget the element pointer
			 */
			virtual void SubWidgetAdd(ewol::Widget* newWidget);
			inline  void SubWidgetAddBack(ewol::Widget* newWidget) { SubWidgetAdd(newWidget); };
			inline  void SubWidgetAddEnd(ewol::Widget* newWidget)  { SubWidgetAdd(newWidget); };
			/**
			 * @brief Add at start position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
			 * @param[in] newWidget the element pointer
			 */
			virtual void SubWidgetAddStart(ewol::Widget* newWidget);
			inline  void SubWidgetAddFront(ewol::Widget* newWidget) { SubWidgetAddStart(newWidget); };
			/**
			 * @brief Remove definitly a widget from the system and this layer.
			 * @param[in] newWidget the element pointer.
			 */
			virtual void SubWidgetRemove(ewol::Widget* newWidget);
			/**
			 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...)
			 * @param[in] newWidget the element pointer.
			 */
			virtual void SubWidgetUnLink(ewol::Widget* newWidget);
		protected: // Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		public:// Derived function
			virtual void OnRegenerateDisplay(void);
			virtual void OnObjectRemove(ewol::EObject* removeObject);
			virtual void CalculateSize(const vec2& availlable);
			virtual void CalculateMinMaxSize(void);
			virtual ewol::Widget* GetWidgetAtPos(const vec2& pos);
			virtual ewol::Widget* GetWidgetNamed(const etk::UString& widgetName);
			virtual const char * const GetObjectType(void) { return "Ewol::ContainerN"; };
			virtual bool LoadXML(TiXmlNode* node);
	};
};

#endif
