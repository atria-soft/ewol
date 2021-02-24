package org.atriasoft.ephysics.constraint;

import org.atriasoft.ephysics.body.RigidBody;

/**
 *  It is used to gather the information needed to create a joint.
 */
public class JointInfo {
	//!< First rigid body of the joint
	public final RigidBody body1;
	//!< Second rigid body of the joint
	public final RigidBody body2;
	//!< Type of the joint
	public final JointType type;
	//!< Position correction technique used for the raint (used for joints). By default, the BAUMGARTE technique is used
	public final JointsPositionCorrectionTechnique positionCorrectionTechnique;
	//!< True if the two bodies of the joint are allowed to collide with each other
	public final boolean isCollisionEnabled;
	
	/// Constructor
	JointInfo(final JointType _raintType) {
		this.body1 = null;
		this.body2 = null;
		this.type = _raintType;
		this.positionCorrectionTechnique = JointsPositionCorrectionTechnique.NON_LINEAR_GAUSS_SEIDEL;
		this.isCollisionEnabled = true;
	}
	
	/// Constructor
	JointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final JointType _raintType) {
		this.body1 = _rigidBody1;
		this.body2 = _rigidBody2;
		this.type = _raintType;
		this.positionCorrectionTechnique = JointsPositionCorrectionTechnique.NON_LINEAR_GAUSS_SEIDEL;
		this.isCollisionEnabled = true;
	}
}
