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
	
	public Time(final double val) { //value in second
		this.data = (long) (val * 1000000000.0);
	}
	
	public Time(final int val) { //value in nanosecond
		this.data = val;
	}
	
	public Time(final long val) { //value in nanosecond
		this.data = val;
	}
	
	public Time(final long valSec, final long valNano) { //value in second and nanosecond
		this.data = valSec * 1000000000L + valNano;
	}
	
	public long get() {
		return this.data;
	}
	
	public Clock toClock() {
		return new Clock(this.data);
	}
}
