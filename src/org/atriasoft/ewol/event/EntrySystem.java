package org.atriasoft.ewol.event;

import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;

public class EntrySystem {
	public final EventEntry event;
	
	public EntrySystem(final KeyKeyboard _type, final KeyStatus _status, final KeySpecial _specialKey, final Character _char) {
		this.event = new EventEntry(_specialKey, _type, _status, _char);
	}
}