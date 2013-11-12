/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_EVENT_ENTRY_H__
#define __EWOL_EVENT_ENTRY_H__

#include <etk/types.h>
#include <ewol/key.h>

namespace ewol {
	class EventEntry {
		private:
			enum ewol::keyEvent::keyboard m_type; //!< type of hardware event
			enum ewol::keyEvent::status m_status; //!< status of hardware event
			ewol::SpecialKey m_specialKey; //!< input key status (prevent change in time..)
			char32_t m_unicodeData; //!< Unicode data (in some case)
		public:
			EventEntry(enum ewol::keyEvent::keyboard _type,
			           enum ewol::keyEvent::status _status,
			           ewol::SpecialKey _specialKey,
			           char32_t _char) :
				m_type(_type),
				m_status(_status),
				m_specialKey(_specialKey),
				m_unicodeData(_char)
			{ };
			void setType(enum ewol::keyEvent::keyboard _type) {
				m_type = _type;
			};
			inline const enum ewol::keyEvent::keyboard& getType(void) const {
				return m_type;
			};
			void setStatus(enum ewol::keyEvent::status _status) {
				m_status = _status;
			};
			inline const enum ewol::keyEvent::status& getStatus(void) const {
				return m_status;
			};
			void setSpecialKey(const ewol::SpecialKey& _specialKey) {
				m_specialKey = _specialKey;
			};
			inline const ewol::SpecialKey& getSpecialKey(void) const {
				return m_specialKey;
			};
			void setChar(char32_t _char) {
				m_unicodeData = _char;
			};
			inline const char32_t& getChar(void) const {
				return m_unicodeData;
			};
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::EventEntry& _obj);
	
	class EventEntrySystem {
		public:
			EventEntrySystem(enum ewol::keyEvent::keyboard _type,
			                 enum ewol::keyEvent::status _status,
			                 ewol::SpecialKey _specialKey,
			                 char32_t _char) :
				m_event(_type, _status, _specialKey, _char)
			{ };
			ewol::EventEntry m_event;
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::EventEntrySystem& _obj);
};

#endif

