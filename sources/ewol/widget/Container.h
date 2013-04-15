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

namespace widget
{
	/**
	 * @brief the Cotainer widget is a widget that have an only one subWidget
	 */
	class Container : public ewol::Widget
	{
		protected:
			ewol::Widget* m_subWidget;
		public:
			/**
			 * @brief Constructor
			 */
			Container(ewol::Widget* subElement=NULL);
			/**
			 * @brief Destructor
			 */
			~Container(void);
		public:
			/**
			 * @brief Get the main node widget
			 * @return the requested pointer on the node
			 */
			ewol::Widget* GetSubWidget(void);
			/**
			 * @brief Set the subWidget node widget.
			 * @param[in] newWidget The widget to Add.
			 */
			void SetSubWidget(ewol::Widget* newWidget);
			/**
			 * @brief Remove the subWidget node.
			 */
			void SubWidgetRemove(void);
			
		protected: // Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		public:// Derived function
			virtual void OnRegenerateDisplay(void);
			virtual void OnObjectRemove(ewol::EObject* removeObject);
			virtual void CalculateSize(const vec2& availlable);
			virtual void CalculateMinMaxSize(void);
			virtual ewol::Widget* GetWidgetAtPos(const vec2& pos);
			virtual ewol::Widget* GetWidgetNamed(const etk::UString& widgetName);
			virtual const char * const GetObjectType(void) { return "Ewol::Container"; };
			virtual bool LoadXML(TiXmlNode* node);
	};
};

#endif
