/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ewol/signal/Interface.h>

namespace ewol {
	namespace signal {
		class Base {
			protected:
				#ifdef DEBUG
					static int32_t m_uidSignal;
					static int32_t m_signalCallLevel;
				#endif
				ewol::signal::Interface& m_signalInterfaceLink;
				std::string m_name;
				std::string m_description;
				int32_t m_callInProgress;
				bool m_someOneRemoveInCall;
				bool m_periodic;
			public:
				/**
				 * @brief Create a parameter with a specific type.
				 * @param[in] _signalInterfaceLink reference on the signal list.
				 * @param[in] _name Static name of the parameter.
				 * @param[in] _description description of the parameter.
			 * @param[in] _periodic Customisation of the log display tag at true to down debug lebel at verbose.
				 */
				Base(ewol::signal::Interface& _signalInterfaceLink,
				     const std::string& _name,
				     const std::string& _description = "",
				     bool _periodic = false);
				/**
				 * @brief Destructor.
				 */
				virtual ~Base() { };
				
				const std::string& getName() const {
					return m_name;
				}
				const std::string& getDescription() const {
					return m_description;
				}
				virtual void release(std::shared_ptr<void> _obj) = 0;
		};
		std::ostream& operator <<(std::ostream& _os, const ewol::signal::Base& _obj);
		const char* logIndent(int32_t _iii);
	};
};
