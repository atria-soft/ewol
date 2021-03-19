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
	private final List<WeakReference<EwolObject>> eObjectList = new ArrayList<>(); // all widget allocated  == > all time increment ... never removed ...
	private EwolContext context = null;
	
	private final List<EwolObject> workerList = new ArrayList<>();
	
	public final Signal<EventTime> periodicCall = new Signal<>();
	
	private final Time applWakeUpTime; //!< Time of the application initialize
	
	private Clock lastPeriodicCallTime; //!< last call time ...
	
	public ObjectManager(final EwolContext _context) {
		this.context = _context;
		//periodicCall(this, "periodic", "Call every time system render");
		Log.debug(" == > init Object-Manager");
		Log.todo("set this back ...");
		//this.periodicCall.setPeriodic(true);
		// set the basic time properties :
		this.applWakeUpTime = Time.now();
		this.lastPeriodicCallTime = new Clock(this.applWakeUpTime.get());
	}
	
	/**
	 * @brief Internal API that used only with Object toi reference itself in the manager.
	 * @note The manager remove the object when the refecence Low down 1 (last keeper)
	 * @param[in] _object Reference shared pointer on the object
	 */
	public synchronized void add(final EwolObject _object) {
		if (_object == null) {
			Log.error("try to add an inexistant Object in manager");
		}
		this.eObjectList.add(new WeakReference<>(_object));
	}
	
	/**
	 * @brief clean the weak pointer list (remove weak_ptr that is remoed)
	 */
	public synchronized void cleanInternalRemoved() {
		final int nbObject = this.eObjectList.size();
		Log.verbose("Clean Object List (if needed) : " + this.eObjectList.size() + " elements");
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
	 * @brief Display all object Open.
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
	 * @brief Retrive an Object with his name
	 * @param[in] _name Name of the Object
	 * @return Pointer on the finded Object.
	 */
	public synchronized EwolObject get(final String _name) {
		if (_name.isEmpty() == true) {
			return null;
		}
		for (final WeakReference<EwolObject> it : this.eObjectList) {
			final EwolObject element = it.get();
			if (element != null && element.getName().contentEquals(_name)) {
				return element;
			}
		}
		return null;
	}
	
	/**
	 * @brief Get the number of loaded object in the system
	 * @return number of Object
	 */
	public synchronized int getNumberObject() {
		return this.eObjectList.size();
	}
	
	/**
	 * @brief retrive an object with his name
	 * @param[in] _name Name of the object
	 * @return the requested object or null
	 */
	public synchronized EwolObject getObjectNamed(final String _name) {
		return get(_name);
	}
	
	/**
	 * @brief Call every time we can with the current time
	 * @param[in] _localTime Current system Time.
	 */
	public synchronized void timeCall(final Clock _localTime) {
		final Clock previousTime = this.lastPeriodicCallTime;
		this.lastPeriodicCallTime = _localTime;
		if (this.periodicCall.size() <= 0) {
			return;
		}
		final Duration deltaTime = new Duration(_localTime.get() - previousTime.get());
		
		final EventTime myTime = new EventTime(_localTime, this.applWakeUpTime.toClock(), deltaTime, deltaTime);
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
	 * @brief If the application is suspended The Ewol Object manager does not know it, just call this to update delta call
	 * @param[in] _localTime Current system Time.
	 */
	public synchronized void timeCallResume(final Clock _localTime) {
		this.lastPeriodicCallTime = _localTime;
	}
	
	/**
	 * @brief remove all resources (un-init) out of the destructor (due to the system implementation)
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
	 * @brief Add a worker on the system list.
	 * @param[in] _worker Worker to add in the list.
	 */
	public synchronized void workerAdd(final EwolObject _worker) {
		this.workerList.add(_worker);
	}
	
	/**
	 * @brief Remove a worker on the system list.
	 * @param[in] _worker Worker to add in the list.
	 */
	public synchronized void workerRemove(final EwolObject _worker) {
		
		final Iterator<EwolObject> iterator = this.workerList.iterator();
		while (iterator.hasNext()) {
			final EwolObject elem = iterator.next();
			if (elem == _worker) {
				iterator.remove();
			}
		}
	}
	
}
