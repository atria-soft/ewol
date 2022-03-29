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

public record EventInput(
		KeyType type,
		KeyStatus status,
		int inputId,
		Vector2f pos,
		KeySpecial specialKey) {
	
}
