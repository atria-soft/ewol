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

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the Cotainer widget is a widget that have an only one subWidget
		 */
		class Container : public ewol::Widget {
			protected:
				ewol::object::Owner<ewol::Widget> m_subWidget;
			public:
				/**
				 * @brief Constructor
				 */
				Container(ewol::object::Shared<ewol::Widget> _subElement=nullptr);
				/**
				 * @brief Destructor
				 */
				~Container();
			public:
				/**
				 * @brief get the main node widget
				 * @return the requested pointer on the node
				 */
				ewol::object::Shared<ewol::Widget> getSubWidget();
				/**
				 * @brief set the subWidget node widget.
				 * @param[in] _newWidget The widget to add.
				 */
				void setSubWidget(ewol::object::Shared<ewol::Widget> _newWidget);
				/**
				 * @brief remove the subWidget node (async).
				 */
				void subWidgetRemove();
				/**
				 * @brief Unlink the subwidget Node.
				 */
				void subWidgetUnLink();
				
			public: // Derived function
				virtual void systemDraw(const ewol::DrawProperty& _displayProp);
				virtual void onRegenerateDisplay();
				virtual void onObjectRemove(const ewol::object::Shared<ewol::Object>& _removeObject);
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
