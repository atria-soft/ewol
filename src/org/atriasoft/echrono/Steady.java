package org.atriasoft.echrono;

/**
 * @brief Steady is a Program start time clock
 */
public class Steady {
	private final long data; //!< Monotonic clock since computer start (ns)
	
	public Steady() {
		this.data = 0;
	}
	
	public Steady(final double _val) { //value in second
		this.data = (long) (_val * 1000000000.0);
	}
	
	public Steady(final int _val) { //value in nanosecond
		this.data = _val;
	}
	
	public Steady(final long _val) { //value in nanosecond
		this.data = _val;
	}
	
	public Steady(final long _valSec, final long _valNano) { //value in second and nanosecond
		this.data = _valSec * 1000000000L + _valNano;
	}
	
	public long get() {
		return this.data;
	}
}
