package org.atriasoft.ewol.object;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

/**
 * @brief A worker might not been possesed by someone, then the system might keep a pointer on it.
 */
public class Worker extends EwolObject {
	/**
	 * @brief Constructor.
	 */
	public Worker() {
		getObjectManager().workerAdd(this);
	}
	
	@Override
	public void destroy() {
		getObjectManager().workerRemove(this);
	}
}
