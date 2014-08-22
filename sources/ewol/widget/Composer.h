/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_WIDGET_COMPOSER_H__
#define __EWOL_WIDGET_COMPOSER_H__

#include <etk/types.h>
#include <ewol/debug.h>
#include <ewol/widget/Container.h>

namespace ewol {
	namespace widget {
		/**
		 * @ingroup ewolWidgetGroup
		 * @brief the composer widget is a widget that create a link on a string.file to parse the data and generate some widget tree
		 */
		class Composer : public ewol::widget::Container {
			public:
				enum composerMode {
					None,
					String,
					file
				};
			protected:
				/**
				 * @brief Constructor
				 */
				Composer();
				/**
				 * @brief Constructor
				 * @param[in] _mode mode of parsing the string
				 * @param[in] _data file/directString data to generate compositing of the widget..
				 */
				void init(enum composerMode _mode = ewol::widget::Composer::None, const std::string& _data = "");
			public:
				DECLARE_WIDGET_FACTORY(Composer, "Composer");
				/**
				 * @brief Destructor
				 */
				virtual ~Composer();
				/**
				 * @brief load a composition with a file
				 * @param[in] _fileName Name of the file
				 * @return true  == > all done OK
				 * @return false  == > some error occured
				 */
				bool loadFromFile(const std::string& _fileName);
				/**
				 * @brief load a composition with a file
				 * @param[in] _composerXmlString xml to parse directly
				 * @return true  == > all done OK
				 * @return false  == > some error occured
				 */
				bool loadFromString(const std::string& _composerXmlString);
			protected:
				// TODO : Create a template ...
				#define composerBind(_type, _name, _event, _obj, _func) do {\
					std::shared_ptr<_type> myObject = std::dynamic_pointer_cast<_type>(getObjectNamed(_name)); \
					if (myObject != nullptr) { \
						myObject->_event.bind(_obj, _func); \
					} \
				} while (false)
				/*
				template<class TYPE> void bind(std::shared_ptr<ewol::Object> _obj, void (TYPE::*_func)()) {
					std::shared_ptr<TYPE> obj2 = std::dynamic_pointer_cast<TYPE>(_obj);
					if (obj2 == nullptr) {
						EWOL_ERROR("Can not bind signal ...");
						return;
					}
					m_callerList.push_back(std::make_pair(std::weak_ptr<ewol::Object>(_obj), std::bind(_func, obj2.get())));
				}
				*/
				/**
				 * @brief Register an Event an named widget. @see registerOnEvent
				 * @param[in] _subWidgetName Name of the subWidget.
				 * @param[in] _eventId Event generate inside the object.
				 * @param[in] _eventIdgenerated event generated when call the distant EObject.onReceiveMessage(...)
				 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
				 * @note : To used when herited from this object.
				 */
				void registerOnEventNameWidget(const std::string& _subWidgetName,
				                               const char * _eventId,
				                               const char * _eventIdgenerated = nullptr,
				                               const std::string& _overloadData="") __attribute__ ((deprecated));
			public:
				#define composerExternBind(_composer, _type, _name, _event, _obj, _func) do {\
					std::shared_ptr<_type> myObject = std::dynamic_pointer_cast<_type>(_composer->getObjectNamed(_name)); \
					if (myObject != nullptr) { \
						myObject->_event.bind(_obj, _func); \
					} \
				} while (false)
				/**
				 * @brief Register an Event an named widget. @see registerOnEvent
				 * @param[in] _destinationObject pointer on the object that might be call when an event is generated
				 * @param[in] _subWidgetName Name of the subWidget.
				 * @param[in] _eventId Event generate inside the object.
				 * @param[in] _eventIdgenerated event generated when call the distant EObject.onReceiveMessage(...)
				 * @param[in] _overloadData When the user prever to receive a data specificly for this event ...
				 * @note : To used when NOT herited from this object.
				 */
				void registerOnEventNameWidget(const std::shared_ptr<ewol::Object>& _destinationObject,
				                               const std::string& _subWidgetName,
				                               const char * _eventId,
				                               const char * _eventIdgenerated = nullptr,
				                               const std::string& _overloadData="") __attribute__ ((deprecated));
		};
	};
};

#endif
