package org.atriasoft.echrono;

/**
 * Steady is a Program start time clock
 */
public class Steady {
	public static Steady now() {
		return new Steady(System.nanoTime());
	}
	
	private final long data; //!< Monotonic clock since computer start (ns)
	
	public Steady() {
		this.data = 0;
	}
	
	public Steady(final double val) { //value in second
		this.data = (long) (val * 1000000000.0);
	}
	
	public Steady(final int val) { //value in nanosecond
		this.data = val;
	}
	
	public Steady(final long val) { //value in nanosecond
		this.data = val;
	}
	
	public Steady(final long valSec, final long valNano) { //value in second and nanosecond
		this.data = valSec * 1000000000L + valNano;
	}
	
	public long get() {
		return this.data;
	}
	
	public Duration less(final Steady other) {
		// TODO Auto-generated method stub
		return new Duration(this.data - other.data);
	}
}
