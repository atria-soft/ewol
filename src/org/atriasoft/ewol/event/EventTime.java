package org.atriasoft.ewol.event;

import org.atriasoft.echrono.Clock;
import org.atriasoft.echrono.Duration;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
public record EventTime(
	Clock timeSystem, //!< Current system time (micro-second)
	Clock timeUpAppl, //!< Current application wake up-time (micro-second)
	Duration timeDelta, //!< Time from the last cycle call of the system (main appl tick) (second)
	Duration timeDeltaCall //!< Time from the last call (when we can manage periodic call with specifying periode) (second)
	) {
	
	public float getTimeDeltaCallSecond() {
		return this.timeDeltaCall.toSeconds();
	}
	public Duration getApplUpTime() {
		return this.timeSystem.less(this.timeUpAppl);
	};
	
}
