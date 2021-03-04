package org.atriasoft.echrono;

public class Duration {
	private final long data; // stored in ns
	
	public Duration() {
		this.data = 0;
	}
	
	public Duration(final double _val) { //value in second
		this.data = (long) (_val * 1000000000.0);
	}
	
	public Duration(final int _val) { //value in nanosecond
		this.data = _val;
	}
	
	public Duration(final long _val) { //value in nanosecond
		this.data = _val;
	}
	
	public Duration(final long _valSec, final long _valNano) { //value in second and nanosecond
		this.data = _valSec * 1000000000L + _valNano;
	}
	
	public long get() {
		return this.data;
	}
	
	public float toSeconds() {
		// TODO Auto-generated method stub
		return (float) (this.data / 1000000000.0);
	}
	
}
