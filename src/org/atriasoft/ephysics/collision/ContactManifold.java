/** @file
 * Original ReactPhysics3D C++ library by Daniel Chappuis <http://www.reactphysics3d.com/> This code is re-licensed with permission from ReactPhysics3D author.
 * @author Daniel CHAPPUIS
 * @author Edouard DUPIN
 * @copyright 2010-2016, Daniel Chappuis
 * @copyright 2017, Edouard DUPIN
 * @license MPL v2.0 (see license file)
 */

package org.atriasoft.ephysics.collision;

import org.atriasoft.ephysics.Configuration;
import org.atriasoft.ephysics.body.CollisionBody;
import org.atriasoft.ephysics.constraint.ContactPoint;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

/**
 *  This class represents the set of contact points between two bodies.
 * The contact manifold is implemented in a way to cache the contact
 * points among the frames for better stability following the
 * "Contact Generation" presentation of Erwin Coumans at GDC 2010
 * conference (bullet.googlecode.com/files/GDC10_Coumans_Erwin_Contact.pdf).
 * Some code of this class is based on the implementation of the
 * btPersistentManifold class from Bullet physics engine (www.http://bulletphysics.org).
 * The contacts between two bodies are added one after the other in the cache.
 * When the cache is full, we have to remove one point. The idea is to keep
 * the point with the deepest penetration depth and also to keep the
 * points producing the larger area (for a more stable contact manifold).
 * The new added point is always kept.
 */
public class ContactManifold {
	
	//!< Maximum number of contacts in the manifold
	public static final int MAX_CONTACT_POINTS_IN_MANIFOLD = 4;;
	
	private final ProxyShape shape1; //!< Pointer to the first proxy shape of the contact
	
	private final ProxyShape shape2; //!< Pointer to the second proxy shape of the contact
	private final ContactPoint[] contactPoints = new ContactPoint[MAX_CONTACT_POINTS_IN_MANIFOLD]; //!< Contact points in the manifold
	private final int normalDirectionId; //!< Normal direction Id (Unique Id representing the normal direction)
	private int nbContactPoints; //!< Number of contacts in the cache
	private Vector3f frictionVector1 = new Vector3f(); //!< First friction vector of the contact manifold
	private Vector3f frictionvec2 = new Vector3f(); //!< Second friction vector of the contact manifold
	private float frictionImpulse1; //!< First friction raint accumulated impulse
	
	private float frictionImpulse2; //!< Second friction raint accumulated impulse
	private float frictionTwistImpulse; //!< Twist friction raint accumulated impulse
	private Vector3f rollingResistanceImpulse; //!< Accumulated rolling resistance impulse
	public boolean isAlreadyInIsland; //!< True if the contact manifold has already been added longo an island
	/// Return the index of maximum area
	/// Constructor
	
	public ContactManifold(final ProxyShape _shape1, final ProxyShape _shape2, final int _normalDirectionId) {
		this.shape1 = _shape1;
		this.shape2 = _shape2;
		this.normalDirectionId = _normalDirectionId;
		this.nbContactPoints = 0;
		this.frictionImpulse1 = 0.0f;
		this.frictionImpulse2 = 0.0f;
		this.frictionTwistImpulse = 0.0f;
		this.isAlreadyInIsland = false;
		
	}
	
	/// Add a contact point to the manifold
	public void addContactPoint(final ContactPoint contact) {
		// For contact already in the manifold
		for (int iii = 0; iii < this.nbContactPoints; iii++) {
			// Check if the new point point does not correspond to a same contact point
			// already in the manifold.
			final float distance = (this.contactPoints[iii].getWorldPointOnBody1().lessNew(contact.getWorldPointOnBody1()).length2());
			if (distance <= Configuration.PERSISTENT_CONTACT_DIST_THRESHOLD * Configuration.PERSISTENT_CONTACT_DIST_THRESHOLD) {
				assert (this.nbContactPoints > 0);
				return;
			}
		}
		// If the contact manifold is full
		if (this.nbContactPoints == MAX_CONTACT_POINTS_IN_MANIFOLD) {
			final int indexMaxPenetration = getIndexOfDeepestPenetration(contact);
			final int indexToRemove = getIndexToRemove(indexMaxPenetration, contact.getLocalPointOnBody1());
			removeContactPoint(indexToRemove);
		}
		// Add the new contact point in the manifold
		this.contactPoints[this.nbContactPoints] = contact;
		this.nbContactPoints++;
		assert (this.nbContactPoints > 0);
	}
	
	/// Clear the contact manifold
	public void clear() {
		for (int iii = 0; iii < this.nbContactPoints; ++iii) {
			this.contactPoints[iii] = null;
		}
		this.nbContactPoints = 0;
	}
	
	/// Return the normalized averaged normal vector
	public Vector3f getAverageContactNormal() {
		if (this.nbContactPoints == 0) {
			return new Vector3f(0, 0, 1);
		}
		final Vector3f averageNormal = new Vector3f();
		for (int iii = 0; iii < this.nbContactPoints; iii++) {
			averageNormal.add(this.contactPoints[iii].getNormal());
		}
		return averageNormal.safeNormalizeNew();
	}
	
	/// Return a pointer to the first body of the contact manifold
	public CollisionBody getBody1() {
		return this.shape1.getBody();
	}
	
	/// Return a pointer to the second body of the contact manifold
	public CollisionBody getBody2() {
		return this.shape2.getBody();
	}
	
	/// Return a contact point of the manifold
	public ContactPoint getContactPoint(final int index) {
		assert (index < this.nbContactPoints);
		return this.contactPoints[index];
	}
	
	/// Return the first friction accumulated impulse
	public float getFrictionImpulse1() {
		return this.frictionImpulse1;
	}
	
	/// Return the second friction accumulated impulse
	public float getFrictionImpulse2() {
		return this.frictionImpulse2;
	}
	
	/// Return the friction twist accumulated impulse
	public float getFrictionTwistImpulse() {
		return this.frictionTwistImpulse;
	}
	
	/// Return the second friction vector at the center of the contact manifold
	public Vector3f getFrictionvec2() {
		return this.frictionvec2;
	}
	
	/// Return the first friction vector at the center of the contact manifold
	public Vector3f getFrictionVector1() {
		return this.frictionVector1;
	}
	
	/**
	 *  Return the index of the contact with the larger penetration depth.
	 *
	 * This corresponding contact will be kept in the cache. The method returns -1 is
	 * the new contact is the deepest.
	 */
	int getIndexOfDeepestPenetration(final ContactPoint newContact) {
		assert (this.nbContactPoints == MAX_CONTACT_POINTS_IN_MANIFOLD);
		int indexMaxPenetrationDepth = -1;
		float maxPenetrationDepth = newContact.getPenetrationDepth();
		// For each contact in the cache
		for (int iii = 0; iii < this.nbContactPoints; iii++) {
			// If the current contact has a larger penetration depth
			if (this.contactPoints[iii].getPenetrationDepth() > maxPenetrationDepth) {
				maxPenetrationDepth = this.contactPoints[iii].getPenetrationDepth();
				indexMaxPenetrationDepth = iii;
			}
		}
		// Return the index of largest penetration depth
		return indexMaxPenetrationDepth;
	}
	
	/**
	 *  Return the index that will be removed.
	 * The index of the contact point with the larger penetration
	 * depth is given as a parameter. This contact won't be removed. Given this contact, we compute
	 * the different area and we want to keep the contacts with the largest area. The new point is also
	 * kept. In order to compute the area of a quadrilateral, we use the formula :
	 * Area = 0.5 * | AC x BD | where AC and BD form the diagonals of the quadrilateral. Note that
	 * when we compute this area, we do not calculate it exactly but we
	 * only estimate it because we do not compute the actual diagonals of the quadrialteral. Therefore,
	 * this is only a guess that is faster to compute. This idea comes from the Bullet Physics library
	 * by Erwin Coumans (http://wwww.bulletphysics.org).
	 */
	int getIndexToRemove(final int indexMaxPenetration, final Vector3f newPoint) {
		assert (this.nbContactPoints == MAX_CONTACT_POINTS_IN_MANIFOLD);
		float area0 = 0.0f; // Area with contact 1,2,3 and newPoint
		float area1 = 0.0f; // Area with contact 0,2,3 and newPoint
		float area2 = 0.0f; // Area with contact 0,1,3 and newPoint
		float area3 = 0.0f; // Area with contact 0,1,2 and newPoint
		if (indexMaxPenetration != 0) {
			// Compute the area
			final Vector3f vector1 = newPoint.lessNew(this.contactPoints[1].getLocalPointOnBody1());
			final Vector3f vector2 = this.contactPoints[3].getLocalPointOnBody1().lessNew(this.contactPoints[2].getLocalPointOnBody1());
			final Vector3f crossProduct = vector1.cross(vector2);
			area0 = crossProduct.length2();
		}
		if (indexMaxPenetration != 1) {
			// Compute the area
			final Vector3f vector1 = newPoint.lessNew(this.contactPoints[0].getLocalPointOnBody1());
			final Vector3f vector2 = this.contactPoints[3].getLocalPointOnBody1().lessNew(this.contactPoints[2].getLocalPointOnBody1());
			final Vector3f crossProduct = vector1.cross(vector2);
			area1 = crossProduct.length2();
		}
		if (indexMaxPenetration != 2) {
			// Compute the area
			final Vector3f vector1 = newPoint.lessNew(this.contactPoints[0].getLocalPointOnBody1());
			final Vector3f vector2 = this.contactPoints[3].getLocalPointOnBody1().lessNew(this.contactPoints[1].getLocalPointOnBody1());
			final Vector3f crossProduct = vector1.cross(vector2);
			area2 = crossProduct.length2();
		}
		if (indexMaxPenetration != 3) {
			// Compute the area
			final Vector3f vector1 = newPoint.lessNew(this.contactPoints[0].getLocalPointOnBody1());
			final Vector3f vector2 = this.contactPoints[2].getLocalPointOnBody1().lessNew(this.contactPoints[1].getLocalPointOnBody1());
			final Vector3f crossProduct = vector1.cross(vector2);
			area3 = crossProduct.length2();
		}
		// Return the index of the contact to remove
		return getMaxArea(area0, area1, area2, area3);
	}
	
	/// Return the largest depth of all the contact points
	public float getLargestContactDepth() {
		float largestDepth = 0.0f;
		for (int iii = 0; iii < this.nbContactPoints; iii++) {
			final float depth = this.contactPoints[iii].getPenetrationDepth();
			if (depth > largestDepth) {
				largestDepth = depth;
			}
		}
		return largestDepth;
	}
	
	private int getMaxArea(final float area0, final float area1, final float area2, final float area3) {
		if (area0 < area1) {
			if (area1 < area2) {
				if (area2 < area3) {
					return 3;
				} else {
					return 2;
				}
			} else if (area1 < area3) {
				return 3;
			} else {
				return 1;
			}
		} else if (area0 < area2) {
			if (area2 < area3) {
				return 3;
			} else {
				return 2;
			}
		} else if (area0 < area3) {
			return 3;
		} else {
			return 0;
		}
	}
	
	/// Return the number of contact points in the manifold
	public int getNbContactPoints() {
		return this.nbContactPoints;
	}
	
	/// Return the normal direction Id
	public int getNormalDirectionId() {
		return this.normalDirectionId;
	}
	
	public Vector3f getRollingResistanceImpulse() {
		return this.rollingResistanceImpulse;
	}
	
	/// Return a pointer to the first proxy shape of the contact
	public ProxyShape getShape1() {
		return this.shape1;
	}
	
	/// Return a pointer to the second proxy shape of the contact
	public ProxyShape getShape2() {
		return this.shape2;
	}
	
	/// Return true if the contact manifold has already been added longo an island
	public boolean isAlreadyInIsland() {
		return this.isAlreadyInIsland;
	}
	
	/// Remove a contact point from the manifold
	public void removeContactPoint(final int index) {
		assert (index < this.nbContactPoints);
		assert (this.nbContactPoints > 0);
		this.contactPoints[index] = null;
		// If we don't remove the last index
		if (index < this.nbContactPoints - 1) {
			this.contactPoints[index] = this.contactPoints[this.nbContactPoints - 1];
		}
		this.nbContactPoints--;
	}
	
	/// Set the first friction accumulated impulse
	public void setFrictionImpulse1(final float frictionImpulse1) {
		this.frictionImpulse1 = frictionImpulse1;
	}
	
	/// Set the second friction accumulated impulse
	public void setFrictionImpulse2(final float frictionImpulse2) {
		this.frictionImpulse2 = frictionImpulse2;
	}
	
	/// Set the friction twist accumulated impulse
	public void setFrictionTwistImpulse(final float frictionTwistImpulse) {
		this.frictionTwistImpulse = frictionTwistImpulse;
	}
	
	/// set the second friction vector at the center of the contact manifold
	public void setFrictionvec2(final Vector3f frictionvec2) {
		this.frictionvec2 = frictionvec2;
	}
	
	/// set the first friction vector at the center of the contact manifold
	public void setFrictionVector1(final Vector3f frictionVector1) {
		this.frictionVector1 = frictionVector1;
	}
	
	/// Set the accumulated rolling resistance impulse
	public void setRollingResistanceImpulse(final Vector3f rollingResistanceImpulse) {
		this.rollingResistanceImpulse = rollingResistanceImpulse;
	}
	
	/**
	 *  Update the contact manifold.
	 * 
	 * First the world space coordinates of the current contacts in the manifold are recomputed from
	 * the corresponding transforms of the bodies because they have moved. Then we remove the contacts
	 * with a negative penetration depth (meaning that the bodies are not penetrating anymore) and also
	 * the contacts with a too large distance between the contact points in the plane orthogonal to the
	 * contact normal.
	 */
	public void update(final Transform3D transform1, final Transform3D transform2) {
		if (this.nbContactPoints == 0) {
			return;
		}
		// Update the world coordinates and penetration depth of the contact points in the manifold
		for (int iii = 0; iii < this.nbContactPoints; iii++) {
			this.contactPoints[iii].setWorldPointOnBody1(transform1.multiplyNew(this.contactPoints[iii].getLocalPointOnBody1()));
			this.contactPoints[iii].setWorldPointOnBody2(transform2.multiplyNew(this.contactPoints[iii].getLocalPointOnBody2()));
			this.contactPoints[iii]
					.setPenetrationDepth((this.contactPoints[iii].getWorldPointOnBody1().lessNew(this.contactPoints[iii].getWorldPointOnBody2())).dot(this.contactPoints[iii].getNormal()));
		}
		final float squarePersistentContactThreshold = Configuration.PERSISTENT_CONTACT_DIST_THRESHOLD * Configuration.PERSISTENT_CONTACT_DIST_THRESHOLD;
		// Remove the contact points that don't represent very well the contact manifold
		for (int iii = (this.nbContactPoints) - 1; iii >= 0; iii--) {
			assert (iii < (this.nbContactPoints));
			// Compute the distance between contact points in the normal direction
			final float distanceNormal = -this.contactPoints[iii].getPenetrationDepth();
			// If the contacts points are too far from each other in the normal direction
			if (distanceNormal > squarePersistentContactThreshold) {
				removeContactPoint(iii);
			} else {
				// Compute the distance of the two contact points in the plane
				// orthogonal to the contact normal
				final Vector3f projOfPoint1 = this.contactPoints[iii].getNormal().multiplyNew(distanceNormal).add(this.contactPoints[iii].getWorldPointOnBody1());
				final Vector3f projDifference = this.contactPoints[iii].getWorldPointOnBody2().lessNew(projOfPoint1);
				// If the orthogonal distance is larger than the valid distance
				// threshold, we remove the contact
				if (projDifference.length2() > squarePersistentContactThreshold) {
					removeContactPoint(iii);
				}
			}
		}
	}
}
