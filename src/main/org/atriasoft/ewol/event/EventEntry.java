package org.atriasoft.ewol.event;

import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;

public record EventEntry(
	KeySpecial specialKey, //!< input key status (prevent change in time..)
	KeyKeyboard type, //!< type of hardware event
	KeyStatus status, //!< status of hardware event
	Character unicodeData //!< Unicode data (in some case)
	){
	
	public Character getChar() {
		return this.unicodeData;
	}
	
	@Override
	public String toString() {
		return "EventEntry [type=" + this.type + ", status=" + this.status + ", unicodeData=" + this.unicodeData + ", specialKey=" + this.specialKey + "]";
	}
}
