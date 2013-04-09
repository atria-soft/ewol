/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_WIDGET_COMPOSER_H__
#define __EWOL_WIDGET_COMPOSER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget
{
	/**
	 * @brief the composer widget is a widget that create a link on a string.file to parse the data and generate some widget tree
	 */
	class Composer : public ewol::Widget
	{
		private:
			class ComposerWidgetListNamed {
				public:
					ewol::Widget* widget;
					etk::UString widgetName;
			};
			ewol::Widget* m_subWidget;
			etk::Vector<ComposerWidgetListNamed> m_list;
		public:
			/**
			 * @brief Constructor
			 */
			Composer(void);
			/**
			 * @brief Destructor
			 */
			~Composer(void);
			/**
			 * @brief Remove all sub elements
			 */
			void Clean(void);
			/**
			 * @brief Load a composition with a file
			 * @param[in] fileName Name of the file
			 * @return true ==> all done OK
			 * @return false ==> some error occured
			 */
			bool LoadFromFile(const etk::UString& fileName);
			/**
			 * @brief Load a composition with a file
			 * @param[in] composerXmlString xml to parse directly
			 * @return true ==> all done OK
			 * @return false ==> some error occured
			 */
			bool LoadFromString(const etk::UString& composerXmlString);
		private:
			/**
			 * @brief Load a composition with a file.
			 * @param[in] data pointer on the file data.
			 * @return true ==> all done OK.
			 * @return false ==> some error occured.
			 */
			bool CommonLoadXML(const char* data);
		public:
			/**
			 * @brief Get the main node widget
			 * @return the requested pointer on the node
			 */
			ewol::Widget* GetMainWidget(void);
			/**
			 * @brief Get the main node widget
			 * @param[in] widgetName name of the widget
			 * @return the requested pointer on the node
			 */
			ewol::Widget* GetWidgetNamed(const etk::UString& widgetName);
			
		protected: // Derived function
			virtual void OnDraw(ewol::DrawProperty& displayProp);
		public:// Derived function
			virtual void OnObjectRemove(ewol::EObject* removeObject);
			virtual void CalculateSize(const vec2& availlable);
			virtual void CalculateMinSize(void);
			// TODO : Call all sub element getter an setter ==> this object might be transparent ...
	};
};

#endif
