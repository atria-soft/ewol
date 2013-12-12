/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_EVENT_INPUT_H__
#define __EWOL_EVENT_INPUT_H__

#include <etk/types.h>

namespace ewol {
	namespace event {
		class Input {
			private:
				enum ewol::key::type m_type;
				enum ewol::key::status m_status;
				uint8_t m_inputId;
				vec2 m_pos;
			public:
				Input(enum ewol::key::type _type,
				      enum ewol::key::status _status,
				      uint8_t _id,
				      const vec2& _pos):
				  m_type(_type),
				  m_status(_status),
				  m_inputId(_id),
				  m_pos(_pos){
					
				};
				void setType(enum ewol::key::type _type) {
					m_type = _type;
				};
				inline const enum ewol::key::type& getType(void) const {
					return m_type;
				};
				void setStatus(enum ewol::key::status _status) {
					m_status = _status;
				};
				inline const enum ewol::key::status& getStatus(void) const {
					return m_status;
				};
				void setId(uint8_t _id) {
					m_inputId = _id;
				};
				inline const uint8_t& getId(void) const {
					return m_inputId;
				};
				void setPos(const vec2& _pos) {
					m_pos = _pos;
				};
				inline const vec2& getPos(void) const {
					return m_pos;
				};
		};
		
		class InputSystem {
			public:
				InputSystem(enum ewol::key::type _type,
				            enum ewol::key::status _status,
				            uint8_t _id,
				            const vec2& _pos,
				            ewol::Widget* _dest,
				            int32_t _realIdEvent) :
				  m_event(_type, _status, _id, _pos),
				  m_dest(_dest),
				  m_realIdEvent(_realIdEvent) { };
				ewol::event::Input m_event;
			private:
				ewol::Widget* m_dest;
				int32_t m_realIdEvent;
			public:
				void setDestWidget(ewol::Widget* _dest) {
					m_dest = _dest;
				};
				inline ewol::Widget* getDestWidget(void) const {
					return m_dest;
				};
				void setRealId(int32_t _realIdEvent) {
					m_realIdEvent = _realIdEvent;
				};
				inline int32_t getRealId(void) const {
					return m_realIdEvent;
				};
		};
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::event::Input& _obj);
	etk::CCout& operator <<(etk::CCout& _os, const ewol::event::InputSystem& _obj);
};

#endif

