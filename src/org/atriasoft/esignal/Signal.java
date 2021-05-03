package org.atriasoft.esignal;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;

class ConnectedElement<T> {
	private final Consumer<T> consumer;
	private final WeakReference<Object> reference;
	
	public ConnectedElement(final WeakReference<Object> reference, final Consumer<T> consumer) {
		this.reference = reference;
		this.consumer = consumer;
	}
	
	public Consumer<T> getConsumer() {
		return this.consumer;
	}
	
	public WeakReference<Object> getReference() {
		return this.reference;
	}
	
}

public class Signal<T> {
	
	List<ConnectedElement<T>> data = new ArrayList<>();
	
	public void clear(final Object obj) {
		
	}
	
	public Connection connect(final Object reference, final T fucntion) {
		
		return null;
	}
	
	public void disconnect(final Connection connection) {
		
	}
	
	public void disconnect(final Object obj) {
		
	}
	
	public void emit(final T value) {
		final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
		while (iterator.hasNext()) {
			final ConnectedElement<T> elem = iterator.next();
			if (elem.getReference().get() == null) {
				iterator.remove();
			}
			elem.getConsumer().accept(value);
		}
	}
	
	public int size() {
		return this.data.size();
	}
}
