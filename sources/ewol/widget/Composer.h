/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_XML_COMPOSER_H__
#define __EWOL_XML_COMPOSER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Widget.h>

namespace widget
{
	class XmlComposer : public ewol::EObject
	{
		private:
			class ComposerWidgetListNamed {
				public:
					ewol::Widget* widget;
					etk::UString widgetName;
			}
			ewol::Widget* m_widget;
			etk::UString composerString;
			etk::Vector<ComposerWidgetListNamed> m_list;
		public:
			/**
			 * @brief Constructor
			 */
			XmlComposer(const etk::UString& composerXmlString);
			/**
			 * @brief Destructor
			 */
			~XmlComposer(void);
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
			
		public:
			// herited function :
			virtual void OnObjectRemove(ewol::EObject* removeObject);
	};
};

#endif
