package org.atriasoft.ephysics.engine;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.ephysics.body.BodyType;
import org.atriasoft.ephysics.body.RigidBody;
import org.atriasoft.ephysics.collision.ContactManifold;
import org.atriasoft.ephysics.constraint.Joint;
import org.atriasoft.ephysics.internal.Log;

/**
 *  An island represent an isolated group of awake bodies that are connected with each other by
 * some contraints (contacts or joints).
 */
public class Island {
	private final List<RigidBody> bodies = new ArrayList<>(); //!< Array with all the bodies of the island
	private final List<ContactManifold> contactManifolds = new ArrayList<>(); //!< Array with all the contact manifolds between bodies of the island
	private final List<Joint> joints = new ArrayList<>(); //!< Array with all the joints between bodies of the island
	
	/**
	 *  Constructor
	 */
	public Island(final int nbMaxBodies, final int nbMaxContactManifolds, final int nbMaxJoints) {
		// Allocate memory for the arrays
		//this.bodies.reserve(_nbMaxBodies);
		//this.contactManifolds.reserve(_nbMaxContactManifolds);
		//this.joints.reserve(_nbMaxJoints);
	}
	
	/** 
		 * Add a body.
		 */
	public void addBody(final RigidBody _body) {
		if (_body.isSleeping() == true) {
			Log.error("Try to add a body that is sleeping ...");
			return;
		}
		this.bodies.add(_body);
	}
	
	/** 
		 * Add a contact manifold.
		 */
	public void addContactManifold(final ContactManifold _contactManifold) {
		this.contactManifolds.add(_contactManifold);
	}
	
	/** 
		 * Add a joint.
		 */
	public void addJoint(final Joint _joint) {
		this.joints.add(_joint);
	}
	
	/** 
	 * Return a pointer to the array of bodies
	 */
	public List<RigidBody> getBodies() {
		return this.bodies;
	}
	
	/** 
	 * Return a pointer to the array of contact manifolds
	 */
	public List<ContactManifold> getContactManifold() {
		return this.contactManifolds;
	}
	
	/** 
	 * Return a pointer to the array of joints
	 */
	public List<Joint> getJoints() {
		return this.joints;
	}
	
	/** 
	 *  Get the number of body
	 * @return Number of bodies.
	 */
	public int getNbBodies() {
		return this.bodies.size();
	}
	
	/** 
	 * @ Get the number of contact manifolds
	 * Return the number of contact manifolds in the island
	 */
	public int getNbContactManifolds() {
		return this.contactManifolds.size();
	}
	
	/** 
	 * Return the number of joints in the island
	 */
	public int getNbJoints() {
		return this.joints.size();
	}
	
	/**
	 *  Reset the isAlreadyIsland variable of the static bodies so that they can also be included in the other islands
	 */
	public void resetStaticBobyNotInIsland() {
		for (final RigidBody it : this.bodies) {
			if (it.getType() == BodyType.STATIC) {
				it.isAlreadyInIsland = false;
			}
		}
	}
	
}
