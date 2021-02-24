package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.RigidBody;

/**
 * This structure is used to gather the information needed to create a slider
 * joint. This structure will be used to create the actual slider joint.
 */
public class SliderJointInfo extends JointInfo {
	
	public Vector3f anchorPointWorldSpace; //!< Anchor point (in world-space coordinates)
	public Vector3f sliderAxisWorldSpace; //!< Slider axis (in world-space coordinates)
	public boolean isLimitEnabled; //!< True if the slider limits are enabled
	public boolean isMotorEnabled; //!< True if the slider motor is enabled
	public float minTranslationLimit; //!< Mininum allowed translation if limits are enabled
	public float maxTranslationLimit; //!< Maximum allowed translation if limits are enabled
	public float motorSpeed; //!< Motor speed
	public float maxMotorForce; //!< Maximum motor force (in Newtons) that can be applied to reach to desired motor speed
	
	/** 
		 *  Constructor without limits and without motor
		 * @param _rigidBody1 The first body of the joint
		 * @param _rigidBody2 The second body of the joint
		 * @param _initAnchorPointWorldSpace The initial anchor point in world-space
		 * @param _initSliderAxisWorldSpace The initial slider axis in world-space
		 */
	public SliderJointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final Vector3f _initAnchorPointWorldSpace, final Vector3f _initSliderAxisWorldSpace) {
		super(_rigidBody1, _rigidBody2, JointType.SLIDERJOINT);
		this.anchorPointWorldSpace = _initAnchorPointWorldSpace;
		this.sliderAxisWorldSpace = _initSliderAxisWorldSpace;
		this.isLimitEnabled = false;
		this.isMotorEnabled = false;
		this.minTranslationLimit = -1.0f;
		this.maxTranslationLimit = 1.0f;
		this.motorSpeed = 0;
		this.maxMotorForce = 0;
		
	}
	
	/**
		 *  Constructor with limits and no motor
		 * @param _rigidBody1 The first body of the joint
		 * @param _rigidBody2 The second body of the joint
		 * @param _initAnchorPointWorldSpace The initial anchor point in world-space
		 * @param _initSliderAxisWorldSpace The initial slider axis in world-space
		 * @param _initMinTranslationLimit The initial minimum translation limit (in meters)
		 * @param _initMaxTranslationLimit The initial maximum translation limit (in meters)
		 */
	public SliderJointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final Vector3f _initAnchorPointWorldSpace, final Vector3f _initSliderAxisWorldSpace,
			final float _initMinTranslationLimit, final float _initMaxTranslationLimit) {
		super(_rigidBody1, _rigidBody2, JointType.SLIDERJOINT);
		this.anchorPointWorldSpace = _initAnchorPointWorldSpace;
		this.sliderAxisWorldSpace = _initSliderAxisWorldSpace;
		this.isLimitEnabled = true;
		this.isMotorEnabled = false;
		this.minTranslationLimit = _initMinTranslationLimit;
		this.maxTranslationLimit = _initMaxTranslationLimit;
		this.motorSpeed = 0;
		this.maxMotorForce = 0;
		
	}
	
	/**
		 *  Constructor with limits and motor
		 * @param _rigidBody1 The first body of the joint
		 * @param _rigidBody2 The second body of the joint
		 * @param _initAnchorPointWorldSpace The initial anchor point in world-space
		 * @param _initSliderAxisWorldSpace The initial slider axis in world-space
		 * @param _initMinTranslationLimit The initial minimum translation limit (in meters)
		 * @param _initMaxTranslationLimit The initial maximum translation limit (in meters)
		 * @param _initMotorSpeed The initial speed of the joint motor (in meters per second)
		 * @param _initMaxMotorForce The initial maximum motor force of the joint (in Newtons x meters)
		 */
	public SliderJointInfo(final RigidBody _rigidBody1, final RigidBody _rigidBody2, final Vector3f _initAnchorPointWorldSpace, final Vector3f _initSliderAxisWorldSpace,
			final float _initMinTranslationLimit, final float _initMaxTranslationLimit, final float _initMotorSpeed, final float _initMaxMotorForce) {
		super(_rigidBody1, _rigidBody2, JointType.SLIDERJOINT);
		this.anchorPointWorldSpace = _initAnchorPointWorldSpace;
		this.sliderAxisWorldSpace = _initSliderAxisWorldSpace;
		this.isLimitEnabled = true;
		this.isMotorEnabled = true;
		this.minTranslationLimit = _initMinTranslationLimit;
		this.maxTranslationLimit = _initMaxTranslationLimit;
		this.motorSpeed = _initMotorSpeed;
		this.maxMotorForce = _initMaxMotorForce;
		
	}
}
