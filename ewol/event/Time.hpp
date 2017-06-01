/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <etk/types.hpp>
#include <echrono/Clock.hpp>
#include <echrono/Duration.hpp>

namespace ewol {
	namespace event {
		class Time {
			private:
				echrono::Clock m_timeSystem; //!< Current system time (micro-second)
				echrono::Clock m_timeUpAppl; //!< Current application wake up-time (micro-second)
				echrono::Duration m_timeDelta; //!< Time from the last cycle call of the system (main appl tick) (second)
				echrono::Duration m_timeDeltaCall; //!< Time from the last call (when we can manage periodic call with specifying periode) (second)
			public:
				Time(const echrono::Clock& _timeSystem,
				     const echrono::Clock& _timeUpAppl,
				     const echrono::Duration& _timeDelta,
				     const echrono::Duration& _timeDeltaCall) :
				  m_timeSystem(_timeSystem),
				  m_timeUpAppl(_timeUpAppl),
				  m_timeDelta(_timeDelta),
				  m_timeDeltaCall(_timeDeltaCall){
					
				};
			public:
				void setTime(const echrono::Clock& _timeSystem) {
					m_timeSystem = _timeSystem;
				};
				inline const echrono::Clock& getTime() const {
					return m_timeSystem;
				};
				void setApplWakeUpTime(const echrono::Clock& _timeUpAppl) {
					m_timeUpAppl = _timeUpAppl;
				};
				inline const echrono::Clock& getApplWakeUpTime() const {
					return m_timeUpAppl;
				};
				inline echrono::Duration getApplUpTime() const {
					return m_timeSystem-m_timeUpAppl;
				};
				void setDelta(const echrono::Duration& _timeDelta) {
					m_timeDelta = _timeDelta;
				};
				inline const echrono::Duration& getDeltaDuration() const {
					return m_timeDelta;
				};
				inline float getDelta() const {
					return m_timeDelta.toSeconds();
				};
				void setDeltaCall(const echrono::Duration& _timeDeltaCall) {
					m_timeDeltaCall = _timeDeltaCall;
				};
				inline const echrono::Duration& getDeltaCallDuration() const {
					return m_timeDeltaCall;
				};
				inline float getDeltaCall() const {
					return m_timeDeltaCall.toSeconds();
				};
		};
		std::ostream& operator <<(std::ostream& _os, const ewol::event::Time& _obj);
	}
}

