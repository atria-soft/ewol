package org.atriasoft.echrono;

public class Duration {
	public static Duration milliseconds(final long milli) {
		return new Duration(milli / 1000.0);
	}
	
	private final long data; // stored in ns
	
	public Duration() {
		this.data = 0;
	}
	
	public Duration(final double val) { //value in second
		this.data = (long) (val * 1000000000.0);
	}
	
	public Duration(final int val) { //value in nanosecond
		this.data = val;
	}
	
	public Duration(final long val) { //value in nanosecond
		this.data = val;
	}
	
	public Duration(final long valSec, final long valNano) { //value in second and nanosecond
		this.data = valSec * 1000000000L + valNano;
	}
	
	public long get() {
		return this.data;
	}
	
	public boolean isGreaterThan(final Duration sepatateTime) {
		// TODO Auto-generated method stub
		return this.data - sepatateTime.data > 0;
	}
	
	public float toSeconds() {
		// TODO Auto-generated method stub
		return (float) (this.data / 1000000000.0);
	}
	
}
