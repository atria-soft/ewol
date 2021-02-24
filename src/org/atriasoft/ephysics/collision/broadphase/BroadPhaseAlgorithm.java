/*
 * ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/
 * Copyright (c) 2010-2013 Daniel Chappuis
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * This file has been modified during the port to Java and differ from the source versions.
 */
package org.atriasoft.ephysics.collision.broadphase;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

import org.atriasoft.ephysics.Configuration;
import org.atriasoft.ephysics.RaycastTest;
import org.atriasoft.ephysics.collision.CollisionDetection;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.shapes.AABB;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Vector3f;

/**
 *  It represents the broad-phase collision detection. The
 * goal of the broad-phase collision detection is to compute the pairs of proxy shapes
 * that have their AABBs overlapping. Only those pairs of bodies will be tested
 * later for collision during the narrow-phase collision detection. A dynamic AABB
 * tree data structure is used for fast broad-phase collision detection.
 */
public class BroadPhaseAlgorithm {
	/**
	 * Callback called when the AABB of a leaf node is hit by a ray the
	 * broad-phase Dynamic AABB Tree.
	 */
	public class BroadPhaseRaycastCallback implements CallbackRaycast {
		private final DynamicAABBTree dynamicAABBTree;
		private final int raycastWithCategoryMaskBits;
		private final RaycastTest raycastTest;
		
		// Constructor
		public BroadPhaseRaycastCallback(final DynamicAABBTree _dynamicAABBTree, final int _raycastWithCategoryMaskBits, final RaycastTest _raycastTest) {
			this.dynamicAABBTree = _dynamicAABBTree;
			this.raycastWithCategoryMaskBits = _raycastWithCategoryMaskBits;
			this.raycastTest = _raycastTest;
			
		}
		
		@Override
		public float callback(final DTree _node, final Ray _ray) {
			float hitFraction = -1.0f;
			// Get the proxy shape from the node
			final ProxyShape proxyShape = (ProxyShape) this.dynamicAABBTree.getNodeDataPointer(_node);
			// Check if the raycast filtering mask allows raycast against this shape
			if ((this.raycastWithCategoryMaskBits & proxyShape.getCollisionCategoryBits()) != 0) {
				// Ask the collision detection to perform a ray cast test against
				// the proxy shape of this node because the ray is overlapping
				// with the shape in the broad-phase
				hitFraction = this.raycastTest.raycastAgainstShape(proxyShape, _ray);
			}
			return hitFraction;
		}
	};
	
	/// A tree of data that is separated by a specific distance in AABB model.
	protected DynamicAABBTree dynamicAABBTree;
	/** 
	 * Array with the broad-phase IDs of all collision shapes that have moved (or have been created) during the last simulation step.
	 * Those are the shapes that need to be tested for overlapping in the next simulation step. ==> and re-index in the overlapping tree
	 */
	protected List<DTree> movedShapes = new ArrayList<>();
	/**
	 * Temporary array of potential overlapping pairs (with potential duplicates)
	 */
	protected List<PairDTree> potentialPairs = new ArrayList<>();
	/**
	 * Reference to the collision detection object
	 */
	protected CollisionDetection collisionDetection; // TODO revoked with generic interface for the callback...
	
	public BroadPhaseAlgorithm(final CollisionDetection _collisionDetection) {
		this.dynamicAABBTree = new DynamicAABBTree(Configuration.DYNAMIC_TREE_AABB_GAP);
		this.collisionDetection = _collisionDetection;
	}
	
	/// Add a collision shape in the array of shapes that have moved in the last simulation step
	/// and that need to be tested again for broad-phase overlapping.
	public void addMovedCollisionShape(final DTree _broadPhaseID) {
		//Log.info("  ** Element that has moved ... = " + _broadPhaseID);
		this.movedShapes.add(_broadPhaseID);
	}
	
	/// Add a proxy collision shape longo the broad-phase collision detection
	public void addProxyCollisionShape(final ProxyShape _proxyShape, final AABB _aabb) {
		// Add the collision shape longo the dynamic AABB tree and get its broad-phase ID
		final DTree nodeId = this.dynamicAABBTree.addObject(_aabb, _proxyShape);
		// Set the broad-phase ID of the proxy shape
		_proxyShape.setBroadPhaseID(nodeId);
		// Add the collision shape longo the array of bodies that have moved (or have been created)
		// during the last simulation step
		addMovedCollisionShape(_proxyShape.getBroadPhaseID());
	}
	
	/// Compute all the overlapping pairs of collision shapes
	public void computeOverlappingPairs() {
		this.potentialPairs.clear();
		// For all collision shapes that have moved (or have been created) during the
		// last simulation step
		//Log.info("moved shape = " + this.movedShapes.size());
		/*
		for (final DTree it : this.movedShapes) {
			Log.info("    - " + it);
		}
		*/
		for (final DTree it : this.movedShapes) {
			// Get the AABB of the shape
			final AABB shapeAABB = this.dynamicAABBTree.getFatAABB(it);
			final BroadPhaseAlgorithm self = this;
			// Ask the dynamic AABB tree to report all collision shapes that overlap with
			// this AABB. The method BroadPhase::notifiyOverlappingPair() will be called
			// by the dynamic AABB tree for each potential overlapping pair.
			this.dynamicAABBTree.reportAllShapesOverlappingWithAABB(shapeAABB, new CallbackOverlapping() {
				@Override
				public void callback(final DTree _nodeId) {
					// TODO Auto-generated method stub// If both the nodes are the same, we do not create store the overlapping pair
					if (it == _nodeId) {
						return;
					}
					// Add the new potential pair longo the array of potential overlapping pairs
					self.potentialPairs.add(new PairDTree(it, _nodeId));
				}
			});
		}
		//Log.print("Find potential pair : " + this.potentialPairs.size());
		// Reset the array of collision shapes that have move (or have been created) during the last simulation step
		this.movedShapes.clear();
		// Sort the array of potential overlapping pairs in order to remove duplicate pairs
		this.potentialPairs.sort(new Comparator<PairDTree>() {
			@Override
			public int compare(final PairDTree _pair1, final PairDTree _pair2) {
				if (_pair1.first.uid < _pair2.first.uid) {
					return -1;
				}
				if (_pair1.first.uid == _pair2.first.uid) {
					if (_pair1.second.uid < _pair2.second.uid) {
						return -1;
					}
					if (_pair1.second.uid == _pair2.second.uid) {
						return 0;
					}
					return +1;
				}
				return +1;
			}
		});
		// Check all the potential overlapping pairs avoiding duplicates to report unique
		// overlapping pairs
		int iii = 0;
		while (iii < this.potentialPairs.size()) {
			// Get a potential overlapping pair
			final PairDTree pair = this.potentialPairs.get(iii);
			++iii;
			// Get the two collision shapes of the pair
			final ProxyShape shape1 = (ProxyShape) (this.dynamicAABBTree.getNodeDataPointer(pair.first));
			final ProxyShape shape2 = (ProxyShape) (this.dynamicAABBTree.getNodeDataPointer(pair.second));
			// Notify the collision detection about the overlapping pair
			this.collisionDetection.broadPhaseNotifyOverlappingPair(shape1, shape2);
			// Skip the duplicate overlapping pairs
			while (iii < this.potentialPairs.size()) {
				// Get the next pair
				final PairDTree nextPair = this.potentialPairs.get(iii);
				// If the next pair is different from the previous one, we stop skipping pairs
				// TODO check if it work without uid ...
				if (nextPair.first.uid != pair.first.uid || nextPair.second.uid != pair.second.uid) {
					break;
				}
				++iii;
			}
		}
	}
	
	/// Ray casting method
	public void raycast(final Ray _ray, final RaycastTest _raycastTest, final int _raycastWithCategoryMaskBits) {
		final BroadPhaseRaycastCallback broadPhaseRaycastCallback = new BroadPhaseRaycastCallback(this.dynamicAABBTree, _raycastWithCategoryMaskBits, _raycastTest);
		this.dynamicAABBTree.raycast(_ray, broadPhaseRaycastCallback);
	}
	
	/// Remove a collision shape from the array of shapes that have moved in the last simulation
	/// step and that need to be tested again for broad-phase overlapping.
	public void removeMovedCollisionShape(final DTree _broadPhaseID) {
		final Iterator<DTree> it = this.movedShapes.iterator();
		while (it.hasNext()) {
			final DTree elem = it.next();
			if (elem == _broadPhaseID) {
				it.remove();
			}
		}
	}
	
	/// Remove a proxy collision shape from the broad-phase collision detection
	public void removeProxyCollisionShape(final ProxyShape _proxyShape) {
		final DTree broadPhaseID = _proxyShape.getBroadPhaseID();
		// Remove the collision shape from the dynamic AABB tree
		this.dynamicAABBTree.removeObject(broadPhaseID);
		// Remove the collision shape longo the array of shapes that have moved (or have been created)
		// during the last simulation step
		removeMovedCollisionShape(broadPhaseID);
	}
	
	/// Return true if the two broad-phase collision shapes are overlapping
	public boolean testOverlappingShapes(final ProxyShape _shape1, final ProxyShape _shape2) {
		if (_shape1 == _shape2) {
			return false;
		}
		// Get the two AABBs of the collision shapes
		final AABB aabb1 = this.dynamicAABBTree.getFatAABB(_shape1.getBroadPhaseID());
		final AABB aabb2 = this.dynamicAABBTree.getFatAABB(_shape2.getBroadPhaseID());
		// Check if the two AABBs are overlapping
		return aabb1.testCollision(aabb2);
	}
	
	/// Notify the broad-phase that a collision shape has moved and need to be updated
	public void updateProxyCollisionShape(final ProxyShape _proxyShape, final AABB _aabb, final Vector3f _displacement) {
		updateProxyCollisionShape(_proxyShape, _aabb, _displacement, false);
	}
	
	public void updateProxyCollisionShape(final ProxyShape _proxyShape, final AABB _aabb, final Vector3f _displacement, final boolean _forceReinsert) {
		final DTree broadPhaseID = _proxyShape.getBroadPhaseID();
		// Update the dynamic AABB tree according to the movement of the collision shape
		final boolean hasBeenReInserted = this.dynamicAABBTree.updateObject(broadPhaseID, _aabb, _displacement, _forceReinsert);
		// If the collision shape has moved out of its fat AABB (and therefore has been reinserted
		//Log.error("                             ==> hasBeenReInserted = " + hasBeenReInserted);
		// longo the tree).
		if (hasBeenReInserted) {
			// Add the collision shape longo the array of shapes that have moved (or have been created)
			// during the last simulation step
			addMovedCollisionShape(broadPhaseID);
		}
	}
};