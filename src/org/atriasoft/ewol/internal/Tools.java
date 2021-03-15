package org.atriasoft.ewol.internal;

public class Tools {
	/**
	 * @brief get the next power 2 if the input
	 * @param[in] value Value that we want the next power of 2
	 * @return result value
	 */
	public static int nextP2(final int _value) {
		int val = 1;
		for (int iii = 1; iii < 31; iii++) {
			if (_value <= val) {
				return val;
			}
			val *= 2;
		}
		Log.critical("impossible CASE....");
		return val;
	}
	
	private Tools() {}
}
