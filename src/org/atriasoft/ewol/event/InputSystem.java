package org.atriasoft.ewol.event;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;

import jdk.internal.org.jline.reader.Widget;

public class InputSystem {
	public EventInput event;
	
	private Widget dest;
	
	private int realIdEvent;
	
	public InputSystem(final KeyType _type, final KeyStatus _status, final int _id, final Vector2f _pos, final Widget _dest, final int _realIdEvent, final KeySpecial _specialKey) {
		this.event = new EventInput(_type, _status, _id, _pos, _specialKey);
		this.dest = _dest;
		this.realIdEvent = _realIdEvent;
	}
	
	public Widget getDestWidget() {
		return this.dest;
	}
	
	public int getRealId() {
		return this.realIdEvent;
	}
	
	public void setDestWidget(final Widget _dest) {
		this.dest = _dest;
	}
	
	public void setRealId(final int _realIdEvent) {
		this.realIdEvent = _realIdEvent;
	}
}