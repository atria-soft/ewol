package org.atriasoft.ephysics.mathematics;

import java.util.Comparator;

public class SetMultiple<TYPE> extends Set<TYPE> {
	/**
	 * @brief Constructor of the Set table.
	 */
	public SetMultiple(final Comparator<TYPE> comparator) {
		super(comparator);
	}
	
	/**
	 * @brief Add an element OR set an element value
	 * @note add and set is the same function.
	 * @param[in] _key Name of the value to set in the Set table.
	 * @param[in] _value Value to set in the Set table.
	 */
	@Override
	public void add(final TYPE _key) {
		for (int iii = 0; iii < this.data.size(); ++iii) {
			if (_key == this.data.get(iii)) {
				return;
			}
			final int compareValue = this.comparator.compare(_key, this.data.get(iii));
			if (compareValue == 0) {
				// Find a position
				this.data.set(iii, _key);
				return;
			}
			// for single Element
			if (compareValue == 1) {
				// Find a position
				this.data.add(iii, _key);
				return;
			}
		}
		this.data.add(_key);
	}
	
}
