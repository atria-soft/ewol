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

import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.collision.shapes.AABB;
import org.atriasoft.ephysics.collision.shapes.CollisionShape;
import org.atriasoft.ephysics.constraint.Joint;
import org.atriasoft.ephysics.constraint.JointListElement;
import org.atriasoft.ephysics.engine.CollisionWorld;
import org.atriasoft.ephysics.engine.DynamicsWorld;
import org.atriasoft.ephysics.engine.Material;
import org.atriasoft.ephysics.internal.Log;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

/**
 *  This class represents a rigid body of the physics
 * engine. A rigid body is a non-deformable body that
 * has a ant mass. This class inherits from the
 * CollisionBody class.
 */
public class RigidBody extends CollisionBody {
	
	protected float initMass = 1.0f; //!< Intial mass of the body
	protected Vector3f centerOfMassLocal = new Vector3f(0, 0, 0); //!< Center of mass of the body in local-space coordinates. The center of mass can therefore be different from the body origin
	public Vector3f centerOfMassWorld = new Vector3f(); //!< Center of mass of the body in world-space coordinates
	public Vector3f linearVelocity = new Vector3f(); //!< Linear velocity of the body
	public Vector3f angularVelocity = new Vector3f(); //!< Angular velocity of the body
	public Vector3f externalForce = new Vector3f(); //!< Current external force on the body
	public Vector3f externalTorque = new Vector3f(); //!< Current external torque on the body
	public Matrix3f inertiaTensorLocal = new Matrix3f(); //!< Local inertia tensor of the body (in local-space) with respect to the center of mass of the body
	public Matrix3f inertiaTensorLocalInverse = new Matrix3f(); //!< Inverse of the inertia tensor of the body
	public float massInverse; //!< Inverse of the mass of the body
	protected boolean isGravityEnabled = true; //!< True if the gravity needs to be applied to this rigid body
	protected Material material = new Material(); //!< Material properties of the rigid body
	protected float linearDamping = 0.0f; //!< Linear velocity damping factor
	protected float angularDamping = 0.0f; //!< Angular velocity damping factor
	public JointListElement jointsList = null; //!< First element of the linked list of joints involving this body
	protected boolean angularReactionEnable = true;
	
	/**
	 *  Constructor
	 * @param transform The transformation of the body
	 * @param world The world where the body has been added
	 * @param id The ID of the body
	 */
	public RigidBody(final Transform3D transform, final CollisionWorld world, final int id) {
		super(transform, world, id);
		this.centerOfMassWorld = new Vector3f(transform.getPosition());
		this.massInverse = 1.0f / this.initMass;
	}
	
	/**
	 *  Add a collision shape to the body.
	 * When you add a collision shape to the body, an intternal copy of this collision shape will be created internally.
	 * Therefore, you can delete it right after calling this method or use it later to add it to another body.
	 * This method will return a pointer to a new proxy shape. A proxy shape is an object that links a collision shape and a given body.
	 * You can use the returned proxy shape to get and set information about the corresponding collision shape for that body.
	 * @param _collisionShape The collision shape you want to add to the body
	 * @param _transform The transformation of the collision shape that transforms the local-space of the collision shape into the local-space of the body
	 * @param _mass Mass (in kilograms) of the collision shape you want to add
	 * @return A pointer to the proxy shape that has been created to link the body to the new collision shape you have added.
	 */
	public ProxyShape addCollisionShape(final CollisionShape _collisionShape, final Transform3D _transform, final float _mass) {
		assert (_mass > 0.0f);
		// Create a new proxy collision shape to attach the collision shape to the body
		final ProxyShape proxyShape = new ProxyShape(this, _collisionShape, _transform, _mass);
		// Add it to the list of proxy collision shapes of the body
		if (this.proxyCollisionShapes == null) {
			this.proxyCollisionShapes = proxyShape;
		} else {
			proxyShape.next = this.proxyCollisionShapes;
			this.proxyCollisionShapes = proxyShape;
		}
		// Compute the world-space AABB of the new collision shape
		final AABB aabb = new AABB();
		_collisionShape.computeAABB(aabb, this.transform.multiplyNew(_transform));
		// Notify the collision detection about this new collision shape
		this.world.collisionDetection.addProxyCollisionShape(proxyShape, aabb);
		this.numberCollisionShapes++;
		recomputeMassInformation();
		return proxyShape;
	}
	
	/**
	 *  Apply an external force to the body at a given point (in world-space coordinates).
	 * If the point is not at the center of mass of the body, it will also
	 * generate some torque and therefore, change the angular velocity of the body.
	 * If the body is sleeping, calling this method will wake it up. Note that the
	 * force will we added to the sum of the applied forces and that this sum will be
	 * reset to zero at the end of each call of the DynamicsWorld::update() method.
	 * You can only apply a force to a dynamic body otherwise, this method will do nothing.
	 * @param _force The force to apply on the body
	 * @param _point The point where the force is applied (in world-space coordinates)
	 */
	public void applyForce(final Vector3f _force, final Vector3f _point) {
		if (this.type != BodyType.DYNAMIC) {
			return;
		}
		if (this.isSleeping) {
			setIsSleeping(false);
		}
		this.externalForce.add(_force);
		this.externalTorque.add(_point.lessNew(this.centerOfMassWorld).cross(_force));
	}
	
	/**
	 *  Apply an external force to the body at its center of mass.
	 * If the body is sleeping, calling this method will wake it up. Note that the
	 * force will we added to the sum of the applied forces and that this sum will be
	 * reset to zero at the end of each call of the DynamicsWorld::update() method.
	 * You can only apply a force to a dynamic body otherwise, this method will do nothing.
	 * @param _force The external force to apply on the center of mass of the body
	 */
	public void applyForceToCenterOfMass(final Vector3f _force) {
		if (this.type != BodyType.DYNAMIC) {
			return;
		}
		if (this.isSleeping) {
			setIsSleeping(false);
		}
		this.externalForce.add(_force);
	}
	
	/**
	 *  Apply an external torque to the body.
	 * If the body is sleeping, calling this method will wake it up. Note that the
	 * force will we added to the sum of the applied torques and that this sum will be
	 * reset to zero at the end of each call of the DynamicsWorld::update() method.
	 * You can only apply a force to a dynamic body otherwise, this method will do nothing.
	 * @param _torque The external torque to apply on the body
	 */
	public void applyTorque(final Vector3f _torque) {
		if (this.type != BodyType.DYNAMIC) {
			return;
		}
		if (this.isSleeping) {
			setIsSleeping(false);
		}
		this.externalTorque.add(_torque);
	}
	
	/**
	 *  Set the variable to know if the gravity is applied to this rigid body
	 * @param _isEnabled True if you want the gravity to be applied to this body
	 */
	public void enableGravity(final boolean _isEnabled) {
		this.isGravityEnabled = _isEnabled;
	}
	
	/**
	 *  Get the angular velocity damping factor
	 * @return The angular damping factor of this body
	 */
	public float getAngularDamping() {
		return this.angularDamping;
	}
	
	/**
		 *  Get the angular velocity of the body
		 * @return The angular velocity vector of the body
		 */
	public Vector3f getAngularVelocity() {
		return this.angularVelocity;
	}
	
	/**
	 *  Get the inverse of the inertia tensor in world coordinates.
	 * The inertia tensor I_w in world coordinates is computed with the
	 * local inverse inertia tensor I_b^-1 in body coordinates
	 * by I_w = R * I_b^-1 * R^T
	 * where R is the rotation matrix (and R^T its transpose) of the
	 * current orientation quaternion of the body
	 * @return The 3x3 inverse inertia tensor matrix of the body in world-space coordinates
	 */
	public Matrix3f getInertiaTensorInverseWorld() {
		// TODO : DO NOT RECOMPUTE THE MATRIX MULTIPLICATION EVERY TIME. WE NEED TO STORE THE
		//		INVERSE WORLD TENSOR IN THE CLASS AND UPLDATE IT WHEN THE ORIENTATION OF THE BODY CHANGES
		// Compute and return the inertia tensor in world coordinates
		//Log.error("}}}    this.transform=" + this.transform);
		//Log.error("}}}    this.inertiaTensorLocalInverse=" + this.inertiaTensorLocalInverse);
		//Log.error("}}}    this.transform.getOrientation().getMatrix().transposeNew()=" + this.transform.getOrientation().getMatrix().transposeNew());
		//Log.error("}}}    this.transform.getOrientation().getMatrix()=" + this.transform.getOrientation().getMatrix());
		final Matrix3f tmp = this.transform.getOrientation().getMatrix().multiplyNew(this.inertiaTensorLocalInverse).multiply(this.transform.getOrientation().getMatrix().transposeNew());
		//Log.error("}}}    tmp=" + tmp);
		return tmp;
	}
	
	/**
	 *  Get the local inertia tensor of the body (in local-space coordinates)
	 * @return The 3x3 inertia tensor matrix of the body
	 */
	public Matrix3f getInertiaTensorLocal() {
		return this.inertiaTensorLocal;
	}
	
	/**
	 *  Get the inertia tensor in world coordinates.
	 * The inertia tensor I_w in world coordinates is computed
	 * with the local inertia tensor I_b in body coordinates
	 * by I_w = R * I_b * R^T
	 * where R is the rotation matrix (and R^T its transpose) of
	 * the current orientation quaternion of the body
	 * @return The 3x3 inertia tensor matrix of the body in world-space coordinates
	 */
	public Matrix3f getInertiaTensorWorld() {
		// Compute and return the inertia tensor in world coordinates
		return this.transform.getOrientation().getMatrix().multiplyNew(this.inertiaTensorLocal).multiply(this.transform.getOrientation().getMatrix().transposeNew());
	}
	
	/**
	 *  Get the first element of the linked list of joints involving this body
	 * @return The first element of the linked-list of all the joints involving this body
	 */
	public JointListElement getJointsList() {
		return this.jointsList;
	}
	
	/**
	 *  Get the linear velocity damping factor
	 * @return The linear damping factor of this body
	 */
	public float getLinearDamping() {
		return this.linearDamping;
	}
	
	/**
	 *  Get the linear velocity
	 * @return The linear velocity vector of the body
	 */
	public Vector3f getLinearVelocity() {
		return this.linearVelocity;
	}
	
	/**
	 *  Get the mass of the body
	 * @return The mass (in kilograms) of the body
	 */
	public float getMass() {
		return this.initMass;
	}
	
	/**
	 *  get a reference to the material properties of the rigid body
	 * @return A reference to the material of the body
	 */
	public Material getMaterial() {
		return this.material;
	}
	
	public boolean isAngularReactionEnable() {
		return this.angularReactionEnable;
	}
	
	/**
	 *  get the need of gravity appling to this rigid body
	 * @return True if the gravity is applied to the body
	 */
	public boolean isGravityEnabled() {
		return this.isGravityEnabled;
	}
	
	/**
	 *  Recompute the center of mass, total mass and inertia tensor of the body using all the collision shapes attached to the body.
	 */
	public void recomputeMassInformation() {
		this.initMass = 0.0f;
		this.massInverse = 0.0f;
		this.inertiaTensorLocal.setZero();
		this.inertiaTensorLocalInverse.setZero();
		this.centerOfMassLocal.setZero();
		// If it is STATIC or KINEMATIC body
		if (this.type == BodyType.STATIC || this.type == BodyType.KINEMATIC) {
			this.centerOfMassWorld = this.transform.getPosition();
			return;
		}
		assert (this.type == BodyType.DYNAMIC);
		// Compute the total mass of the body
		for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.next) {
			this.initMass += shape.getMass();
			this.centerOfMassLocal.add(shape.getLocalToBodyTransform().getPosition().multiplyNew(shape.getMass()));
		}
		if (this.initMass > 0.0f) {
			this.massInverse = 1.0f / this.initMass;
		} else {
			this.initMass = 1.0f;
			this.massInverse = 1.0f;
		}
		// Compute the center of mass
		final Vector3f oldCenterOfMass = this.centerOfMassWorld;
		this.centerOfMassLocal.multiply(this.massInverse);
		this.centerOfMassWorld = this.transform.multiplyNew(this.centerOfMassLocal);
		// Compute the total mass and inertia tensor using all the collision shapes
		for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.next) {
			// Get the inertia tensor of the collision shape in its local-space
			Matrix3f inertiaTensor = new Matrix3f();
			shape.getCollisionShape().computeLocalInertiaTensor(inertiaTensor, shape.getMass());
			// Convert the collision shape inertia tensor into the local-space of the body
			final Transform3D shapeTransform = shape.getLocalToBodyTransform();
			final Matrix3f rotationMatrix = shapeTransform.getOrientation().getMatrix();
			inertiaTensor = rotationMatrix.multiplyNew(inertiaTensor).multiply(rotationMatrix.transposeNew());
			// Use the parallel axis theorem to convert the inertia tensor w.r.t the collision shape
			// center into a inertia tensor w.r.t to the body origin.
			final Vector3f offset = shapeTransform.getPosition().lessNew(this.centerOfMassLocal);
			final float offsetSquare = offset.length2();
			final Vector3f off1 = offset.multiplyNew(-offset.x);
			final Vector3f off2 = offset.multiplyNew(-offset.y);
			final Vector3f off3 = offset.multiplyNew(-offset.z);
			final Matrix3f offsetMatrix = new Matrix3f(off1.x + offsetSquare, off1.y, off1.z, off2.x, off2.y + offsetSquare, off2.z, off3.x, off3.y, off3.z + offsetSquare);
			offsetMatrix.multiply(shape.getMass());
			this.inertiaTensorLocal.add(inertiaTensor.addNew(offsetMatrix));
		}
		// Compute the local inverse inertia tensor
		this.inertiaTensorLocalInverse = this.inertiaTensorLocal.inverseNew();
		// Update the linear velocity of the center of mass
		this.linearVelocity.add(this.angularVelocity.cross(this.centerOfMassWorld.lessNew(oldCenterOfMass)));
	}
	
	@Override
	public void removeCollisionShape(final ProxyShape _proxyShape) {
		super.removeCollisionShape(_proxyShape);
		recomputeMassInformation();
	}
	
	/**
	 *  Remove a joint from the joints list
	 */
	public void removeJointFromjointsList(final Joint joint) {
		assert (joint != null);
		assert (this.jointsList != null);
		// Remove the joint from the linked list of the joints of the first body
		if (this.jointsList.joint == joint) { // If the first element is the one to remove
			JointListElement elementToRemove = this.jointsList;
			this.jointsList = elementToRemove.next;
			elementToRemove = null;
		} else { // If the element to remove is not the first one in the list
			JointListElement currentElement = this.jointsList;
			while (currentElement.next != null) {
				if (currentElement.next.joint == joint) {
					JointListElement elementToRemove = currentElement.next;
					currentElement.next = elementToRemove.next;
					elementToRemove = null;
					break;
				}
				currentElement = currentElement.next;
			}
		}
	}
	
	/**
	 *  Set the angular damping factor. This is the ratio of the angular velocity that the body will lose at every seconds of simulation.
	 * @param _angularDamping The angular damping factor of this body
	 */
	public void setAngularDamping(final float _angularDamping) {
		assert (_angularDamping >= 0.0f);
		this.angularDamping = _angularDamping;
	}
	
	public void setAngularReactionEnable(final boolean angularReactionEnable) {
		this.angularReactionEnable = angularReactionEnable;
	}
	
	/**
	*  Set the angular velocity.
	* @param _angularVelocity The angular velocity vector of the body
	*/
	public void setAngularVelocity(final Vector3f _angularVelocity) {
		if (this.type == BodyType.STATIC) {
			return;
		}
		this.angularVelocity = _angularVelocity;
		if (this.angularVelocity.length2() > 0.0f) {
			setIsSleeping(false);
		}
	}
	
	/**
	 *  Set the local center of mass of the body (in local-space coordinates)
	 * @param _centerOfMassLocal The center of mass of the body in local-space coordinates
	 */
	public void setCenterOfMassLocal(final Vector3f centerOfMassLocal) {
		if (this.type != BodyType.DYNAMIC) {
			return;
		}
		final Vector3f oldCenterOfMass = this.centerOfMassWorld;
		this.centerOfMassLocal = centerOfMassLocal;
		this.centerOfMassWorld = this.transform.multiplyNew(this.centerOfMassLocal);
		this.linearVelocity.add(this.angularVelocity.cross(this.centerOfMassWorld.lessNew(oldCenterOfMass)));
	}
	
	/**
	 *  Set the local inertia tensor of the body (in local-space coordinates)
	 * @param _inertiaTensorLocal The 3x3 inertia tensor matrix of the body in local-space coordinates
	 */
	public void setInertiaTensorLocal(final Matrix3f inertiaTensorLocal) {
		if (this.type != BodyType.DYNAMIC) {
			return;
		}
		this.inertiaTensorLocal = inertiaTensorLocal;
		this.inertiaTensorLocalInverse = this.inertiaTensorLocal.inverseNew();
	}
	
	/**
	 *  Set the variable to know whether or not the body is sleeping
	 * @param _isSleeping New sleeping state of the body
	 */
	@Override
	public void setIsSleeping(final boolean _isSleeping) {
		if (_isSleeping) {
			this.linearVelocity.setZero();
			this.angularVelocity.setZero();
			this.externalForce.setZero();
			this.externalTorque.setZero();
		}
		super.setIsSleeping(_isSleeping);
	}
	
	/**
	 *  Set the linear damping factor. This is the ratio of the linear velocity that the body will lose every at seconds of simulation.
	 * @param _linearDamping The linear damping factor of this body
	 */
	public void setLinearDamping(final float _linearDamping) {
		assert (_linearDamping >= 0.0f);
		this.linearDamping = _linearDamping;
	}
	
	/**
	 *  Set the linear velocity of the rigid body.
	 * @param _linearVelocity Linear velocity vector of the body
	 */
	public void setLinearVelocity(final Vector3f _linearVelocity) {
		if (this.type == BodyType.STATIC) {
			return;
		}
		this.linearVelocity = _linearVelocity;
		if (this.linearVelocity.length2() > 0.0f) {
			setIsSleeping(false);
		}
	}
	
	/**
	 *  Set the mass of the rigid body
	 * @param _mass The mass (in kilograms) of the body
	 */
	public void setMass(final float _mass) {
		if (this.type != BodyType.DYNAMIC) {
			return;
		}
		this.initMass = _mass;
		if (this.initMass > 0.0f) {
			this.massInverse = 1.0f / this.initMass;
		} else {
			this.initMass = 1.0f;
			this.massInverse = 1.0f;
		}
	}
	
	/**
	 *  Set a new material for this rigid body
	 * @param _material The material you want to set to the body
	 */
	public void setMaterial(final Material _material) {
		this.material = _material;
	}
	
	/**
	 *  Set the current position and orientation
	 * @param _transform The transformation of the body that transforms the local-space of the body into world-space
	 */
	@Override
	public void setTransform(final Transform3D _transform) {
		//Log.info("         set transform: " + this.transform + " ==> " + _transform);
		if (_transform.getPosition().x == Float.NaN) {
			Log.critical("         set transform: " + this.transform + " ==> " + _transform);
		}
		if (Float.isInfinite(_transform.getOrientation().z)) {
			Log.critical("         set transform: " + this.transform + " ==> " + _transform);
		}
		this.transform = _transform;
		final Vector3f oldCenterOfMass = this.centerOfMassWorld;
		// Compute the new center of mass in world-space coordinates
		this.centerOfMassWorld = this.transform.multiplyNew(this.centerOfMassLocal);
		// Update the linear velocity of the center of mass
		this.linearVelocity.add(this.angularVelocity.cross(this.centerOfMassWorld.lessNew(oldCenterOfMass)));
		updateBroadPhaseState();
	}
	
	@Override
	public void setType(final BodyType _type) {
		if (this.type == _type) {
			return;
		}
		super.setType(_type);
		recomputeMassInformation();
		if (this.type == BodyType.STATIC) {
			// Reset the velocity to zero
			this.linearVelocity.setZero();
			this.angularVelocity.setZero();
		}
		if (this.type == BodyType.STATIC || this.type == BodyType.KINEMATIC) {
			// Reset the inverse mass and inverse inertia tensor to zero
			this.massInverse = 0.0f;
			this.inertiaTensorLocal.setZero();
			this.inertiaTensorLocalInverse.setZero();
		} else {
			this.massInverse = 1.0f / this.initMass;
			this.inertiaTensorLocalInverse = this.inertiaTensorLocal.inverseNew();
		}
		setIsSleeping(false);
		resetContactManifoldsList();
		// Ask the broad-phase to test again the collision shapes of the body for collision detection (as if the body has moved)
		askForBroadPhaseCollisionCheck();
		this.externalForce.setZero();
		this.externalTorque.setZero();
	}
	
	@Override
	public void updateBroadPhaseState() {
		final DynamicsWorld world = (DynamicsWorld) this.world;
		final Vector3f displacement = this.linearVelocity.multiplyNew(world.timeStep);
		
		// For all the proxy collision shapes of the body
		for (ProxyShape shape = this.proxyCollisionShapes; shape != null; shape = shape.next) {
			// Recompute the world-space AABB of the collision shape
			final AABB aabb = new AABB();
			//Log.info("         : " + aabb.getMin() + " " + aabb.getMax());
			//Log.info("         this.transform: " + this.transform);
			shape.getCollisionShape().computeAABB(aabb, this.transform.multiplyNew(shape.getLocalToBodyTransform()));
			//Log.info("         : " + aabb.getMin() + " " + aabb.getMax());
			// Update the broad-phase state for the proxy collision shape
			//Log.warning("            ==> updateProxyCollisionShape");
			world.collisionDetection.updateProxyCollisionShape(shape, aabb, displacement);
		}
	}
	
	/**
	 *  Update the transform of the body after a change of the center of mass
	 */
	public void updateTransformWithCenterOfMass() {
		// Translate the body according to the translation of the center of mass position
		this.transform.setPosition(this.centerOfMassWorld.lessNew(this.transform.getOrientation().multiply(this.centerOfMassLocal)));
		if (Float.isNaN(this.transform.getPosition().x) == true) {
			Log.critical("updateTransformWithCenterOfMass: " + this.transform);
		}
		if (Float.isInfinite(this.transform.getOrientation().z) == true) {
			Log.critical("         set transform: " + this.transform);
		}
	}
	
}
