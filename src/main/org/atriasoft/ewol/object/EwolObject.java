package org.atriasoft.ewol.object;

import java.lang.ref.WeakReference;

import org.atriasoft.aknot.annotation.AknotAttribute;
import org.atriasoft.aknot.annotation.AknotDefaultAttribute;
import org.atriasoft.aknot.annotation.AknotDefaultManaged;
import org.atriasoft.aknot.annotation.AknotDefaultOptional;
import org.atriasoft.aknot.annotation.AknotDescription;
import org.atriasoft.aknot.annotation.AknotIgnoreUnknown;
import org.atriasoft.aknot.annotation.AknotManaged;
import org.atriasoft.aknot.annotation.AknotName;
import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.context.EwolContext;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

/**
 * Basic message classes for ewol system
 * this class permit at every Object to communicate between them.
 */
@AknotDefaultManaged(value = false)
@AknotDefaultOptional
@AknotDefaultAttribute
@AknotIgnoreUnknown
public class EwolObject {
	private static final Logger LOGGER = LoggerFactory.getLogger(EwolObject.class);
	private static Integer valUID = 0; //!< Static used for the unique ID definition

	/**
	 * get the current the system interface.
	 * @return current reference on the instance.
	 */
	protected static EwolContext getContext() {
		return Ewol.getContext();
	}

	/**
	 * @breif get the current Object manager.
	 * @return the requested object manager.
	 */
	public static ObjectManager getObjectManager() {
		return Ewol.getContext().getEObjectManager();
	}

	/**
	 * Retrive an object with his name (in the global list)
	 * @param objectName Name of the object
	 * @return the requested object or null
	 */
	public static EwolObject getObjectNamed(final String objectName) {
		return EwolObject.getObjectManager().getObjectNamed(objectName);
	}

	protected boolean destroy = false; //!< Flag to know if the object is requesting has destroy.

	private boolean isResource = false; //!< enable this when you want to declare this element is auto-remove

	protected String name = ""; //!< name of the element ...

	protected WeakReference<EwolObject> parent = null; //!< Reference on the current parent.

	private final boolean staticObject = false; //!< set this variable at true if this element must not be auto destroy (exemple : use static object);

	private final int uniqueId; //!< Object UniqueID  == > TODO : Check if it use is needed

	/**
	 * Constructor.
	 */
	public EwolObject() {
		// note this is nearly atomic ... (but it is enough)
		synchronized (EwolObject.valUID) {
			this.uniqueId = EwolObject.valUID++;
		}
		LOGGER.trace("new Object: [{}]", this.uniqueId);

		EwolObject.getObjectManager().add(this);
	}

	/**
	 * Auto-destroy the object
	 */
	protected void autoDestroy() {
		LOGGER.trace("Destroy object: [{}] type: {}", getId(), this.getClass().getCanonicalName());
		if (this.parent != null) {
			final EwolObject parent = this.parent.get();
			// TODO : set a signal to do this ...
			if (parent != null) {
				LOGGER.trace("Destroy object: Call parrent");
				parent.requestDestroyFromChild(this);
			}
		}
		//if no parent ==> noting to do ...
		this.destroy = true;

	}

	/**
	 * Destroy the current object
	 */
	public void destroy() {
		autoDestroy();
	}

	/**
	 * get the UniqueId of the Object
	 * @return the requested ID
	 */
	public int getId() {
		return this.uniqueId;
	}

	@AknotManaged
	@AknotAttribute
	@AknotName(value = "name")
	@AknotDescription(value = "Name of the object.")
	public String getName() {
		return this.name;
	}

	/**
	 * get the static status of the Object  == > mark at true if the user set the object mark as static allocated element ==> not auto remove element
	 * @return true if it might not be removed  == > usefull for conficuration class
	 */
	public boolean getStatic() {
		return this.staticObject;
	}

	/**
	 * Get the resource status of the element.
	 * @return the resource status.
	 */
	public boolean getStatusResource() {
		return this.isResource;
	}

	/**
	 * Retrive an object with his name (in the global list)
	 * @param name Name of the object
	 * @return the requested object or null
	 */
	public EwolObject getSubObjectNamed(final String objectName) {
		LOGGER.trace("check if name: {} ?= {}", objectName, this.name);
		if (objectName.equals(this.name)) {
			return this;
		}
		return null;
	}

	/**
	 * Check if the current objetc his destroy (in removing)
	 * @return true The object is removed
	 * @return false The object is not removed
	 */
	boolean isDestroyed() {
		return this.destroy;
	}

	/**
	 * Remove the current parenting.
	 */
	public void removeParent() {
		this.parent = null;
	}

	/**
	 * Called by a whild that want to remove pointer of itself from the current list of his parrent
	 * @param child Object of the child that want to remove itself
	 */
	protected void requestDestroyFromChild(final EwolObject child) {
		LOGGER.info(
				"requestDestroyFromChild(...) is called when an object reference as a parent have a child that request quto-destroy ...");
		LOGGER.error("Call From Child with no effects ==> must implement : requestDestroyFromChild(...)");
		System.exit(-1);
	}

	public void setName(final String name) {
		this.name = name;
	}

	/**
	 * Set the Object has new parrent.
	 * @param newParent Object that requesting the parenting
	 */
	public void setParent(final EwolObject newParent) {
		// TODO : Implement change of parent ...
		this.parent = new WeakReference<>(newParent);
	}

	/**
	 * Declare this element as a resource (or singleton) this mean the element will
	 * not be auto Remove at the end of the programm. It just notify that it is not removed.
	 * @param val Value of the type of the element.
	 */
	public void setStatusResource(final boolean val) {
		this.isResource = val;
	}

}
