package org.atriasoft.echrono;

/**
 * Clock is a compleate virtual clock that is used to virtualize the urrent clock used (can be non real-time, ex:for simulation)
 */
public class Clock {
	public static Clock now() {
		return new Clock(System.nanoTime());
	}
	
	private final long data; //!< virtual clock
	
	public Clock() {
		this.data = 0;
	}
	
	public Clock(final double val) { //value in second
		this.data = (long) (val * 1000000000.0);
	}
	
	public Clock(final int val) { //value in nanosecond
		this.data = val;
	}
	
	public Clock(final long val) { //value in nanosecond
		this.data = val;
	}
	
	public Clock(final long valSec, final long valNano) { //value in second and nanosecond
		this.data = valSec * 1000000000L + valNano;
	}
	
	public long get() {
		return this.data;
	}
	
	public Duration less(final Clock timeUpAppl) {
		// TODO Auto-generated method stub
		return new Duration(this.data - timeUpAppl.data);
	}
	
}
