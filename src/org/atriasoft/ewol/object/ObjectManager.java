package org.atriasoft.ewol.object;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.atriasoft.echrono.Clock;
import org.atriasoft.echrono.Duration;
import org.atriasoft.echrono.Time;
import org.atriasoft.esignal.Signal;
import org.atriasoft.ewol.context.EwolContext;
import org.atriasoft.ewol.event.EventTime;
import org.atriasoft.ewol.internal.Log;

/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

public class ObjectManager {
	private final Time applWakeUpTime; //!< Time of the application initialize
	private EwolContext context = null;
	
	private final List<WeakReference<EwolObject>> eObjectList = new ArrayList<>(); // all widget allocated  == > all time increment ... never removed ...
	
	private Clock lastPeriodicCallTime; //!< last call time ...
	
	public final Signal<EventTime> periodicCall = new Signal<>();
	
	private final List<EwolObject> workerList = new ArrayList<>();
	
	public ObjectManager(final EwolContext context) {
		this.context = context;
		//periodicCall(this, "periodic", "Call every time system render");
		Log.debug(" == > init Object-Manager");
		Log.todo("set this back ...");
		//this.periodicCall.setPeriodic(true);
		// set the basic time properties :
		this.applWakeUpTime = Time.now();
		this.lastPeriodicCallTime = new Clock(this.applWakeUpTime.get());
	}
	
	/**
	 * Internal API that used only with Object toi reference itself in the manager.
	 * @note The manager remove the object when the refecence Low down 1 (last keeper)
	 * @param object Reference shared pointer on the object
	 */
	public synchronized void add(final EwolObject object) {
		if (object == null) {
			Log.error("try to add an inexistant Object in manager");
		}
		this.eObjectList.add(new WeakReference<>(object));
	}
	
	/**
	 * clean the weak pointer list (remove weakptr that is remoed)
	 */
	public synchronized void cleanInternalRemoved() {
		final int nbObject = this.eObjectList.size();
		//Log.verbose("Clean Object List (if needed) : " + this.eObjectList.size() + " elements");
		final Iterator<WeakReference<EwolObject>> iterator = this.eObjectList.iterator();
		while (iterator.hasNext()) {
			final WeakReference<EwolObject> elem = iterator.next();
			if (elem.get() == null) {
				iterator.remove();
			}
		}
		if (this.eObjectList.size() != nbObject) {
			Log.verbose(" remove " + (nbObject - this.eObjectList.size()) + " deprecated objects");
		}
	}
	
	/**
	 * Display all object Open.
	 */
	public synchronized void displayListObject() {
		Log.info("List loaded object : ");
		for (final WeakReference<EwolObject> it : this.eObjectList) {
			final EwolObject element = it.get();
			if (element != null) {
				Log.info("  [" + element.getId() + "] name='" + element.getName() + "' type=" + element.getClass().getCanonicalName());
			}
		}
	}
	
	/**
	 * Retrive an Object with his name
	 * @param name Name of the Object
	 * @return Pointer on the finded Object.
	 */
	public synchronized EwolObject get(final String name) {
		if (name.isEmpty()) {
			return null;
		}
		for (final WeakReference<EwolObject> it : this.eObjectList) {
			final EwolObject element = it.get();
			if (element != null && element.getName().contentEquals(name)) {
				return element;
			}
		}
		return null;
	}
	
	/**
	 * Get the number of loaded object in the system
	 * @return number of Object
	 */
	public synchronized int getNumberObject() {
		return this.eObjectList.size();
	}
	
	/**
	 * retrive an object with his name
	 * @param name Name of the object
	 * @return the requested object or null
	 */
	public synchronized EwolObject getObjectNamed(final String name) {
		return get(name);
	}
	
	/**
	 * Call every time we can with the current time
	 * @param localTime Current system Time.
	 */
	public synchronized void timeCall(final Clock localTime) {
		final Clock previousTime = this.lastPeriodicCallTime;
		this.lastPeriodicCallTime = localTime;
		if (this.periodicCall.size() <= 0) {
			return;
		}
		final Duration deltaTime = new Duration(localTime.get() - previousTime.get());
		
		final EventTime myTime = new EventTime(localTime, this.applWakeUpTime.toClock(), deltaTime, deltaTime);
		this.periodicCall.emit(myTime);
	}
	
	/**
	 * @breif check if the Interface have some user that request a periodic call
	 * @return true, have some periodic event...
	 */
	public synchronized boolean timeCallHave() {
		return this.periodicCall.size() > 0;
	}
	
	/**
	 * If the application is suspended The Ewol Object manager does not know it, just call this to update delta call
	 * @param localTime Current system Time.
	 */
	public synchronized void timeCallResume(final Clock localTime) {
		this.lastPeriodicCallTime = localTime;
	}
	
	/**
	 * remove all resources (un-init) out of the destructor (due to the system implementation)
	 */
	public synchronized void unInit() {
		Log.debug(" == > Un-Init Object-Manager");
		if (this.workerList.size() > 0) {
			Log.debug(" == > Remove all workers");
			this.workerList.clear();
		}
		for (final WeakReference<EwolObject> it : this.eObjectList) {
			final EwolObject element = it.get();
			if (element != null) {
				//it.removeObject();
			}
		}
		if (this.eObjectList.size() != 0) {
			Log.error("Have " + this.eObjectList.size() + " active Object");
		}
		this.eObjectList.clear();
	}
	
	/**
	 * Add a worker on the system list.
	 * @param worker Worker to add in the list.
	 */
	public synchronized void workerAdd(final EwolObject worker) {
		this.workerList.add(worker);
	}
	
	/**
	 * Remove a worker on the system list.
	 * @param worker Worker to add in the list.
	 */
	public synchronized void workerRemove(final EwolObject worker) {
		
		final Iterator<EwolObject> iterator = this.workerList.iterator();
		while (iterator.hasNext()) {
			final EwolObject elem = iterator.next();
			if (elem == worker) {
				iterator.remove();
			}
		}
	}
	
}
