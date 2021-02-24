package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.RigidBody;

/**
 * This structure is used to gather the information needed to create a fixed
 * joint. This structure will be used to create the actual fixed joint.
 */
public class FixedJointInfo extends JointInfo {
	
	Vector3f anchorPointWorldSpace; //!< Anchor point (in world-space coordinates)
	
	/**
	 * Constructor
	 * @param _rigidBody1 The first body of the joint
	 * @param _rigidBody2 The second body of the joint
	 * @param _initAnchorPointWorldSpace The initial anchor point of the joint in world-space coordinates
	 */
	FixedJointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final Vector3f _initAnchorPointWorldSpace) {
		super(_rigidBody1, _rigidBody2, JointType.FIXEDJOINT);
		this.anchorPointWorldSpace = _initAnchorPointWorldSpace;
	}
}
