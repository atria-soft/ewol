package org.atriasoft.ewol.event;

import java.time.Clock;
import java.time.Duration;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
public record EventTime(
		Clock currentClock, //!< Current system time "Clock.systemUTC()"
		Clock upClock, //!< Current application wake up-time "Clock.systemUTC() @ start"
		long currentTime, //!< Current system time "System.nanoTime()"
		long upTime, //!< Current application wake up-time "System.nanoTime() @ start"
		Duration timeDelta, //!< Time from the last cycle call of the system (main appl tick)
		Duration timeDeltaCall //!< Time from the last call (when we can manage periodic call with specifying periode)
	) {
	
	public float getTimeDeltaCallSecond() {
		return (float)(this.timeDeltaCall.toNanos() * 0.000000001);
	}
	public Duration getApplUpTime() {
		return Duration.ofNanos(this.currentTime-this.upTime);
	}
	
}
