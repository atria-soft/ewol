package org.atriasoft.ewol.event;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.gale.key.KeySpecial;
import org.atriasoft.gale.key.KeyStatus;
import org.atriasoft.gale.key.KeyType;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class EventInput {
	private KeyType type;
	private KeyStatus status;
	private int inputId;
	private Vector2f pos;
	private KeySpecial specialKey; //!< input key status (prevent change in time..)
	
	public EventInput(final KeyType _type, final KeyStatus _status, final int _id, final Vector2f _pos, final KeySpecial _specialKey) {
		this.type = _type;
		this.status = _status;
		this.inputId = _id;
		this.pos = _pos;
		this.specialKey = _specialKey;
		
	};
	
	public int getId() {
		return this.inputId;
	};
	
	public Vector2f getPos() {
		return this.pos;
	};
	
	public KeySpecial getSpecialKey() {
		return this.specialKey;
	};
	
	public KeyStatus getStatus() {
		return this.status;
	};
	
	public KeyType getType() {
		return this.type;
	};
	
	/**
	 * Reset the input property of the curent event.
	 */
	public void reset() {
		// TODO : Call the entry element ant rest it ...
	};
	
	public void setId(final int _id) {
		this.inputId = _id;
	};
	
	public void setPos(final Vector2f _pos) {
		this.pos = _pos;
	};
	
	public void setSpecialKey(final KeySpecial _specialKey) {
		this.specialKey = _specialKey;
	};
	
	public void setStatus(final KeyStatus _status) {
		this.status = _status;
	};
	
	public void setType(final KeyType _type) {
		this.type = _type;
	}
}
