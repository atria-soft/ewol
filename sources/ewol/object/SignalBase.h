/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_SIGNAL_BASE_H__
#define __EWOL_SIGNAL_BASE_H__

#include <ewol/object/SignalList.h>
#include <ewol/object/SignalBase.h>
#include <ewol/object/Object.h>


namespace ewol {
	namespace object {
		class SignalBase {
			protected:
				ewol::object::SignalList& m_objectLink;
				std::string m_name;
				std::string m_description;
			public:
				/**
				 * @brief Create a parameter with a specific type.
				 * @param[in] _objectLink reference on the parameter lister.
				 * @param[in] _name Static name of the parameter.
				 * @param[in] _description description of the parameter.
				 */
				SignalBase(ewol::object::SignalList& _objectLink,
				           const std::string& _name,
				           const std::string& _description = "");
				/**
				 * @brief Destructor.
				 */
				virtual ~SignalBase() { };
				
				const std::string& getName() const {
					return m_name;
				}
				const std::string& getDescription() const {
					return m_description;
				}
				virtual void connect(std::shared_ptr<ewol::Object> _obj, const char* _destId=nullptr, const std::string& _data="" ) = 0;
				virtual void release(std::shared_ptr<ewol::Object> _obj) = 0;
		};
		std::ostream& operator <<(std::ostream& _os, const SignalBase& _obj);
	};
};
#endif
