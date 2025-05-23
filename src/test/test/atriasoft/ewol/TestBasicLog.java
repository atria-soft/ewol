/*******************************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Contributors:
 *     Edouard DUPIN - initial API and implementation
 ******************************************************************************/
package test.atriasoft.ewol;

import org.junit.jupiter.api.MethodOrderer.OrderAnnotation;
import org.junit.jupiter.api.Order;
import org.junit.jupiter.api.Test;
//import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.TestMethodOrder;

@TestMethodOrder(OrderAnnotation.class)
public class TestBasicLog {
	
	@Test
	@Order(1)
	public void aaFirstInitialisation() {
		// basic kanva
	}
	
}
