package org.atriasoft.ephysics.engine;

import org.atriasoft.ephysics.body.CollisionBody;
import org.atriasoft.ephysics.collision.ContactManifoldSet;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.broadphase.PairDTree;
import org.atriasoft.ephysics.constraint.ContactPoint;
import org.atriasoft.ephysics.mathematics.PairInt;
import org.atriasoft.etk.math.Vector3f;

/**
 *  This class represents a pair of two proxy collision shapes that are overlapping
 * during the broad-phase collision detection. It is created when
 * the two proxy collision shapes start to overlap and is destroyed when they do not
 * overlap anymore. This class contains a contact manifold that
 * store all the contact points between the two bodies.
 */
public class OverlappingPair {
	/// Return the pair of bodies index of the pair
	public static PairInt computeBodiesIndexPair(final CollisionBody body1, final CollisionBody body2) {
		// Construct the pair of body index
		final PairInt indexPair = body1.getID() < body2.getID() ? new PairInt(body1.getID(), body2.getID()) : new PairInt(body2.getID(), body1.getID());
		assert (indexPair.first != indexPair.second);
		return indexPair;
	}
	
	/// Return the pair of bodies index
	public static PairDTree computeID(final ProxyShape shape1, final ProxyShape shape2) {
		assert (shape1.getBroadPhaseID() != null && shape2.getBroadPhaseID() != null);
		// Construct the pair of body index
		return new PairDTree(shape1.getBroadPhaseID(), shape2.getBroadPhaseID());
	}
	
	private final ContactManifoldSet contactManifoldSet; //!< Set of persistent contact manifolds
	
	private Vector3f cachedSeparatingAxis; //!< Cached previous separating axis
	
	/// Constructor
	public OverlappingPair(final ProxyShape shape1, final ProxyShape shape2, final int nbMaxContactManifolds) {
		this.contactManifoldSet = new ContactManifoldSet(shape1, shape2, nbMaxContactManifolds);
		this.cachedSeparatingAxis = new Vector3f(1.0f, 1.0f, 1.0f);
	}
	
	/// Add a contact to the contact cache
	public void addContact(final ContactPoint contact) {
		this.contactManifoldSet.addContactPoint(contact);
	}
	
	/// Clear the contact points of the contact manifold
	public void clearContactPoints() {
		this.contactManifoldSet.clear();
	}
	
	/// Return the cached separating axis
	public Vector3f getCachedSeparatingAxis() {
		return this.cachedSeparatingAxis;
	}
	
	/// Return the a reference to the contact manifold set
	public ContactManifoldSet getContactManifoldSet() {
		return this.contactManifoldSet;
	}
	
	/// Return the number of contacts in the cache
	public int getNbContactPoints() {
		return this.contactManifoldSet.getTotalNbContactPoints();
	}
	
	/// Return the pointer to first proxy collision shape
	public ProxyShape getShape1() {
		return this.contactManifoldSet.getShape1();
	}
	
	/// Return the pointer to second body
	public ProxyShape getShape2() {
		return this.contactManifoldSet.getShape2();
	}
	
	/// Set the cached separating axis
	public void setCachedSeparatingAxis(final Vector3f axis) {
		this.cachedSeparatingAxis = axis;
	}
	
	/// Update the contact cache
	public void update() {
		this.contactManifoldSet.update();
	}
}
