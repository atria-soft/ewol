package org.atriasoft.echrono;

/**
 * @brief Clock is a compleate virtual clock that is used to virtualize the urrent clock used (can be non real-time, ex:for simulation)
 */
public class Clock {
	public static Clock now() {
		return new Clock(System.nanoTime());
	}
	
	private final long data; //!< virtual clock
	
	public Clock() {
		this.data = 0;
	}
	
	public Clock(final double _val) { //value in second
		this.data = (long) (_val * 1000000000.0);
	}
	
	public Clock(final int _val) { //value in nanosecond
		this.data = _val;
	}
	
	public Clock(final long _val) { //value in nanosecond
		this.data = _val;
	}
	
	public Clock(final long _valSec, final long _valNano) { //value in second and nanosecond
		this.data = _valSec * 1000000000L + _valNano;
	}
	
	public long get() {
		return this.data;
	}
	
	public Duration less(final Clock timeUpAppl) {
		// TODO Auto-generated method stub
		return new Duration(this.data - timeUpAppl.data);
	}
	
}
