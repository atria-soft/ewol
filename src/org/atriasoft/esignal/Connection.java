package org.atriasoft.esignal;

import java.lang.ref.WeakReference;

public class Connection implements AutoCloseable {
	protected final WeakReference<ConnectionRemoveInterface> connection;
	
	public void disconnect() {
		close();
	}

	public Connection( final ConnectionRemoveInterface object ) {
		this.connection = new WeakReference<>(object);
	}

	public Connection() {
		this.connection = null;
	}

	@Override
	public void close() {
		if (this.connection == null) {
			return;
		}
		ConnectionRemoveInterface tmp = this.connection.get();
		if (tmp == null) {
			return;
		}
		tmp.disconnect(this);
	}
}
