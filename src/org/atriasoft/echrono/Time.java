package org.atriasoft.echrono;

/**
 * Represent the earth clock (if computer is synchronized)
 */
public class Time {
	public static Time now() {
		return new Time(System.nanoTime());
	}
	
	private final long data; //!< earth time since Epock in ns
	
	public Time() {
		this.data = 0;
	}
	
	public Time(final double _val) { //value in second
		this.data = (long) (_val * 1000000000.0);
	}
	
	public Time(final int _val) { //value in nanosecond
		this.data = _val;
	}
	
	public Time(final long _val) { //value in nanosecond
		this.data = _val;
	}
	
	public Time(final long _valSec, final long _valNano) { //value in second and nanosecond
		this.data = _valSec * 1000000000L + _valNano;
	}
	
	public long get() {
		return this.data;
	}
	
	public Clock toClock() {
		return new Clock(this.data);
	}
}
