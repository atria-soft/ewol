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
			void setType(ewol::keyEvent::type_te _type) { m_type = _type; };
			inline const ewol::keyEvent::type_te& getType(void) const { return m_type; };
			void setStatus(ewol::keyEvent::status_te _status) { m_status = _status; };
			inline const ewol::keyEvent::status_te& getStatus(void) const { return m_status; };
			void setId(uint8_t _id) { m_inputId = _id; };
			inline const uint8_t& getId(void) const { return m_inputId; };
			void setPos(const vec2& _pos) { m_pos = _pos; };
			inline const vec2& getPos(void) const { return m_pos; };
	};
	etk::CCout& operator <<(etk::CCout& _os, const ewol::EventInput& _obj);
	
	class EventInputSystem {
		public:
			EventInputSystem(ewol::keyEvent::type_te _type,
			                 ewol::keyEvent::status_te _status,
			                 uint8_t _id,
			                 const vec2& _pos,
			                 ewol::Widget* _dest,
			                 int32_t _realIdEvent) :
				m_event(_type, _status, _id, _pos),
				m_dest(_dest),
				m_realIdEvent(_realIdEvent)
			{ };
			ewol::EventInput m_event;
		private:
			ewol::Widget* m_dest;
			int32_t m_realIdEvent;
		public:
			void setDestWidget(ewol::Widget* _dest) { m_dest = _dest; };
			inline ewol::Widget* getDestWidget(void) const { return m_dest; };
			void setRealId(int32_t _realIdEvent) { m_realIdEvent = _realIdEvent; };
			inline int32_t getRealId(void) const { return m_realIdEvent; };
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
	etk::CCout& operator <<(etk::CCout& _os, const ewol::EventInputSystem& _obj);
};

#endif

