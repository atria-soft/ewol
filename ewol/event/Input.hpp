/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>

namespace ewol {
	namespace event {
		class Input {
			private:
				enum gale::key::type m_type;
				enum gale::key::status m_status;
				uint8_t m_inputId;
				vec2 m_pos;
				gale::key::Special m_specialKey; //!< input key status (prevent change in time..)
			public:
				Input(enum gale::key::type _type,
				      enum gale::key::status _status,
				      uint8_t _id,
				      const vec2& _pos,
				      gale::key::Special _specialKey):
				  m_type(_type),
				  m_status(_status),
				  m_inputId(_id),
				  m_pos(_pos),
				  m_specialKey(_specialKey) {
					
				};
				void setType(enum gale::key::type _type) {
					m_type = _type;
				};
				inline const enum gale::key::type& getType() const {
					return m_type;
				};
				void setStatus(enum gale::key::status _status) {
					m_status = _status;
				};
				inline const enum gale::key::status& getStatus() const {
					return m_status;
				};
				void setId(uint8_t _id) {
					m_inputId = _id;
				};
				inline const uint8_t& getId() const {
					return m_inputId;
				};
				void setPos(const vec2& _pos) {
					m_pos = _pos;
				};
				inline const vec2& getPos() const {
					return m_pos;
				};
				void setSpecialKey(const gale::key::Special& _specialKey) {
					m_specialKey = _specialKey;
				};
				inline const gale::key::Special& getSpecialKey() const {
					return m_specialKey;
				};
				/**
				 * @brief Reset the input property of the curent event.
				 */
				void reset() const {
					// TODO : Call the entry element ant rest it ...
				}
		};
		etk::Stream& operator <<(etk::Stream& _os, const ewol::event::Input& _obj);
		
		class InputSystem {
			public:
				InputSystem(enum gale::key::type _type,
				            enum gale::key::status _status,
				            uint8_t _id,
				            const vec2& _pos,
				            ewol::WidgetShared _dest,
				            int32_t _realIdEvent,
				            gale::key::Special _specialKey) :
				  m_event(_type, _status, _id, _pos, _specialKey),
				  m_dest(_dest),
				  m_realIdEvent(_realIdEvent) { };
				ewol::event::Input m_event;
			private:
				ewol::WidgetShared m_dest;
				int32_t m_realIdEvent;
			public:
				void setDestWidget(ewol::WidgetShared _dest) {
					m_dest = _dest;
				};
				inline ewol::WidgetShared getDestWidget() const {
					return m_dest;
				};
				void setRealId(int32_t _realIdEvent) {
					m_realIdEvent = _realIdEvent;
				};
				inline int32_t getRealId() const {
					return m_realIdEvent;
				};
		};
		etk::Stream& operator <<(etk::Stream& _os, const ewol::event::InputSystem& _obj);
	};
};

