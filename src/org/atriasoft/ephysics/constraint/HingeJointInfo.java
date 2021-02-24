package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.RigidBody;

/**
 *  It is used to gather the information needed to create a hinge joint.
 * This structure will be used to create the actual hinge joint.
 */
public class HingeJointInfo extends JointInfo {
	
	public Vector3f anchorPointWorldSpace; //!< Anchor point (in world-space coordinates)
	public Vector3f rotationAxisWorld; //!< Hinge rotation axis (in world-space coordinates)
	public boolean isLimitEnabled; //!< True if the hinge joint limits are enabled
	public boolean isMotorEnabled; //!< True if the hinge joint motor is enabled
	public float minAngleLimit; //!< Minimum allowed rotation angle (in radian) if limits are enabled. The angle must be in the range [-2*pi, 0]
	public float maxAngleLimit; //!< Maximum allowed rotation angle (in radian) if limits are enabled. The angle must be in the range [0, 2*pi]
	public float motorSpeed; //!< Motor speed (in radian/second)
	public float maxMotorTorque; //!< Maximum motor torque (in Newtons * meters) that can be applied to reach to desired motor speed
	
	/**
	 *  Constructor without limits and without motor
	 * @param _rigidBody1 The first body of the joint
	 * @param _rigidBody2 The second body of the joint
	 * @param _initAnchorPointWorldSpace The initial anchor point in world-space coordinates
	 * @param _initRotationAxisWorld The initial rotation axis in world-space coordinates
	 */
	public HingeJointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final Vector3f _initAnchorPointWorldSpace, final Vector3f _initRotationAxisWorld) {
		super(_rigidBody1, _rigidBody2, JointType.HINGEJOINT);
		this.anchorPointWorldSpace = _initAnchorPointWorldSpace;
		this.rotationAxisWorld = _initRotationAxisWorld;
		this.isLimitEnabled = false;
		this.isMotorEnabled = false;
		this.minAngleLimit = -1;
		this.maxAngleLimit = 1;
		this.motorSpeed = 0;
		this.maxMotorTorque = 0;
		
	}
	
	/**
	 *  Constructor with limits but without motor
	 * @param _rigidBody1 The first body of the joint
	 * @param _rigidBody2 The second body of the joint
	 * @param _initAnchorPointWorldSpace The initial anchor point in world-space coordinates
	 * @param _initRotationAxisWorld The intial rotation axis in world-space coordinates
	 * @param _initMinAngleLimit The initial minimum limit angle (in radian)
	 * @param _initMaxAngleLimit The initial maximum limit angle (in radian)
	 */
	public HingeJointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final Vector3f _initAnchorPointWorldSpace, final Vector3f _initRotationAxisWorld, final float _initMinAngleLimit,
			final float _initMaxAngleLimit) {
		super(_rigidBody1, _rigidBody2, JointType.HINGEJOINT);
		this.anchorPointWorldSpace = _initAnchorPointWorldSpace;
		this.rotationAxisWorld = _initRotationAxisWorld;
		this.isLimitEnabled = true;
		this.isMotorEnabled = false;
		this.minAngleLimit = _initMinAngleLimit;
		this.maxAngleLimit = _initMaxAngleLimit;
		this.motorSpeed = 0;
		this.maxMotorTorque = 0;
		
	}
	
	/**
	 *  Constructor with limits and motor
	 * @param _rigidBody1 The first body of the joint
	 * @param _rigidBody2 The second body of the joint
	 * @param _initAnchorPointWorldSpace The initial anchor point in world-space
	 * @param _initRotationAxisWorld The initial rotation axis in world-space
	 * @param _initMinAngleLimit The initial minimum limit angle (in radian)
	 * @param _initMaxAngleLimit The initial maximum limit angle (in radian)
	 * @param _initMotorSpeed The initial motor speed of the joint (in radian per second)
	 * @param _initMaxMotorTorque The initial maximum motor torque (in Newtons)
	 */
	public HingeJointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final Vector3f _initAnchorPointWorldSpace, final Vector3f _initRotationAxisWorld, final float _initMinAngleLimit,
			final float _initMaxAngleLimit, final float _initMotorSpeed, final float _initMaxMotorTorque) {
		super(_rigidBody1, _rigidBody2, JointType.HINGEJOINT);
		this.anchorPointWorldSpace = _initAnchorPointWorldSpace;
		this.rotationAxisWorld = _initRotationAxisWorld;
		this.isLimitEnabled = true;
		this.isMotorEnabled = false;
		this.minAngleLimit = _initMinAngleLimit;
		this.maxAngleLimit = _initMaxAngleLimit;
		this.motorSpeed = _initMotorSpeed;
		this.maxMotorTorque = _initMaxMotorTorque;
		
	}
};
