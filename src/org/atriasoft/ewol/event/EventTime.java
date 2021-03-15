package org.atriasoft.ewol.event;

import org.atriasoft.echrono.Clock;
import org.atriasoft.echrono.Duration;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
public class EventTime {
	private final Clock timeSystem; //!< Current system time (micro-second)
	private final Clock timeUpAppl; //!< Current application wake up-time (micro-second)
	private final Duration timeDelta; //!< Time from the last cycle call of the system (main appl tick) (second)
	private final Duration timeDeltaCall; //!< Time from the last call (when we can manage periodic call with specifying periode) (second)
	
	public EventTime(final Clock _timeSystem, final Clock _timeUpAppl, final Duration _timeDelta, final Duration _timeDeltaCall) {
		this.timeSystem = _timeSystem;
		this.timeUpAppl = _timeUpAppl;
		this.timeDelta = _timeDelta;
		this.timeDeltaCall = _timeDeltaCall;
		
	};
	
	public Duration getApplUpTime() {
		return this.timeSystem.less(this.timeUpAppl);
	};
	
	public Clock getApplWakeUpTime() {
		return this.timeUpAppl;
	};
	
	public float getDelta() {
		return this.timeDelta.toSeconds();
	};
	
	public float getDeltaCall() {
		return this.timeDeltaCall.toSeconds();
	};
	
	public Duration getDeltaCallDuration() {
		return this.timeDeltaCall;
	};
	
	public Duration getDeltaDuration() {
		return this.timeDelta;
	};
	
	public Clock getTime() {
		return this.timeSystem;
	};
	
}
