package org.atriasoft.esignal;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;

class ConnectedElement<T> {
	protected final WeakReference<Consumer<T>> consumer;
	
	public ConnectedElement(final Consumer<T> consumer) {
		this.consumer = new WeakReference<Consumer<T>>(consumer);
	}
	
	public Consumer<T> getConsumer() {
		return this.consumer.get();
	}

	public boolean isCompatibleWith(final Object elem) {
		/*
		Object out = this.reference.get();
		if (out == elem) {
			return true;
		}
		return false;
		*/
		return false;
	}

	public void disconnect() {
		
	}
}

class ConnectedElementDynamic<T> extends ConnectedElement<T> {
	protected final WeakReference<Object> linkedObject;
	
	public ConnectedElementDynamic(Object linkedObject, final Consumer<T> consumer) {
		super(consumer);
		this.linkedObject = new WeakReference<Object>(linkedObject);
	}
	
	@Override
	public Consumer<T> getConsumer() {
		if (this.linkedObject.get() == null) {
			return null;
		}
		return this.consumer.get();
	}
	
	@Override
	public boolean isCompatibleWith(final Object elem) {
		if (super.isCompatibleWith(elem)) {
			return true;
		}
		Object obj = this.linkedObject.get();
		if (obj == elem) {
			return true;
		}
		return false;
	}

	@Override
	public void disconnect() {
		Object obj = this.linkedObject.get();
		if (obj == null) {
			return;
		}
		if (obj instanceof Connection tmp) {
			tmp.connectionIsRemovedBySignal();
		}
	}
}

public class Signal<T> implements ConnectionRemoveInterface {
	List<ConnectedElement<T>> data = new ArrayList<>();
	
	public void clear() {
		List<ConnectedElement<T>> data2 = data;
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
		final Iterator<ConnectedElement<T>> iterator = this.data2.iterator();
		while (iterator.hasNext()) {
			final ConnectedElement<T> elem = iterator.next();
			elem.disconnect();
		}
	}

	public void connect(final Consumer<T> function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElement<T>(function));
		}
	}
//	public void disconnect(final Consumer<T> obj) {
//		synchronized(this.data) {
//			final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
//			while (iterator.hasNext()) {
//				final ConnectedElement<T> elem = iterator.next();
//				if (elem.isCompatibleWith(obj)) {
//					iterator.remove();
//				}
//			}
//		}
//	}
	public Connection connectDynamic(final Consumer<T> function) {
		Connection out = new Connection(this);
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<T>(out, function));
		}
		return out;
	}
	public void connectAutoRemoveObject(Object reference, final Consumer<T> function) {
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<T>(reference, function));
		}
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<T> elem = iterator.next();
				if (elem.isCompatibleWith(connection)) {
					elem.disconnect();
					iterator.remove();
				}
			}
		}
	}
	
	public void emit(final T value) {
		List<ConnectedElement<T>> tmp;
		// clean the list:
		synchronized(this.data) {
			final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<T> elem = iterator.next();
				Object tmpObject = elem.getConsumer();
				if (tmpObject == null) {
					elem.disconnect();
					iterator.remove();
				}
			}
			// simple optimization:
			if (this.data.isEmpty()) {
				return;
			}
			// clone the list to permit to have asynchronous remove call
			tmp = new ArrayList<>(this.data);
		}
		// real call elements
		{
			final Iterator<ConnectedElement<T>> iterator = tmp.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<T> elem = iterator.next();
				Consumer<T> tmpObject = elem.getConsumer();
				if (tmpObject == null) {
					continue;
				}
				tmpObject.accept(value);
			}
		}
	}
	
	public int size() {
		return this.data.size();
	}

}
