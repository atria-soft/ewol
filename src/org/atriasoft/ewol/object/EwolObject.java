package org.atriasoft.ewol.object;

import java.lang.ref.WeakReference;

import org.atriasoft.ewol.Ewol;
import org.atriasoft.ewol.context.EwolContext;
import org.atriasoft.ewol.internal.Log;
import org.atriasoft.exml.model.XmlElement;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

/**
 * Basic message classes for ewol system
 * this class permit at every Object to communicate between them.
 */
public class EwolObject {
	private static Integer valUID = 0; //!< Static used for the unique ID definition
	
	/**
	 * get the curent the system inteface.
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
	
	//@EwolPropertyDescription("Object name, might be a unique reference in all the program")
	//@JacksonXmlProperty(isAttribute = true, localName = "name")
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
		Log.debug("new Object : [" + this.uniqueId + "]");
		
		EwolObject.getObjectManager().add(this);
	}
	
	/**
	 * Auto-destroy the object
	 */
	protected void autoDestroy() {
		Log.verbose("Destroy object: [" + getId() + "] type:" + this.getClass().getCanonicalName());
		final EwolObject parent = this.parent.get();
		// TODO : set a signal to do this ...
		if (parent != null) {
			Log.verbose("Destroy object: Call parrent");
			parent.requestDestroyFromChild(this);
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
	
	public String getName() {
		return this.name;
	}
	
	/**
	 * load attribute properties with an XML node.
	 * @param node Reference on the XML node.
	 * @return true : All has been done corectly.
	 * @return false : An error occured.
	 */
	/*
	boolean loadXMLAttributes( exml::Element node){
		if (node.exist() == false) {
			return false;
		}
		boolean errorOccured = false;
		
		for( auto it : node.attributes) {
			auto pair = it.getPair();
			if (pair.first == "") {
				continue;
			}
			if (properties.set(pair.first, pair.second) == false) {
				errorOccured = true;
			}
		}
		return errorOccured;
	}
	*/
	
	/**
	 * get the static status of the Object  == > mark at true if the user set the object mark as static allocated element ==> not auto remove element
	 * @return true if it might not be removed  == > usefull for conficuration class
	 */
	public boolean getStatic() {
		return this.staticObject;
	}
	
	/**
	 * store properties in this XML node.
	 * @param[in,out] node Reference on the XML node.
	 * @return true : All has been done corectly.
	 * @return false : An error occured.
	 */
	/*
	boolean storeXML(exml::Element node){
		if (node.exist() == false) {
			return false;
		}
		boolean errorOccured = true;
		for (auto it : properties.getAll(true)) {
			node.attributes.set(it.first, it.second);
		}
		return errorOccured;
	}
	*/
	
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
		Log.verbose("check if name : " + objectName + " ?= " + this.name);
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
	 * load properties with an XML node.
	 * @param node Reference on the XML node.
	 * @return true : All has been done corectly.
	 * @return false : An error occured.
	 */
	protected boolean loadXML(final XmlElement node) {
		return true;
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
		Log.info("requestDestroyFromChild(...) is called when an object reference as a parent have a child that request quto-destroy ...");
		Log.critical("Call From Child with no effects ==> must implement : requestDestroyFromChild(...)");
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
