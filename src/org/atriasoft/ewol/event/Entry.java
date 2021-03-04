package org.atriasoft.ewol.event;

import org.atriasoft.gale.key.KeyKeyboard;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class Entry {
	private final KeyKeyboard type; //!< type of hardware event
	private final KeyStatus status; //!< status of hardware event
	private final KeySpecial specialKey; //!< input key status (prevent change in time..)
	private final Character unicodeData; //!< Unicode data (in some case)
	
	public Entry(final KeyKeyboard _type, final KeyStatus _status, final KeySpecial _specialKey, final Character _char) {
		this.type = _type;
		this.status = _status;
		this.specialKey = _specialKey;
		this.unicodeData = _char;
		
	}
	
	public Character getChar() {
		return this.unicodeData;
	};
	
	public KeySpecial getSpecialKey() {
		return this.specialKey;
	};
	
	public KeyStatus getStatus() {
		return this.status;
	};
	
	public KeyKeyboard getType() {
		return this.type;
	};
	
};