package org.atriasoft.ephysics.engine;

import org.atriasoft.ephysics.constraint.ContactPointInfo;

/**
 *  This class can be used to receive event callbacks from the physics engine.
 * In order to receive callbacks, you need to create a new class that inherits from
 * this one and you must  the methods you need. Then, you need to register your
 * new event listener class to the physics world using the DynamicsWorld::setEventListener()
 * method.
 */
public interface EventListener {
	/**
	 *  Called when a new contact point is found between two bodies that were separated before
	 * @param contact Information about the contact
	 */
	void beginContact(ContactPointInfo contact);
	
	/**
	 *  Called at the beginning of an internal tick of the simulation step.
	 * Each time the DynamicsWorld::update() method is called, the physics
	 * engine will do several internal simulation steps. This method is
	 * called at the beginning of each internal simulation step.
	 */
	void beginInternalTick();
	
	/**
	 *  Called at the end of an internal tick of the simulation step.
	 * Each time the DynamicsWorld::update() metho is called, the physics
	 * engine will do several internal simulation steps. This method is
	 * called at the end of each internal simulation step.
	 */
	void endInternalTick();
	
	/**
	 *  Called when a new contact point is found between two bodies
	 * @param contact Information about the contact
	 */
	void newContact(ContactPointInfo contact);
}
