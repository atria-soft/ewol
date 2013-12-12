/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_KEY_SPECIAL_H__
#define __EWOL_KEY_SPECIAL_H__

#include <etk/types.h>
#include <etk/Stream.h>

namespace ewol {
	namespace key {
		/**
		 * @brief This class consider generic special keyborad key (insert, control, shift ...)
		 */
		class Special {
			private:
				uint32_t m_value;
			public:
				/**
				 * @brief Main constructor
				 */
				Special(void);
				/**
				 * @brief get the current CapLock Status
				 * @return The CapLock value
				 */
				bool getCapsLock(void) const;
				/**
				 * @brief set the current CapLock Status
				 * @param[in] _value The new CapLock value
				 */
				void setCapsLock(bool _value);
				/**
				 * @brief Get the current Shift key status
				 * @return The Shift value
				 */
				bool getShift(void) const;
				/**
				 * @brief Set the current Shift key status
				 * @param[in] _value The new Shift value
				 */
				void setShift(bool _value);
				/**
				 * @brief Get the Current Control key status
				 * @return The Control value
				 */
				bool getCtrl(void) const;
				/**
				 * @brief Set the Current Control key status
				 * @param[in] _value The new Control value
				 */
				void setCtrl(bool _value);
				/**
				 * @brief Get the current Meta key status (also named windows or apple key)
				 * @return The Meta value (name Windows key, apple key, command key ...)
				 */
				bool getMeta(void) const;
				/**
				 * @brief Set the current Meta key status (also named windows or apple key)
				 * @param[in] _value The new Meta value (name Windows key, apple key, command key ...)
				 */
				void setMeta(bool _value);
				/**
				 * @brief Get the current Alt key status
				 * @return The Alt value
				 */
				bool getAlt(void) const;
				/**
				 * @brief Set the current Alt key status
				 * @param[in] _value The new Alt value
				 */
				void setAlt(bool _value);
				/**
				 * @brief Get the current Alt-Gr key status
				 * @return The Alt-gr value (does not exist on MacOs)
				 */
				bool getAltGr(void) const;
				/**
				 * @brief Set the current Alt-Gr key status
				 * @param[in] _value The new Alt-gr value (does not exist on MacOs)
				 */
				void setAltGr(bool _value);
				/**
				 * @brief Get the current Ver-num key status
				 * @return The Numerical Lock value
				 */
				bool getNumLock(void) const;
				/**
				 * @brief Set the current Ver-num key status
				 * @param[in] _value The new Numerical Lock value
				 */
				void setNumLock(bool _value);
				/**
				 * @brief Get the current Intert key status
				 * @return The Insert value
				 */
				bool getInsert(void) const;
				/**
				 * @brief Set the current Intert key status
				 * @param[in] _value The new Insert value
				 */
				void setInsert(bool _value);
		};
		etk::CCout& operator <<(etk::CCout& _os, const ewol::key::Special _obj);
	};
};

#endif


