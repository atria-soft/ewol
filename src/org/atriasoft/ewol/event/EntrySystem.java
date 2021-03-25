package org.atriasoft.ewol.event;

import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;

public record EntrySystem(
		EventEntry event) {
	public EntrySystem(final EventEntry event) {
		this.event = event;
	}
	
	public EntrySystem(final KeyKeyboard type, final KeyStatus status, final KeySpecial specialKey, final Character unicodeChar) {
		this(new EventEntry(specialKey, type, status, unicodeChar));
	}
}