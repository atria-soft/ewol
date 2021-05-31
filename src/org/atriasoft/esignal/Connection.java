package org.atriasoft.esignal;

import java.lang.ref.WeakReference;

public class Connection implements AutoCloseable {
	protected WeakReference<ConnectionRemoveInterface> connection;
	
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
		this.connection = null;
	}

	public boolean isConnected() {
		if (this.connection == null) {
			return false;
		}
		ConnectionRemoveInterface tmp = this.connection.get();
		if (tmp == null) {
			return false;
		}
		return true;
	}
}
