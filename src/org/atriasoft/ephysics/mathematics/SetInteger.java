package org.atriasoft.ephysics.mathematics;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class SetInteger {
	private static void quickSort2(final List<Integer> _data, final int _low, final int _high, final Comparator<Integer> _comparator) {
		if (_low >= _high) {
			return;
		}
		// pi is partitioning index, arr[p] is now at right place
		final int pi = quickSortPartition(_data, _low, _high, _comparator);
		// Separately sort elements before partition and after partition
		//if (pi != 0) {
		quickSort2(_data, _low, pi - 1, _comparator);
		//}
		quickSort2(_data, pi + 1, _high, _comparator);
	}
	
	private static int quickSortPartition(final List<Integer> _data, final int _low, final int _high, final Comparator<Integer> _comparator) {
		int iii = (_low - 1);
		for (int jjj = _low; jjj < _high; ++jjj) {
			if (_comparator.compare(_data.get(jjj), _data.get(_high)) < 0) {
				iii++;
				Collections.swap(_data, iii, jjj);
			}
		}
		Collections.swap(_data, iii + 1, _high);
		return (iii + 1);
	}
	
	private final List<Integer> data = new ArrayList<>(); //!< Data of the Set ==> the Set table is composed of pointer, this permit to have high speed when resize the vector ...
	
	private Comparator<Integer> comparator = new Comparator<Integer>() {
		@Override
		public int compare(final Integer a, final Integer b) {
			if (a < b) {
				return -1;
			} else if (a == b) {
				return 0;
			}
			return -1;
		}
	};
	
	/**
	 * @brief Constructor of the Set table.
	 */
	public SetInteger() {
		
	}
	
	/**
	 * @brief Add an element OR set an element value
	 * @note add and set is the same function.
	 * @param[in] _key Name of the value to set in the Set table.
	 * @param[in] _value Value to set in the Set table.
	 */
	public void add(final Integer _key) {
		for (int iii = 0; iii < this.data.size(); ++iii) {
			if (_key == this.data.get(iii)) {
				return;
			}
			if (this.comparator.compare(_key, this.data.get(iii)) == 0) {
				// Find a position
				this.data.add(iii, _key);
				return;
			}
		}
		this.data.add(_key);
	}
	
	/**
	 * @brief Remove all entry in the Set table.
	 * @note It does not delete pointer if your value is a pointer type...
	 */
	public void clear() {
		this.data.clear();
	}
	
	/**
	 * @brief Count the number of occurence of a specific element.
	 * @param[in] _key Name of the element to count iterence
	 * @return 0 No element was found
	 * @return 1 One element was found
	 */
	public int count(final Integer _key) {
		// TODO: search in a dichotomic way.
		for (int iii = 0; iii < this.data.size(); iii++) {
			if (this.comparator.compare(this.data.get(iii), _key) == 0) {
				return 1;
			}
		}
		return 0;
	}
	
	/**
	 * @brief Check if an element exist or not
	 * @param[in] _key Name of the Set requested
	 * @return true if the element exist
	 */
	public boolean exist(final Integer _key) {
		final int it = find(_key);
		if (it == -1) {
			return false;
		}
		return true;
	}
	
	/**
	 * @brief Find an element position the the Set table
	 * @param[in] _key Name of the element to find
	 * @return Iterator on the element find or end()
	 */
	public int find(final Integer _key) {
		// TODO: search in a dichotomic way.
		for (int iii = 0; iii < this.data.size(); iii++) {
			if (this.comparator.compare(this.data.get(iii), _key) == 0) {
				return iii;
			}
		}
		return -1;
	}
	
	/**
	 * @brief Get Element an a special position
	 * @param[in] _position Position in the Set
	 * @return An reference on the selected element
	 */
	public Integer get(final int _position) {
		return this.data.get(_position);
	}
	
	public List<Integer> getRaw() {
		return this.data;
	}
	
	/**
	 * @brief Check if the container have some element
	 * @return true The container is empty
	 * @return false The container have some element
	 */
	public boolean isEmpty() {
		return this.data.size() == 0;
	}
	
	/**
	 * @brief Remove the last element of the vector
	 */
	public void popBack() {
		this.data.remove(this.data.size() - 1);
	}
	
	/**
	 * @brief Remove the first element of the vector
	 */
	public void popFront() {
		this.data.remove(0);
	}
	
	/**
	 * @brief: QuickSort implementation of sorting vector (all elements.
	 * @param[in,out] _data Vector to sort.
	 * @param[in] _high Comparator function of this element.
	 */
	void quickSort(final List<Integer> _data, final Comparator<Integer> _comparator) {
		quickSort2(_data, 0, _data.size() - 1, _comparator);
	}
	
	/**
	 * @brief: QuickSort implementation of sorting vector.
	 * @param[in,out] _data Vector to sort.
	 * @param[in] _low Lowest element to sort.
	 * @param[in] _high Highest element to sort.
	 * @param[in] _high Comparator function of this element.
	 */
	void quickSort(final List<Integer> _data, final int _low, int _high, final Comparator<Integer> _comparator) {
		if (_high >= _data.size()) {
			_high = _data.size() - 1;
		}
		/*if (_low >= _data.size()) {
			_low = _data.size()-1;
		}*/
		quickSort2(_data, _low, _high, _comparator);
	}
	
	/**
	 * @brief Set the comparator of the set.
	 * @param[in] _comparator comparing function.
	 */
	public void setComparator(final Comparator<Integer> _comparator) {
		this.comparator = _comparator;
		sort();
	}
	
	/**
		 * @brief Get the number of element in the Set table
		 * @return number of elements
		 */
	public int size() {
		return this.data.size();
	}
	
	/**
	 * @brief Order the Set with the corect functor
	 */
	private void sort() {
		quickSort(this.data, this.comparator);
	}
}
