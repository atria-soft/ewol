/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_CONTAINER_H__
#define __EWOL_WIDGET_CONTAINER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget {
	/**
	 * @ingroup ewolWidgetGroup
	 * @brief the Cotainer widget is a widget that have an only one subWidget
	 */
	class Container : public ewol::Widget {
		protected:
			ewol::Widget* m_subWidget;
		public:
			/**
			 * @brief Constructor
			 */
			Container(ewol::Widget* _subElement=NULL);
			/**
			 * @brief Destructor
			 */
			~Container(void);
		public:
			/**
			 * @brief get the main node widget
			 * @return the requested pointer on the node
			 */
			ewol::Widget* getSubWidget(void);
			/**
			 * @brief set the subWidget node widget.
			 * @param[in] _newWidget The widget to add.
			 */
			void setSubWidget(ewol::Widget* _newWidget);
			/**
			 * @brief remove the subWidget node.
			 */
			void subWidgetRemove(void);
			/**
			 * @brief remove the subWidget node (delayed to prevent remove in the callbback).
			 */
			virtual void subWidgetRemoveDelayed(void);
			
		public: // Derived function
			virtual void systemDraw(const ewol::DrawProperty& _displayProp);
			virtual void onRegenerateDisplay(void);
			virtual void onObjectRemove(ewol::EObject* _removeObject);
			virtual void calculateSize(const vec2& _availlable);
			virtual void calculateMinMaxSize(void);
			virtual ewol::Widget* getWidgetAtPos(const vec2& _pos);
			virtual ewol::Widget* getWidgetNamed(const etk::UString& _widgetName);
			virtual const char * const getObjectType(void) {
				return "widget::Container";
			};
			virtual bool loadXML(exml::Element* _node);
			virtual void setOffset(const vec2& _newVal);
	};
};

#endif
