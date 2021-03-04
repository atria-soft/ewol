package org.atriasoft.ewol.event;

import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;

public class EntrySystem {
	public final Entry event;
	
	public EntrySystem(final KeyKeyboard _type, final KeyStatus _status, final KeySpecial _specialKey, final Character _char) {
		this.event = new Entry(_type, _status, _specialKey, _char);
	}
}