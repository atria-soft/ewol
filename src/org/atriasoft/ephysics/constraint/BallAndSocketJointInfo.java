package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.RigidBody;

/**
 *  It is used to gather the information needed to create a ball-and-socket
 * joint. This structure will be used to create the actual ball-and-socket joint.
 */
public class BallAndSocketJointInfo extends JointInfo {
	
	public Vector3f anchorPointWorldSpace; //!< Anchor point (in world-space coordinates)
	
	/**
	 * Constructor
	 * @param _rigidBody1 Pointer to the first body of the joint
	 * @param _rigidBody2 Pointer to the second body of the joint
	 * @param _initAnchorPointWorldSpace The anchor point in world-space coordinates
	 */
	public BallAndSocketJointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final Vector3f _initAnchorPointWorldSpace) {
		super(_rigidBody1, _rigidBody2, JointType.BALLSOCKETJOINT);
		this.anchorPointWorldSpace = _initAnchorPointWorldSpace;
		
	}
}
