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
package org.atriasoft.ephysics.body;

import org.atriasoft.ephysics.RaycastInfo;
import org.atriasoft.ephysics.collision.ContactManifoldListElement;
import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.shapes.AABB;
import org.atriasoft.ephysics.collision.shapes.CollisionShape;
import org.atriasoft.ephysics.engine.CollisionWorld;
import org.atriasoft.ephysics.internal.Log;
import org.atriasoft.ephysics.mathematics.Ray;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

/**
 * This class represents a body that is able to collide with others bodies. This class inherits from the Body class.
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
public class CollisionBody extends Body {
	// Type of body (static, kinematic or dynamic)
	protected BodyType type;
	// Position and orientation of the body
	protected Transform3D transform;
	// First element of the linked list of proxy collision shapes of this body
	protected ProxyShape proxyCollisionShapes;
	// Number of collision shapes
	protected long numberCollisionShapes;
	// First element of the linked list of contact manifolds involving this body
	public ContactManifoldListElement contactManifoldsList;
	// Reference to the world the body belongs to
	protected CollisionWorld world;
	
	/**
	 * @brief Constructor
	 * @param[in] _transform The transform of the body
	 * @param[in] _world The physics world where the body is created
	 * @param[in] _id ID of the body
	 */
	public CollisionBody(final Transform3D _transform, final CollisionWorld _world, final int _id) {
		super(_id);
		this.type = BodyType.DYNAMIC;
		this.transform = _transform;
		this.proxyCollisionShapes = null;
		this.numberCollisionShapes = 0;
		this.contactManifoldsList = null;
		this.world = _world;
		
		//Log.debug("         set transform: " + _transform);
		if (Float.isNaN(_transform.getPosition().x)) {
			Log.critical("         set transform: " + _transform);
		}
		if (Float.isInfinite(_transform.getOrientation().z)) {
			Log.critical("         set transform: " + _transform);
		}
	}
	
	/**
	 * @brief Add a collision shape to the body. Note that you can share a collision shape between several bodies using the same collision shape instance to
	 * when you add the shape to the different bodies. Do not forget to delete the collision shape you have created at the end of your program.
	 * 
	 * This method will return a pointer to a new proxy shape. A proxy shape is an object that links a collision shape and a given body. You can use the
	 * returned proxy shape to get and set information about the corresponding collision shape for that body.
	 * @param[in] collisionShape A pointer to the collision shape you want to add to the body
	 * @param[in] transform The transformation of the collision shape that transforms the local-space of the collision shape int32_to the local-space of the body
	 * @return A pointer to the proxy shape that has been created to link the body to the new collision shape you have added.
	 */
	public ProxyShape addCollisionShape(final CollisionShape _collisionShape, final Transform3D _transform) {
		// Create a proxy collision shape to attach the collision shape to the body
		final ProxyShape proxyShape = new ProxyShape(this, _collisionShape, _transform, 1.0f);
		// Add it to the list of proxy collision shapes of the body
		if (this.proxyCollisionShapes == null) {
			this.proxyCollisionShapes = proxyShape;
		} else {
			proxyShape.next = this.proxyCollisionShapes;
			this.proxyCollisionShapes = proxyShape;
		}
		final AABB aabb = new AABB();
		_collisionShape.computeAABB(aabb, this.transform.multiplyNew(_transform));
		this.world.collisionDetection.addProxyCollisionShape(proxyShape, aabb);
		this.numberCollisionShapes++;
		return proxyShape;
	}
	
	/**
	 * @brief Ask the broad-phase to test again the collision shapes of the body for collision (as if the body has moved).
	 */
	protected void askForBroadPhaseCollisionCheck() {
		for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.getNext()) {
			this.world.getCollisionDetection().askForBroadPhaseCollisionCheck(shape);
		}
	}
	
	/**
	 * @brief Compute and return the AABB of the body by merging all proxy shapes AABBs
	 * @return The axis-aligned bounding box (AABB) of the body in world-space coordinates
	 */
	public AABB getAABB() {
		final AABB bodyAABB = new AABB();
		if (this.proxyCollisionShapes == null) {
			return bodyAABB;
		}
		//Log.info("tmp this.transform : " + this.transform);
		//Log.info("tmp this.proxyCollisionShapes.getLocalToBodyTransform() : " + this.proxyCollisionShapes.getLocalToBodyTransform());
		//Log.info("tmp this.transform.multiplyNew(this.proxyCollisionShapes.getLocalToBodyTransform()) : " + this.transform.multiplyNew(this.proxyCollisionShapes.getLocalToBodyTransform()));
		this.proxyCollisionShapes.getCollisionShape().computeAABB(bodyAABB, this.transform.multiplyNew(this.proxyCollisionShapes.getLocalToBodyTransform()));
		for (ProxyShape shape = this.proxyCollisionShapes.next; shape != null; shape = shape.next) {
			final AABB aabb = new AABB();
			shape.getCollisionShape().computeAABB(aabb, this.transform.multiplyNew(shape.getLocalToBodyTransform()));
			bodyAABB.mergeWithAABB(aabb);
		}
		//Log.info("tmp aabb : " + bodyAABB);
		//Log.info("tmp aabb : " + bodyAABB.getMax().lessNew(bodyAABB.getMin()));
		return bodyAABB;
	}
	
	/**
	 * @brief Get the first element of the linked list of contact manifolds involving this body
	 * @return A pointer to the first element of the linked-list with the contact manifolds of this body
	 */
	public ContactManifoldListElement getContactManifoldsList() {
		return this.contactManifoldsList;
	}
	
	/**
	 * @brief Get the body local-space coordinates of a point given in the world-space coordinates
	 * @param[in] _worldPoint A point in world-space coordinates
	 * @return The point in the local-space coordinates of the body
	 */
	public Vector3f getLocalPoint(final Vector3f _worldPoint) {
		return this.transform.inverseNew().multiply(_worldPoint);
	}
	
	/**
	 * @brief Get the body local-space coordinates of a vector given in the world-space coordinates
	 * @param[in] _worldVector A vector in world-space coordinates
	 * @return The vector in the local-space coordinates of the body
	 */
	public Vector3f getLocalVector(final Vector3f _worldVector) {
		return this.transform.getOrientation().inverseNew().multiply(_worldVector);
	}
	
	/**
	 * @brief Get the linked list of proxy shapes of that body
	 * @return The pointer of the first proxy shape of the linked-list of all the
	 *		 proxy shapes of the body
	 */
	public ProxyShape getProxyShapesList() {
		return this.proxyCollisionShapes;
	}
	
	/**
	 * @brief Return the current position and orientation
	 * @return The current transformation of the body that transforms the local-space of the body int32_to world-space
	 */
	public Transform3D getTransform() {
		return this.transform;
	}
	
	/**
	 * @brief Return the type of the body
	 * @return the type of the body (STATIC, KINEMATIC, DYNAMIC)
	 */
	public BodyType getType() {
		return this.type;
	}
	
	public CollisionWorld getWorld() {
		return this.world;
	}
	
	/**
	 * @brief Get the world-space coordinates of a point given the local-space coordinates of the body
	 * @param[in] _localPoint A point in the local-space coordinates of the body
	 * @return The point in world-space coordinates
	 */
	public Vector3f getWorldPoint(final Vector3f _localPoint) {
		return this.transform.multiplyNew(_localPoint);
	}
	
	/**
	 * @brief Get the world-space vector of a vector given in local-space coordinates of the body
	 * @param[in] _localVector A vector in the local-space coordinates of the body
	 * @return The vector in world-space coordinates
	 */
	public Vector3f getWorldVector(final Vector3f _localVector) {
		return this.transform.getOrientation().multiply(_localVector);
	}
	
	/**
	 * @brief Raycast method with feedback information
	 * The method returns the closest hit among all the collision shapes of the body
	 * @param[in] _ray The ray used to raycast agains the body
	 * @param[out] _raycastInfo Structure that contains the result of the raycasting (valid only if the method returned true)
	 * @return True if the ray hit the body and false otherwise
	 */
	public boolean raycast(final Ray _ray, final RaycastInfo _raycastInfo) {
		if (this.isActive == false) {
			return false;
		}
		boolean isHit = false;
		
		final Ray rayTemp = new Ray(_ray);
		for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.next) {
			// Test if the ray hits the collision shape
			if (shape.raycast(rayTemp, _raycastInfo)) {
				rayTemp.maxFraction = _raycastInfo.hitFraction;
				isHit = true;
			}
		}
		return isHit;
	}
	
	/**
	 * @brief Remove all the collision shapes
	 */
	public void removeAllCollisionShapes() {
		ProxyShape current = this.proxyCollisionShapes;
		// Look for the proxy shape that contains the collision shape in parameter
		while (current != null) {
			// Remove the proxy collision shape
			final ProxyShape nextElement = current.next;
			if (this.isActive) {
				this.world.collisionDetection.removeProxyCollisionShape(current);
			}
			// Get the next element in the list
			current = nextElement;
		}
		this.proxyCollisionShapes = null;
	}
	
	/**
	 * @brief Remove a collision shape from the body
	 * To remove a collision shape, you need to specify the pointer to the proxy shape that has been returned when you have added the collision shape to the body
	 * @param[in] _proxyShape The pointer of the proxy shape you want to remove
	 */
	public void removeCollisionShape(final ProxyShape _proxyShape) {
		ProxyShape current = this.proxyCollisionShapes;
		// If the the first proxy shape is the one to remove
		if (current == _proxyShape) {
			this.proxyCollisionShapes = current.next;
			if (this.isActive) {
				this.world.collisionDetection.removeProxyCollisionShape(current);
			}
			current = null;
			this.numberCollisionShapes--;
			return;
		}
		// Look for the proxy shape that contains the collision shape in parameter
		while (current.next != null) {
			// If we have found the collision shape to remove
			if (current.next == _proxyShape) {
				// Remove the proxy collision shape
				ProxyShape elementToRemove = current.next;
				current.next = elementToRemove.next;
				if (this.isActive) {
					this.world.collisionDetection.removeProxyCollisionShape(elementToRemove);
				}
				elementToRemove = null;
				this.numberCollisionShapes--;
				return;
			}
			// Get the next element in the list
			current = current.next;
		}
	}
	
	/**
	 * @brief Reset the contact manifold lists
	 */
	public void resetContactManifoldsList() {
		// Delete the linked list of contact manifolds of that body
		this.contactManifoldsList = null;
	}
	
	/**
	 * @brief Reset the this.isAlreadyInIsland variable of the body and contact manifolds.
	 * This method also returns the number of contact manifolds of the body.
	 */
	public int resetIsAlreadyInIslandAndCountManifolds() {
		this.isAlreadyInIsland = false;
		int nbManifolds = 0;
		// Reset the this.isAlreadyInIsland variable of the contact manifolds for this body
		ContactManifoldListElement currentElement = this.contactManifoldsList;
		while (currentElement != null) {
			currentElement.contactManifold.isAlreadyInIsland = false;
			currentElement = currentElement.next;
			nbManifolds++;
		}
		return nbManifolds;
	}
	
	/**
	 * @brief Set whether or not the body is active
	 * @param[in] _isActive True if you want to activate the body
	 */
	@Override
	public void setIsActive(final boolean _isActive) {
		// If the state does not change
		if (this.isActive == _isActive) {
			return;
		}
		super.setIsActive(_isActive);
		// If we have to activate the body
		if (_isActive == true) {
			for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.next) {
				final AABB aabb = new AABB();
				shape.getCollisionShape().computeAABB(aabb, this.transform.multiplyNew(shape.localToBodyTransform));
				this.world.collisionDetection.addProxyCollisionShape(shape, aabb);
			}
		} else {
			for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.next) {
				this.world.collisionDetection.removeProxyCollisionShape(shape);
			}
			resetContactManifoldsList();
		}
	}
	
	/**
	 * @brief Set the current position and orientation
	 * @param transform The transformation of the body that transforms the local-space of the body int32_to world-space
	 */
	public void setTransform(final Transform3D _transform) {
		//Log.info("         set transform: " + this.transform + " ==> " + _transform);
		if (Float.isNaN(_transform.getPosition().x)) {
			Log.critical("         set transform: " + this.transform + " ==> " + _transform);
		}
		if (Float.isInfinite(_transform.getOrientation().z)) {
			Log.critical("         set transform: " + this.transform + " ==> " + _transform);
		}
		this.transform = _transform;
		updateBroadPhaseState();
	}
	
	/**
	 * @brief Set the type of the body
	 * @param[in] type The type of the body (STATIC, KINEMATIC, DYNAMIC)
	 */
	public void setType(final BodyType _type) {
		this.type = _type;
		if (this.type == BodyType.STATIC) {
			// Update the broad-phase state of the body
			updateBroadPhaseState();
		}
	}
	
	/**
	 * @brief Return true if a point is inside the collision body
	 * This method returns true if a point is inside any collision shape of the body
	 * @param[in] _worldPoint The point to test (in world-space coordinates)
	 * @return True if the point is inside the body
	 */
	public boolean testPointInside(final Vector3f _worldPoint) {
		for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.next) {
			if (shape.testPointInside(_worldPoint)) {
				return true;
			}
		}
		return false;
	}
	
	/**
	 * @brief Update the broad-phase state for this body (because it has moved for instance)
	 */
	protected void updateBroadPhaseState() {
		// For all the proxy collision shapes of the body
		for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.getNext()) {
			// Update the proxy
			updateProxyShapeInBroadPhase(shape, false);
		}
	}
	
	/**
	 * @brief Update the broad-phase state of a proxy collision shape of the body
	 */
	public void updateProxyShapeInBroadPhase(final ProxyShape _proxyShape, final boolean _forceReinsert /* false */) {
		final AABB aabb = new AABB();
		_proxyShape.getCollisionShape().computeAABB(aabb, this.transform.multiplyNew(_proxyShape.getLocalToBodyTransform()));
		this.world.getCollisionDetection().updateProxyCollisionShape(_proxyShape, aabb, new Vector3f(0, 0, 0), _forceReinsert);
	}
}
