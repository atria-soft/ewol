package org.atriasoft.ephysics.mathematics;

import java.util.Set;

import javafx.collections.transformation.SortedList;

public class PairInt {
	public static int countInSet(final Set<PairInt> values, final PairInt sample) {
		int count = 0;
		for (final PairInt elem : values) {
			if (elem.first != sample.first) {
				continue;
			}
			if (elem.second != sample.second) {
				continue;
			}
			count++;
		}
		return count;
	}
	
	public static int countInSet(final SortedList<PairInt> values, final PairInt sample) {
		int count = 0;
		for (final PairInt elem : values) {
			if (elem.first != sample.first) {
				continue;
			}
			if (elem.second != sample.second) {
				continue;
			}
			count++;
		}
		return count;
	}
	
	public final int first;
	
	public final int second;
	
	public PairInt(final int first, final int second) {
		this.first = first;
		this.second = second;
	}
	
	@Override
	public boolean equals(final Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null || getClass() != obj.getClass()) {
			return false;
		}
		final PairInt tmp = (PairInt) obj;
		if (this.first != tmp.first) {
			return false;
		}
		if (this.second != tmp.second) {
			return false;
		}
		return true;
	}
	
	@Override
	public String toString() {
		return "PairInt [first=" + this.first + ", second=" + this.second + "]";
	}
}
