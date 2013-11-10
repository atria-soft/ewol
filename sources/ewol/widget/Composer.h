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
#include <ewol/widget/Container.h>

namespace widget
{
	/**
	 * @ingroup ewolWidgetGroup
	 * @brief the composer widget is a widget that create a link on a string.file to parse the data and generate some widget tree
	 */
	class Composer : public widget::Container
	{
		public:
			enum composerMode {
				None,
				String,
				file
			};
		public:
			/**
			 * @brief Constructor
			 */
			Composer(void);
			/**
			 * @brief Constructor
			 * @param[in] _mode mode of parsing the string
			 * @param[in] _data file/directString data to generate compositing of the widget..
			 */
			Composer(enum composerMode _mode, const etk::UString& _data);
			/**
			 * @brief Destructor
			 */
			~Composer(void);
			/**
			 * @brief load a composition with a file
			 * @param[in] _fileName Name of the file
			 * @return true  == > all done OK
			 * @return false  == > some error occured
			 */
			bool loadFromFile(const etk::UString& _fileName);
			/**
			 * @brief load a composition with a file
			 * @param[in] _composerXmlString xml to parse directly
			 * @return true  == > all done OK
			 * @return false  == > some error occured
			 */
			bool loadFromString(const etk::UString& _composerXmlString);
			/**
			 * @brief Register an Event an named widget. @see registerOnEvent
			 * @param[in] _subWidgetName Name of the subWidget.
			 * @param[in] _eventId Event generate inside the object.
			 * @param[in] _eventIdgenerated event generated when call the distant EObject.onReceiveMessage(...)
			 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
			 * @note : To used when herited from this object.
			 */
			void registerOnEventNameWidget(const etk::UString& _subWidgetName,
			                               const char * _eventId,
			                               const char * _eventIdgenerated = NULL,
			                               const etk::UString& _overloadData="");
			/**
			 * @brief Register an Event an named widget. @see registerOnEvent
			 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
			 * @param[in] _subWidgetName Name of the subWidget.
			 * @param[in] _eventId Event generate inside the object.
			 * @param[in] _eventIdgenerated event generated when call the distant EObject.onReceiveMessage(...)
			 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
			 * @note : To used when NOT herited from this object.
			 */
			void registerOnEventNameWidget(ewol::EObject * _destinationObject,
			                               const etk::UString& _subWidgetName,
			                               const char * _eventId,
			                               const char * _eventIdgenerated = NULL,
			                               const etk::UString& _overloadData="");
		public: // herited functions:
			virtual const char * const getObjectType(void) {
				return "widget::Composer";
			};
	};
};

#endif
