package org.atriasoft.esignal;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;

class ConnectedElement<T> {
	protected final Consumer<T> consumer;
	protected final WeakReference<Object> reference;
	
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
	public Object getIfAlive() {
		return this.reference.get();
	}
	public boolean isCompatibleWith(final Object elem) {
		Object out = this.reference.get();
		if (out == elem) {
			return true;
		}
		return false;
	}
}
class ConnectedElementDynamic<T> extends ConnectedElement<T> {
	protected final WeakReference<Connection> connection;
	
	public ConnectedElementDynamic(final WeakReference<Connection> connection, final WeakReference<Object> reference, final Consumer<T> consumer) {
		super(reference, consumer);
		this.connection = connection;
	}
	
	public WeakReference<Connection> getConnection() {
		return this.connection;
	}
	
	@Override
	public Object getIfAlive() {
		Object out = this.reference.get();
		if (out == null) {
			return null;
		}
		Object outConnection = this.connection.get();
		if (outConnection == null) {
			return null;
		}
		return out;
	}
	@Override
	public boolean isCompatibleWith(final Object elem) {
		if (super.isCompatibleWith(elem)) {
			return true;
		}
		Object outConnection = this.connection.get();
		if (outConnection == elem) {
			return true;
		}
		return false;
	}
	
}

public class Signal<T> implements ConnectionRemoveInterface {
	List<ConnectedElement<T>> data = new ArrayList<>();
	
	public void clear() {
		synchronized(this.data) {
			this.data = new ArrayList<>();
		}
	}

	public void connect(final Object reference, final Consumer<T> function) {
		synchronized(this.data) {
			WeakReference<Object> weakRef = new WeakReference<Object>(reference);
			this.data.add(new ConnectedElement<T>(weakRef, function));
		}
	}
	public void disconnect(final Object obj) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<T> elem = iterator.next();
				if (elem.isCompatibleWith(obj)) {
					iterator.remove();
				}
			}
		}
	}
	public Connection connectDynamic(final Object reference, final Consumer<T> function) {
		WeakReference<Object> weakRef = new WeakReference<Object>(reference);
		Connection out = new Connection(this); 
		WeakReference<Connection> weakConnection = new WeakReference<Connection>(out);
		synchronized(this.data) {
			this.data.add(new ConnectedElementDynamic<T>(weakConnection, weakRef, function));
		}
		return out;
	}
	
	@Override
	public void disconnect(final Connection connection) {
		synchronized(this.data) {
			final Iterator<ConnectedElement<T>> iterator = this.data.iterator();
			while (iterator.hasNext()) {
				final ConnectedElement<T> elem = iterator.next();
				if (elem.isCompatibleWith(connection)) {
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
				Object tmpObject = elem.getIfAlive();
				if (tmpObject == null) {
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
				Object tmpObject = elem.getIfAlive();
				if (tmpObject == null) {
					continue;
				}
				elem.getConsumer().accept(value);
			}
		}
	}
	
	public int size() {
		return this.data.size();
	}
}
