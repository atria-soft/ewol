package org.atriasoft.ephysics.collision.broadphase;

import java.util.Set;

import javafx.collections.transformation.SortedList;

public class PairDTree {
	public static int countInSet(final Set<PairDTree> values, final PairDTree sample) {
		int count = 0;
		for (final PairDTree elem : values) {
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
	
	public static int countInSet(final SortedList<PairDTree> values, final PairDTree sample) {
		int count = 0;
		for (final PairDTree elem : values) {
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
	
	public final DTree first;
	
	public final DTree second;
	
	public PairDTree(final DTree first, final DTree second) {
		if (first.uid < second.uid) {
			this.first = first;
			this.second = second;
		} else {
			this.first = second;
			this.second = first;
			
		}
	}
	
	@Override
	public boolean equals(final Object obj) {
		if (this == obj) {
			return true;
		}
		if (obj == null || getClass() != obj.getClass()) {
			return false;
		}
		final PairDTree tmp = (PairDTree) obj;
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
		return "PairDTree [first=" + this.first.uid + ", second=" + this.second.uid + "]";
	}
}
