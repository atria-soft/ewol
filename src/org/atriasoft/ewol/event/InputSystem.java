package org.atriasoft.ewol.event;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.widget.Widget;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;

@SuppressWarnings("preview")
public record InputSystem(
		EventInput event,
		Widget dest,
		int realIdEvent) {
	public InputSystem(final EventInput event, final Widget dest, final int realIdEvent) {
		this.event = event;
		this.dest = dest;
		this.realIdEvent = realIdEvent;
	}
	
	public InputSystem(final KeyType type, final KeyStatus status, final int id, final Vector2f pos, final Widget dest, final int realIdEvent, final KeySpecial specialKey) {
		this(new EventInput(type, status, id, pos, specialKey), dest, realIdEvent);
	}
	
}