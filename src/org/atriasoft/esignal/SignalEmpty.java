package org.atriasoft.esignal;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

class ConnectedElementEmpty {
	private final WeakReference<Object> reference;
	private final Runnable runnable;
	
	public ConnectedElementEmpty(final WeakReference<Object> reference, final Runnable runnable) {
		this.reference = reference;
		this.runnable = runnable;
	}
	
	public WeakReference<Object> getReference() {
		return this.reference;
	}
	
	public Runnable getRunner() {
		return this.runnable;
	}
	
}

public class SignalEmpty {
	
	List<ConnectedElementEmpty> data = new ArrayList<>();
	
	public void clear(final Object obj) {
		
	}
	
	public Connection connect(final Object reference, final Runnable runnable) {
		
		return null;
	}
	
	public void disconnect(final Connection connection) {
		
	}
	
	public void disconnect(final Object obj) {
		
	}
	
	public void emit() {
		final Iterator<ConnectedElementEmpty> iterator = this.data.iterator();
		while (iterator.hasNext()) {
			final ConnectedElementEmpty elem = iterator.next();
			if (elem.getReference().get() == null) {
				iterator.remove();
			}
			elem.getRunner().run();
		}
	}
	
	public int size() {
		return this.data.size();
	}
}
