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
	class EventInput {
		private:
			ewol::keyEvent::type_te m_type;
			ewol::keyEvent::status_te m_status;
			uint8_t m_inputId;
			vec2 m_pos;
		public:
			EventInput(ewol::keyEvent::type_te _type,
			           ewol::keyEvent::status_te _status,
			           uint8_t _id,
			           const vec2& _pos):
				m_type(_type),
				m_status(_status),
				m_inputId(_id),
				m_pos(_pos)
			{ };
			void SetType(ewol::keyEvent::type_te _type) { m_type = _type; };
			inline const ewol::keyEvent::type_te& GetType(void) const { return m_type; };
			void SetStatus(ewol::keyEvent::status_te _status) { m_status = _status; };
			inline const ewol::keyEvent::status_te& GetStatus(void) const { return m_status; };
			void SetId(uint8_t _id) { m_inputId = _id; };
			inline const uint8_t& GetId(void) const { return m_inputId; };
			void SetPos(const vec2& _pos) { m_pos = _pos; };
			inline const vec2& GetPos(void) const { return m_pos; };
	};
	
	class EventInputSystem {
		public:
			EventInputSystem(ewol::keyEvent::type_te _type,
			                 ewol::keyEvent::status_te _status,
			                 uint8_t _id,
			                 const vec2& _pos) :
				m_event(_type, _status, _id, _pos)
			{ };
			ewol::EventInput m_event;
		/*
		private:
			int64_t m_lastTime;
			uint8_t m_systemId;
			uint8_t m_id;
			uint8_t m_numberClick;
			bool m_isUsed;
			bool m_isDown;
			bool m_isInside;
			ewol::Widget* m_widget;
			vec2 m_downStart;
			vec2 m_pos;
		*/
	};
};

#endif

