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
#include <ewol/key/key.h>

namespace ewol {
	namespace event {
		class Entry {
			private:
				enum ewol::key::keyboard m_type; //!< type of hardware event
				enum ewol::key::status m_status; //!< status of hardware event
				ewol::key::Special m_specialKey; //!< input key status (prevent change in time..)
				char32_t m_unicodeData; //!< Unicode data (in some case)
			public:
				Entry(enum ewol::key::keyboard _type,
				      enum ewol::key::status _status,
				      ewol::key::Special _specialKey,
				      char32_t _char) :
				  m_type(_type),
				  m_status(_status),
				  m_specialKey(_specialKey),
				  m_unicodeData(_char) {
					
				};
				void setType(enum ewol::key::keyboard _type) {
					m_type = _type;
				};
				inline const enum ewol::key::keyboard& getType(void) const {
					return m_type;
				};
				void setStatus(enum ewol::key::status _status) {
					m_status = _status;
				};
				inline const enum ewol::key::status& getStatus(void) const {
					return m_status;
				};
				void setSpecialKey(const ewol::key::Special& _specialKey) {
					m_specialKey = _specialKey;
				};
				inline const ewol::key::Special& getSpecialKey(void) const {
					return m_specialKey;
				};
				void setChar(char32_t _char) {
					m_unicodeData = _char;
				};
				inline const char32_t& getChar(void) const {
					return m_unicodeData;
				};
		};
		etk::CCout& operator <<(etk::CCout& _os, const ewol::event::Entry& _obj);
		
		class EntrySystem {
			public:
				EntrySystem(enum ewol::key::keyboard _type,
				            enum ewol::key::status _status,
				            ewol::key::Special _specialKey,
				            char32_t _char) :
				  m_event(_type, _status, _specialKey, _char) {
					
				};
				ewol::event::Entry m_event;
		};
		etk::CCout& operator <<(etk::CCout& _os, const ewol::event::EntrySystem& _obj);
	};
};

#endif

