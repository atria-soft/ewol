package org.atriasoft.ephysics.collision;

import org.atriasoft.ephysics.constraint.ContactPoint;
import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Vector3f;

/**
 *  This class represents a set of one or several contact manifolds. Typically a
 * convex/convex collision will have a set with a single manifold and a convex-concave
 * collision can have more than one manifolds. Note that a contact manifold can
 * contains several contact points.
 */
public class ContactManifoldSet {
	
	private static final int MAX_MANIFOLDS_IN_CONTACT_MANIFOLD_SET = 3; // Maximum number of contact manifolds in the set
	private static final int CONTACT_CUBEMAP_FACE_NB_SUBDIVISIONS = 3; // N Number for the N x N subdivisions of the cubemap
	
	private final int nbMaxManifolds; //!< Maximum number of contact manifolds in the set
	private int nbManifolds; //!< Current number of contact manifolds in the set
	private final ProxyShape shape1; //!< Pointer to the first proxy shape of the contact
	private final ProxyShape shape2; //!< Pointer to the second proxy shape of the contact
	
	private final ContactManifold[] manifolds = new ContactManifold[MAX_MANIFOLDS_IN_CONTACT_MANIFOLD_SET]; //!< Contact manifolds of the set
	/// Create a new contact manifold and add it to the set
	
	/// Constructor
	public ContactManifoldSet(final ProxyShape _shape1, final ProxyShape _shape2, final int _nbMaxManifolds) {
		this.nbMaxManifolds = _nbMaxManifolds;
		this.nbManifolds = 0;
		this.shape1 = _shape1;
		this.shape2 = _shape2;
		assert (_nbMaxManifolds >= 1);
	}
	
	/// Add a contact point to the manifold set
	public void addContactPoint(final ContactPoint contact) {
		// Compute an Id corresponding to the normal direction (using a cubemap)
		final int normalDirectionId = computeCubemapNormalId(contact.getNormal());
		// If there is no contact manifold yet
		if (this.nbManifolds == 0) {
			createManifold(normalDirectionId);
			this.manifolds[0].addContactPoint(contact);
			assert (this.manifolds[this.nbManifolds - 1].getNbContactPoints() > 0);
			for (int iii = 0; iii < this.nbManifolds; iii++) {
				assert (this.manifolds[iii].getNbContactPoints() > 0);
			}
			return;
		}
		// Select the manifold with the most similar normal (if exists)
		int similarManifoldIndex = 0;
		if (this.nbMaxManifolds > 1) {
			similarManifoldIndex = selectManifoldWithSimilarNormal(normalDirectionId);
		}
		// If a similar manifold has been found
		if (similarManifoldIndex != -1) {
			// Add the contact point to that similar manifold
			this.manifolds[similarManifoldIndex].addContactPoint(contact);
			assert (this.manifolds[similarManifoldIndex].getNbContactPoints() > 0);
			return;
		}
		// If the maximum number of manifold has not been reached yet
		if (this.nbManifolds < this.nbMaxManifolds) {
			// Create a new manifold for the contact point
			createManifold(normalDirectionId);
			this.manifolds[this.nbManifolds - 1].addContactPoint(contact);
			for (int iii = 0; iii < this.nbManifolds; iii++) {
				assert (this.manifolds[iii].getNbContactPoints() > 0);
			}
			return;
		}
		// The contact point will be in a new contact manifold, we now have too much
		// manifolds condidates. We need to remove one. We choose to keep the manifolds
		// with the largest contact depth among their points
		int smallestDepthIndex = -1;
		float minDepth = contact.getPenetrationDepth();
		assert (this.nbManifolds == this.nbMaxManifolds);
		for (int iii = 0; iii < this.nbManifolds; iii++) {
			final float depth = this.manifolds[iii].getLargestContactDepth();
			if (depth < minDepth) {
				minDepth = depth;
				smallestDepthIndex = iii;
			}
		}
		// If we do not want to keep to new manifold (not created yet) with the
		// new contact point
		if (smallestDepthIndex == -1) {
			return;
		}
		assert (smallestDepthIndex >= 0 && smallestDepthIndex < this.nbManifolds);
		// Here we need to replace an existing manifold with a new one (that contains
		// the new contact point)
		removeManifold(smallestDepthIndex);
		createManifold(normalDirectionId);
		this.manifolds[this.nbManifolds - 1].addContactPoint(contact);
		assert (this.manifolds[this.nbManifolds - 1].getNbContactPoints() > 0);
		for (int iii = 0; iii < this.nbManifolds; iii++) {
			assert (this.manifolds[iii].getNbContactPoints() > 0);
		}
		return;
	}
	
	/// Clear the contact manifold set
	public void clear() {
		for (int iii = this.nbManifolds - 1; iii >= 0; iii--) {
			removeManifold(iii);
		}
		assert (this.nbManifolds == 0);
	}
	
	// Map the normal vector longo a cubemap face bucket (a face contains 4x4 buckets)
	// Each face of the cube is divided longo 4x4 buckets. This method maps the
	// normal vector longo of the of the bucket and returns a unique Id for the bucket
	private int computeCubemapNormalId(final Vector3f normal) {
		assert (normal.length2() > Constant.FLOAT_EPSILON);
		int faceNo;
		float u, v;
		final float max = FMath.max(FMath.abs(normal.x), FMath.abs(normal.y), FMath.abs(normal.z));
		final Vector3f normalScaled = normal.divideNew(max);
		if (normalScaled.x >= normalScaled.y && normalScaled.x >= normalScaled.z) {
			faceNo = normalScaled.x > 0 ? 0 : 1;
			u = normalScaled.y;
			v = normalScaled.z;
		} else if (normalScaled.y >= normalScaled.x && normalScaled.y >= normalScaled.z) {
			faceNo = normalScaled.y > 0 ? 2 : 3;
			u = normalScaled.x;
			v = normalScaled.z;
		} else {
			faceNo = normalScaled.z > 0 ? 4 : 5;
			u = normalScaled.x;
			v = normalScaled.y;
		}
		int indexU = FMath.floor(((u + 1) / 2) * CONTACT_CUBEMAP_FACE_NB_SUBDIVISIONS);
		int indexV = FMath.floor(((v + 1) / 2) * CONTACT_CUBEMAP_FACE_NB_SUBDIVISIONS);
		if (indexU == CONTACT_CUBEMAP_FACE_NB_SUBDIVISIONS) {
			indexU--;
		}
		if (indexV == CONTACT_CUBEMAP_FACE_NB_SUBDIVISIONS) {
			indexV--;
		}
		final int nbSubDivInFace = CONTACT_CUBEMAP_FACE_NB_SUBDIVISIONS * CONTACT_CUBEMAP_FACE_NB_SUBDIVISIONS;
		return faceNo * 200 + indexU * nbSubDivInFace + indexV;
	}
	
	private void createManifold(final int normalDirectionId) {
		assert (this.nbManifolds < this.nbMaxManifolds);
		this.manifolds[this.nbManifolds] = new ContactManifold(this.shape1, this.shape2, normalDirectionId);
		this.nbManifolds++;
	}
	
	/// Return a given contact manifold
	public ContactManifold getContactManifold(final int index) {
		assert (index >= 0 && index < this.nbManifolds);
		return this.manifolds[index];
	}
	
	/// Return the number of manifolds in the set
	public long getNbContactManifolds() {
		return this.nbManifolds;
	}
	
	/// Return the first proxy shape
	public ProxyShape getShape1() {
		return this.shape1;
	}
	
	/// Return the second proxy shape
	public ProxyShape getShape2() {
		return this.shape2;
	}
	
	/// Return the total number of contact points in the set of manifolds
	public int getTotalNbContactPoints() {
		int nbPoints = 0;
		for (int iii = 0; iii < this.nbManifolds; iii++) {
			nbPoints += this.manifolds[iii].getNbContactPoints();
		}
		return nbPoints;
	}
	
	/// Remove a contact manifold from the set
	private void removeManifold(final int index) {
		assert (this.nbManifolds > 0);
		assert (index >= 0 && index < this.nbManifolds);
		// Delete the new contact
		this.manifolds[index] = null;
		for (int iii = index; (iii + 1) < this.nbManifolds; iii++) {
			this.manifolds[iii] = this.manifolds[iii + 1];
		}
		this.nbManifolds--;
	}
	
	// Return the index of the contact manifold with a similar average normal.
	private int selectManifoldWithSimilarNormal(final int normalDirectionId) {
		// Return the Id of the manifold with the same normal direction id (if exists)
		for (int iii = 0; iii < this.nbManifolds; iii++) {
			if (normalDirectionId == this.manifolds[iii].getNormalDirectionId()) {
				return iii;
			}
		}
		return -1;
	}
	
	/// Update the contact manifolds
	public void update() {
		for (int iii = this.nbManifolds - 1; iii >= 0; iii--) {
			// Update the contact manifold
			this.manifolds[iii].update(this.shape1.getBody().getTransform().multiplyNew(this.shape1.getLocalToBodyTransform()),
					this.shape2.getBody().getTransform().multiplyNew(this.shape2.getLocalToBodyTransform()));
			// Remove the contact manifold if has no contact points anymore
			if (this.manifolds[iii].getNbContactPoints() == 0) {
				removeManifold(iii);
			}
		}
	}
	
}
