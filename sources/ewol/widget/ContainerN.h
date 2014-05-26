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
#include <list>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class ContainerN : public ewol::Widget {
			protected:
				std::list<ewol::object::Owner<ewol::Widget>> m_subWidget;
			public:
				/**
				 * @brief Constructor
				 */
				ContainerN();
				/**
				 * @brief Destructor
				 */
				~ContainerN();
			protected:
				bvec2 m_lockExpand; //!< Lock the expend of the sub widget to this one  == > this permit to limit bigger subWidget
				bvec2 m_subExpend; //!< reference of the sub element expention requested.
			public:
				/**
				 * @brief Limit the expend properties to the current widget (no contamination)
				 * @param[in] _lockExpend Lock mode of the expend properties
				 */
				void lockExpand(const bvec2& _lockExpand);
				// herited function
				virtual bvec2 canExpand();
			public:
				/**
				 * @brief remove all sub element from the widget.
				 */
				virtual void subWidgetRemoveAll();
				/**
				 * @brief remove all sub element from the widget (delayed to prevent remove in the callbback).
				 */
				virtual void subWidgetRemoveAllDelayed();
				/**
				 * @brief add at end position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _newWidget the element pointer
				 * @return the ID of the set element
				 */
				virtual int32_t subWidgetAdd(ewol::object::Shared<ewol::Widget> _newWidget);
				//! @previous
				inline  int32_t subWidgetAddBack(ewol::object::Shared<ewol::Widget> _newWidget) {
					return subWidgetAdd(_newWidget);
				};
				//! @previous
				inline  int32_t subWidgetAddEnd(ewol::object::Shared<ewol::Widget> _newWidget) {
					return subWidgetAdd(_newWidget);
				};
				/**
				 * @brief add at start position a Widget (note : This system use an inverted phylisophie (button to top, and left to right)
				 * @param[in] _newWidget the element pointer
				 * @return the ID of the set element
				 */
				virtual int32_t subWidgetAddStart(ewol::object::Shared<ewol::Widget> _newWidget);
				//! @previous
				inline  int32_t subWidgetAddFront(ewol::object::Shared<ewol::Widget> _newWidget) {
					return subWidgetAddStart(_newWidget);
				};
				/**
				 * @brief remove definitly a widget from the system and this layer.
				 * @param[in] _newWidget the element pointer.
				 */
				virtual void subWidgetRemove(ewol::object::Shared<ewol::Widget> _newWidget);
				/**
				 * @brief Just unlick the specify widget, this function does not remove it from the system (if you can, do nt use it ...)
				 * @param[in] _newWidget the element pointer.
				 */
				virtual void subWidgetUnLink(ewol::object::Shared<ewol::Widget> _newWidget);
			public:// Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual void onObjectRemove(const ewol::object::Shared<ewol::Object>& _object);
				virtual void calculateSize(const vec2& _availlable);
				virtual void calculateMinMaxSize();
				virtual ewol::object::Shared<ewol::Widget> getWidgetAtPos(const vec2& _pos);
				virtual ewol::object::Shared<ewol::Widget> getWidgetNamed(const std::string& _widgetName);
				virtual bool loadXML(exml::Element* _node);
				virtual void setOffset(const vec2& _newVal);
		};
	};
};

#endif
