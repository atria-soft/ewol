package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Quaternion;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.BodyType;
import org.atriasoft.ephysics.engine.ConstraintSolverData;
import org.atriasoft.ephysics.mathematics.Matrix2f;

public class SliderJoint extends Joint {
	private static final float BETA = 0.2f; //!< Beta value for the position correction bias factor
	private final Vector3f localAnchorPointBody1; //!< Anchor point of body 1 (in local-space coordinates of body 1)
	private final Vector3f localAnchorPointBody2; //!< Anchor point of body 2 (in local-space coordinates of body 2)
	private final Vector3f sliderAxisBody1; //!< Slider axis (in local-space coordinates of body 1)
	private Matrix3f i1; //!< Inertia tensor of body 1 (in world-space coordinates)
	private Matrix3f i2; //!< Inertia tensor of body 2 (in world-space coordinates)
	private final Quaternion initOrientationDifferenceInv; //!< Inverse of the initial orientation difference between the two bodies
	private Vector3f N1; //!< First vector orthogonal to the slider axis local-space of body 1
	private Vector3f N2; //!< Second vector orthogonal to the slider axis and N1 in local-space of body 1
	private Vector3f R1; //!< Vector r1 in world-space coordinates
	private Vector3f R2; //!< Vector r2 in world-space coordinates
	private Vector3f R2CrossN1; //!< Cross product of r2 and n1
	private Vector3f R2CrossN2; //!< Cross product of r2 and n2
	private Vector3f R2CrossSliderAxis; //!< Cross product of r2 and the slider axis
	private Vector3f R1PlusUCrossN1; //!< Cross product of vector (r1 + u) and n1
	private Vector3f R1PlusUCrossN2; //!< Cross product of vector (r1 + u) and n2
	private Vector3f R1PlusUCrossSliderAxis; //!< Cross product of vector (r1 + u) and the slider axis
	private Vector2f bTranslation; //!< Bias of the 2 translation raints
	private Vector3f bRotation; //!< Bias of the 3 rotation raints
	private float bLowerLimit; //!< Bias of the lower limit raint
	private float bUpperLimit; //!< Bias of the upper limit raint
	private Matrix2f inverseMassMatrixTranslationConstraint; //!< Inverse of mass matrix K=JM^-1J^t for the translation raint (2x2 matrix)
	private Matrix3f inverseMassMatrixRotationConstraint; //!< Inverse of mass matrix K=JM^-1J^t for the rotation raint (3x3 matrix)
	private float inverseMassMatrixLimit; //!< Inverse of mass matrix K=JM^-1J^t for the upper and lower limit raints (1x1 matrix)
	private float inverseMassMatrixMotor; //!< Inverse of mass matrix K=JM^-1J^t for the motor
	private final Vector2f impulseTranslation; //!< Accumulated impulse for the 2 translation raints
	private final Vector3f impulseRotation; //!< Accumulated impulse for the 3 rotation raints
	private float impulseLowerLimit; //!< Accumulated impulse for the lower limit raint
	private float impulseUpperLimit; //!< Accumulated impulse for the upper limit raint
	private float impulseMotor; //!< Accumulated impulse for the motor
	private boolean isLimitEnabled; //!< True if the slider limits are enabled
	private boolean isMotorEnabled; //!< True if the motor of the joint in enabled
	private Vector3f sliderAxisWorld; //!< Slider axis in world-space coordinates
	private float lowerLimit; //!< Lower limit (minimum translation distance)
	private float upperLimit; //!< Upper limit (maximum translation distance)
	private boolean isLowerLimitViolated; //!< True if the lower limit is violated
	private boolean isUpperLimitViolated; //!< True if the upper limit is violated
	private float motorSpeed; //!< Motor speed (in m/s)
	private float maxMotorForce; //!< Maximum motor force (in Newtons) that can be applied to reach to desired motor speed
	/// Constructor
	
	public SliderJoint(final SliderJointInfo jointInfo) {
		super(jointInfo);
		this.impulseTranslation = new Vector2f(0, 0);
		this.impulseRotation = new Vector3f(0, 0, 0);
		this.impulseLowerLimit = 0;
		this.impulseUpperLimit = 0;
		this.impulseMotor = 0;
		this.isLimitEnabled = jointInfo.isLimitEnabled;
		this.isMotorEnabled = jointInfo.isMotorEnabled;
		this.lowerLimit = jointInfo.minTranslationLimit;
		this.upperLimit = jointInfo.maxTranslationLimit;
		this.isLowerLimitViolated = false;
		this.isUpperLimitViolated = false;
		this.motorSpeed = jointInfo.motorSpeed;
		this.maxMotorForce = jointInfo.maxMotorForce;
		
		assert (this.upperLimit >= 0.0f);
		assert (this.lowerLimit <= 0.0f);
		assert (this.maxMotorForce >= 0.0f);
		
		// Compute the local-space anchor point for each body
		final Transform3D transform1 = this.body1.getTransform();
		final Transform3D transform2 = this.body2.getTransform();
		this.localAnchorPointBody1 = transform1.inverseNew().multiply(jointInfo.anchorPointWorldSpace);
		this.localAnchorPointBody2 = transform2.inverseNew().multiply(jointInfo.anchorPointWorldSpace);
		
		// Compute the inverse of the initial orientation difference between the two bodies
		this.initOrientationDifferenceInv = transform2.getOrientation().multiplyNew(transform1.getOrientation().inverseNew());
		this.initOrientationDifferenceInv.normalize();
		this.initOrientationDifferenceInv.inverse();
		
		// Compute the slider axis in local-space of body 1
		this.sliderAxisBody1 = this.body1.getTransform().getOrientation().inverseNew().multiply(jointInfo.sliderAxisWorldSpace);
		this.sliderAxisBody1.normalize();
	}
	
	/// Enable/Disable the limits of the joint
	/**
	 * @param isLimitEnabled True if you want to enable the joint limits and false
	 *					   otherwise
	 */
	public void enableLimit(final boolean isLimitEnabled) {
		
		if (isLimitEnabled != this.isLimitEnabled) {
			
			this.isLimitEnabled = isLimitEnabled;
			
			// Reset the limits
			resetLimits();
		}
	}
	
	/// Enable/Disable the motor of the joint
	/**
	 * @param isMotorEnabled True if you want to enable the joint motor and false
	 *					   otherwise
	 */
	public void enableMotor(final boolean isMotorEnabled) {
		
		this.isMotorEnabled = isMotorEnabled;
		this.impulseMotor = 0.0f;
		
		// Wake up the two bodies of the joint
		this.body1.setIsSleeping(false);
		this.body2.setIsSleeping(false);
	}
	
	/// Return the maximum motor force
	/**
	 * @return The maximum force of the joint motor (in Newton x meters)
	 */
	public float getMaxMotorForce() {
		return this.maxMotorForce;
	}
	
	/// Return the maximum translation limit
	/**
	 * @return The maximum translation limit of the joint (in meters)
	 */
	public float getMaxTranslationLimit() {
		return this.upperLimit;
	}
	
	/// Return the minimum translation limit
	/**
	 * @return The minimum translation limit of the joint (in meters)
	 */
	public float getMinTranslationLimit() {
		return this.lowerLimit;
	}
	
	/// Return the intensity of the current force applied for the joint motor
	/**
	 * @param timeStep Time step (in seconds)
	 * @return The current force of the joint motor (in Newton x meters)
	 */
	public float getMotorForce(final float timeStep) {
		return this.impulseMotor / timeStep;
	}
	
	/// Return the motor speed
	/**
	 * @return The current motor speed of the joint (in meters per second)
	 */
	public float getMotorSpeed() {
		return this.motorSpeed;
	}
	
	/// Return the current translation value of the joint
	/**
	 * @return The current translation distance of the joint (in meters)
	 */
	public float getTranslation() {
		
		// TODO : Check if we need to compare rigid body position or center of mass here
		
		// Get the bodies positions and orientations
		final Vector3f x1 = this.body1.getTransform().getPosition();
		final Vector3f x2 = this.body2.getTransform().getPosition();
		final Quaternion q1 = this.body1.getTransform().getOrientation();
		final Quaternion q2 = this.body2.getTransform().getOrientation();
		
		// Compute the two anchor points in world-space coordinates
		final Vector3f anchorBody1 = x1.addNew(q1.multiply(this.localAnchorPointBody1));
		final Vector3f anchorBody2 = x2.addNew(q2.multiply(this.localAnchorPointBody2));
		
		// Compute the vector u (difference between anchor points)
		final Vector3f u = anchorBody2.lessNew(anchorBody1);
		
		// Compute the slider axis in world-space
		final Vector3f sliderAxisWorld = q1.multiply(this.sliderAxisBody1);
		sliderAxisWorld.normalize();
		
		// Compute and return the translation value
		return u.dot(sliderAxisWorld);
	}
	
	@Override
	public void initBeforeSolve(final ConstraintSolverData raintSolverData) {
		{
			
			// Initialize the bodies index in the veloc ity array
			this.indexBody1 = raintSolverData.mapBodyToConstrainedVelocityIndex.get(this.body1);
			this.indexBody2 = raintSolverData.mapBodyToConstrainedVelocityIndex.get(this.body2);
			
			// Get the bodies positions and orientations
			final Vector3f x1 = this.body1.centerOfMassWorld;
			final Vector3f x2 = this.body2.centerOfMassWorld;
			final Quaternion orientationBody1 = this.body1.getTransform().getOrientation();
			final Quaternion orientationBody2 = this.body2.getTransform().getOrientation();
			
			// Get the inertia tensor of bodies
			this.i1 = this.body1.getInertiaTensorInverseWorld();
			this.i2 = this.body2.getInertiaTensorInverseWorld();
			
			// Vector from body center to the anchor point
			this.R1 = orientationBody1.multiply(this.localAnchorPointBody1);
			this.R2 = orientationBody2.multiply(this.localAnchorPointBody2);
			
			// Compute the vector u (difference between anchor points)
			final Vector3f u = x2.addNew(this.R2).less(x1).less(this.R1);
			
			// Compute the two orthogonal vectors to the slider axis in world-space
			this.sliderAxisWorld = orientationBody1.multiply(this.sliderAxisBody1);
			this.sliderAxisWorld.normalize();
			this.N1 = this.sliderAxisWorld.getOrthoVector();
			this.N2 = this.sliderAxisWorld.cross(this.N1);
			
			// Check if the limit raints are violated or not
			final float uDotSliderAxis = u.dot(this.sliderAxisWorld);
			final float lowerLimitError = uDotSliderAxis - this.lowerLimit;
			final float upperLimitError = this.upperLimit - uDotSliderAxis;
			final boolean oldIsLowerLimitViolated = this.isLowerLimitViolated;
			this.isLowerLimitViolated = lowerLimitError <= 0;
			if (this.isLowerLimitViolated != oldIsLowerLimitViolated) {
				this.impulseLowerLimit = 0.0f;
			}
			final boolean oldIsUpperLimitViolated = this.isUpperLimitViolated;
			this.isUpperLimitViolated = upperLimitError <= 0;
			if (this.isUpperLimitViolated != oldIsUpperLimitViolated) {
				this.impulseUpperLimit = 0.0f;
			}
			
			// Compute the cross products used in the Jacobians
			this.R2CrossN1 = this.R2.cross(this.N1);
			this.R2CrossN2 = this.R2.cross(this.N2);
			this.R2CrossSliderAxis = this.R2.cross(this.sliderAxisWorld);
			final Vector3f r1PlusU = this.R1.addNew(u);
			this.R1PlusUCrossN1 = (r1PlusU).cross(this.N1);
			this.R1PlusUCrossN2 = (r1PlusU).cross(this.N2);
			this.R1PlusUCrossSliderAxis = (r1PlusU).cross(this.sliderAxisWorld);
			
			// Compute the inverse of the mass matrix K=JM^-1J^t for the 2 translation
			// raints (2x2 matrix)
			final float sumInverseMass = this.body1.massInverse + this.body2.massInverse;
			final Vector3f I1R1PlusUCrossN1 = this.i1.multiplyNew(this.R1PlusUCrossN1);
			final Vector3f I1R1PlusUCrossN2 = this.i1.multiplyNew(this.R1PlusUCrossN2);
			final Vector3f I2R2CrossN1 = this.i2.multiplyNew(this.R2CrossN1);
			final Vector3f I2R2CrossN2 = this.i2.multiplyNew(this.R2CrossN2);
			final float el11 = sumInverseMass + this.R1PlusUCrossN1.dot(I1R1PlusUCrossN1) + this.R2CrossN1.dot(I2R2CrossN1);
			final float el12 = this.R1PlusUCrossN1.dot(I1R1PlusUCrossN2) + this.R2CrossN1.dot(I2R2CrossN2);
			final float el21 = this.R1PlusUCrossN2.dot(I1R1PlusUCrossN1) + this.R2CrossN2.dot(I2R2CrossN1);
			final float el22 = sumInverseMass + this.R1PlusUCrossN2.dot(I1R1PlusUCrossN2) + this.R2CrossN2.dot(I2R2CrossN2);
			
			final Matrix2f matrixKTranslation = new Matrix2f(el11, el12, el21, el22);
			this.inverseMassMatrixTranslationConstraint.setZero();
			if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
				this.inverseMassMatrixTranslationConstraint = matrixKTranslation.inverseNew();
			}
			
			// Compute the bias "b" of the translation raint
			this.bTranslation.setZero();
			final float biasFactor = (BETA / raintSolverData.timeStep);
			if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
				this.bTranslation.setX(u.dot(this.N1));
				this.bTranslation.setY(u.dot(this.N2));
				this.bTranslation.multiply(biasFactor);
			}
			
			// Compute the inverse of the mass matrix K=JM^-1J^t for the 3 rotation
			// contraints (3x3 matrix)
			this.inverseMassMatrixRotationConstraint = this.i1.addNew(this.i2);
			if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
				this.inverseMassMatrixRotationConstraint = this.inverseMassMatrixRotationConstraint.inverseNew();
			}
			
			// Compute the bias "b" of the rotation raint
			this.bRotation.setZero();
			if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
				final Quaternion currentOrientationDifference = orientationBody2.multiplyNew(orientationBody1.inverseNew());
				currentOrientationDifference.normalize();
				final Quaternion qError = currentOrientationDifference.multiplyNew(this.initOrientationDifferenceInv);
				this.bRotation = qError.getVectorV().multiplyNew(biasFactor * 2.0f);
			}
			// If the limits are enabled
			if (this.isLimitEnabled && (this.isLowerLimitViolated || this.isUpperLimitViolated)) {
				// Compute the inverse of the mass matrix K=JM^-1J^t for the limits (1x1 matrix)
				this.inverseMassMatrixLimit = this.body1.massInverse + this.body2.massInverse + this.R1PlusUCrossSliderAxis.dot(this.i1.multiplyNew(this.R1PlusUCrossSliderAxis))
						+ this.R2CrossSliderAxis.dot(this.i2.multiplyNew(this.R2CrossSliderAxis));
				this.inverseMassMatrixLimit = (this.inverseMassMatrixLimit > 0.0f) ? 1.0f / this.inverseMassMatrixLimit : 0.0f;
				// Compute the bias "b" of the lower limit raint
				this.bLowerLimit = 0.0f;
				if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
					this.bLowerLimit = biasFactor * lowerLimitError;
				}
				// Compute the bias "b" of the upper limit raint
				this.bUpperLimit = 0.0f;
				if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
					this.bUpperLimit = biasFactor * upperLimitError;
				}
			}
			
			// If the motor is enabled
			if (this.isMotorEnabled) {
				// Compute the inverse of mass matrix K=JM^-1J^t for the motor (1x1 matrix)
				this.inverseMassMatrixMotor = this.body1.massInverse + this.body2.massInverse;
				this.inverseMassMatrixMotor = (this.inverseMassMatrixMotor > 0.0f) ? 1.0f / this.inverseMassMatrixMotor : 0.0f;
			}
			
			// If warm-starting is not enabled
			if (!raintSolverData.isWarmStartingActive) {
				// Reset all the accumulated impulses
				this.impulseTranslation.setZero();
				this.impulseRotation.setZero();
				this.impulseLowerLimit = 0.0f;
				this.impulseUpperLimit = 0.0f;
				this.impulseMotor = 0.0f;
			}
		}
	}
	
	/// Return true if the limits or the joint are enabled
	/**
	 * @return True if the joint limits are enabled
	 */
	public boolean isLimitEnabled() {
		return this.isLimitEnabled;
	}
	
	/// Return true if the motor of the joint is enabled
	/**
	 * @return True if the joint motor is enabled
	 */
	public boolean isMotorEnabled() {
		return this.isMotorEnabled;
	}
	
	/// Reset the limits
	private void resetLimits() {
		
		// Reset the accumulated impulses for the limits
		this.impulseLowerLimit = 0.0f;
		this.impulseUpperLimit = 0.0f;
		
		// Wake up the two bodies of the joint
		this.body1.setIsSleeping(false);
		this.body2.setIsSleeping(false);
	}
	
	/// Set the maximum motor force
	/**
	 * @param maxMotorForce The maximum force of the joint motor (in Newton x meters)
	 */
	public void setMaxMotorForce(final float maxMotorForce) {
		
		if (maxMotorForce != this.maxMotorForce) {
			
			assert (this.maxMotorForce >= 0.0f);
			this.maxMotorForce = maxMotorForce;
			
			// Wake up the two bodies of the joint
			this.body1.setIsSleeping(false);
			this.body2.setIsSleeping(false);
		}
	}
	
	/// Set the maximum translation limit
	/**
	 * @param lowerLimit The maximum translation limit of the joint (in meters)
	 */
	public void setMaxTranslationLimit(final float upperLimit) {
		
		assert (this.lowerLimit <= upperLimit);
		
		if (upperLimit != this.upperLimit) {
			
			this.upperLimit = upperLimit;
			
			// Reset the limits
			resetLimits();
		}
	}
	
	/// Set the minimum translation limit
	/**
	 * @param lowerLimit The minimum translation limit of the joint (in meters)
	 */
	public void setMinTranslationLimit(final float lowerLimit) {
		
		assert (lowerLimit <= this.upperLimit);
		
		if (lowerLimit != this.lowerLimit) {
			
			this.lowerLimit = lowerLimit;
			
			// Reset the limits
			resetLimits();
		}
	}
	
	/// Set the motor speed
	/**
	 * @param motorSpeed The speed of the joint motor (in meters per second)
	 */
	public void setMotorSpeed(final float motorSpeed) {
		
		if (motorSpeed != this.motorSpeed) {
			
			this.motorSpeed = motorSpeed;
			
			// Wake up the two bodies of the joint
			this.body1.setIsSleeping(false);
			this.body2.setIsSleeping(false);
		}
	}
	
	@Override
	public void solvePositionConstraint(final ConstraintSolverData raintSolverData) {
		
		// If the error position correction technique is not the non-linear-gauss-seidel, we do
		// do not execute this method
		if (this.positionCorrectionTechnique != JointsPositionCorrectionTechnique.NON_LINEAR_GAUSS_SEIDEL) {
			return;
		}
		
		// Get the bodies positions and orientations
		final Vector3f x1 = raintSolverData.positions[this.indexBody1];
		final Vector3f x2 = raintSolverData.positions[this.indexBody2];
		final Quaternion q1 = raintSolverData.orientations[this.indexBody1];
		final Quaternion q2 = raintSolverData.orientations[this.indexBody2];
		
		// Get the inverse mass and inverse inertia tensors of the bodies
		final float inverseMassBody1 = this.body1.massInverse;
		final float inverseMassBody2 = this.body2.massInverse;
		
		// Recompute the inertia tensor of bodies
		this.i1 = this.body1.getInertiaTensorInverseWorld();
		this.i2 = this.body2.getInertiaTensorInverseWorld();
		
		// Vector from body center to the anchor point
		this.R1 = q1.multiply(this.localAnchorPointBody1);
		this.R2 = q2.multiply(this.localAnchorPointBody2);
		
		// Compute the vector u (difference between anchor points)
		final Vector3f u = x2.addNew(this.R2).less(x1).less(this.R1);
		
		// Compute the two orthogonal vectors to the slider axis in world-space
		this.sliderAxisWorld = q1.multiply(this.sliderAxisBody1);
		this.sliderAxisWorld.normalize();
		this.N1 = this.sliderAxisWorld.getOrthoVector();
		this.N2 = this.sliderAxisWorld.cross(this.N1);
		
		// Check if the limit raints are violated or not
		final float uDotSliderAxis = u.dot(this.sliderAxisWorld);
		final float lowerLimitError = uDotSliderAxis - this.lowerLimit;
		final float upperLimitError = this.upperLimit - uDotSliderAxis;
		this.isLowerLimitViolated = lowerLimitError <= 0;
		this.isUpperLimitViolated = upperLimitError <= 0;
		
		// Compute the cross products used in the Jacobians
		this.R2CrossN1 = this.R2.cross(this.N1);
		this.R2CrossN2 = this.R2.cross(this.N2);
		this.R2CrossSliderAxis = this.R2.cross(this.sliderAxisWorld);
		final Vector3f r1PlusU = this.R1.addNew(u);
		this.R1PlusUCrossN1 = (r1PlusU).cross(this.N1);
		this.R1PlusUCrossN2 = (r1PlusU).cross(this.N2);
		this.R1PlusUCrossSliderAxis = (r1PlusU).cross(this.sliderAxisWorld);
		
		// --------------- Translation Constraints --------------- //
		
		// Recompute the inverse of the mass matrix K=JM^-1J^t for the 2 translation
		// raints (2x2 matrix)
		final float sumInverseMass = this.body1.massInverse + this.body2.massInverse;
		final Vector3f I1R1PlusUCrossN1 = this.i1.multiplyNew(this.R1PlusUCrossN1);
		final Vector3f I1R1PlusUCrossN2 = this.i1.multiplyNew(this.R1PlusUCrossN2);
		final Vector3f I2R2CrossN1 = this.i2.multiplyNew(this.R2CrossN1);
		final Vector3f I2R2CrossN2 = this.i2.multiplyNew(this.R2CrossN2);
		final float el11 = sumInverseMass + this.R1PlusUCrossN1.dot(I1R1PlusUCrossN1) + this.R2CrossN1.dot(I2R2CrossN1);
		final float el12 = this.R1PlusUCrossN1.dot(I1R1PlusUCrossN2) + this.R2CrossN1.dot(I2R2CrossN2);
		final float el21 = this.R1PlusUCrossN2.dot(I1R1PlusUCrossN1) + this.R2CrossN2.dot(I2R2CrossN1);
		final float el22 = sumInverseMass + this.R1PlusUCrossN2.dot(I1R1PlusUCrossN2) + this.R2CrossN2.dot(I2R2CrossN2);
		
		final Matrix2f matrixKTranslation = new Matrix2f(el11, el12, el21, el22);
		this.inverseMassMatrixTranslationConstraint.setZero();
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixTranslationConstraint = matrixKTranslation.inverseNew();
		}
		
		// Compute the position error for the 2 translation raints
		final Vector2f translationError = new Vector2f(u.dot(this.N1), u.dot(this.N2));
		
		// Compute the Lagrange multiplier lambda for the 2 translation raints
		final Vector2f lambdaTranslation = this.inverseMassMatrixTranslationConstraint.multiplyNew(translationError.multiplyNew(-1));
		
		// Compute the impulse P=J^T * lambda for the 2 translation raints of body 1
		final Vector3f linearImpulseBody1 = this.N1.multiplyNew(-1).multiplyNew(lambdaTranslation.x).lessNew(this.N2.multiplyNew(lambdaTranslation.y));
		Vector3f angularImpulseBody1 = this.R1PlusUCrossN1.multiplyNew(-1).multiplyNew(lambdaTranslation.x).lessNew(this.R1PlusUCrossN2.multiplyNew(lambdaTranslation.y));
		
		// Apply the impulse to the body 1
		final Vector3f v1 = linearImpulseBody1.multiplyNew(inverseMassBody1);
		Vector3f w1 = this.i1.multiplyNew(angularImpulseBody1);
		
		// Update the body position/orientation of body 1
		x1.add(v1);
		q1.add((new Quaternion(0, w1)).multiply(q1).multiply(0.5f));
		q1.normalize();
		
		// Compute the impulse P=J^T * lambda for the 2 translation raints of body 2
		final Vector3f linearImpulseBody2 = this.N1.multiplyNew(lambdaTranslation.x).addNew(this.N2.multiplyNew(lambdaTranslation.y));
		Vector3f angularImpulseBody2 = this.R2CrossN1.multiplyNew(lambdaTranslation.x).add(this.R2CrossN2.multiplyNew(lambdaTranslation.y));
		
		// Apply the impulse to the body 2
		final Vector3f v2 = linearImpulseBody2.multiplyNew(inverseMassBody2);
		Vector3f w2 = this.i2.multiplyNew(angularImpulseBody2);
		
		// Update the body position/orientation of body 2
		x2.add(v2);
		q2.add((new Quaternion(0, w2)).multiply(q2).multiply(0.5f));
		q2.normalize();
		
		// --------------- Rotation Constraints --------------- //
		
		// Compute the inverse of the mass matrix K=JM^-1J^t for the 3 rotation
		// contraints (3x3 matrix)
		this.inverseMassMatrixRotationConstraint = this.i1.addNew(this.i2);
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixRotationConstraint = this.inverseMassMatrixRotationConstraint.inverseNew();
		}
		
		// Compute the position error for the 3 rotation raints
		final Quaternion currentOrientationDifference = q2.multiplyNew(q1.inverseNew());
		currentOrientationDifference.normalize();
		final Quaternion qError = currentOrientationDifference.multiplyNew(this.initOrientationDifferenceInv);
		final Vector3f errorRotation = qError.getVectorV().multiply(2.0f);
		
		// Compute the Lagrange multiplier lambda for the 3 rotation raints
		final Vector3f lambdaRotation = this.inverseMassMatrixRotationConstraint.multiplyNew(errorRotation.multiplyNew(-1));
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints of body 1
		angularImpulseBody1 = lambdaRotation.multiplyNew(-1);
		
		// Apply the impulse to the body 1
		w1 = this.i1.multiplyNew(angularImpulseBody1);
		
		// Update the body position/orientation of body 1
		q1.add((new Quaternion(0, w1)).multiply(q1).multiply(0.5f));
		q1.normalize();
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints of body 2
		angularImpulseBody2 = lambdaRotation;
		
		// Apply the impulse to the body 2
		w2 = this.i2.multiplyNew(angularImpulseBody2);
		
		// Update the body position/orientation of body 2
		q2.add((new Quaternion(0, w2)).multiply(q2).multiply(0.5f));
		q2.normalize();
		
		// --------------- Limits Constraints --------------- //
		
		if (this.isLimitEnabled) {
			
			if (this.isLowerLimitViolated || this.isUpperLimitViolated) {
				
				// Compute the inverse of the mass matrix K=JM^-1J^t for the limits (1x1 matrix)
				this.inverseMassMatrixLimit = this.body1.massInverse + this.body2.massInverse + this.R1PlusUCrossSliderAxis.dot(this.i1.multiplyNew(this.R1PlusUCrossSliderAxis))
						+ this.R2CrossSliderAxis.dot(this.i2.multiplyNew(this.R2CrossSliderAxis));
				this.inverseMassMatrixLimit = (this.inverseMassMatrixLimit > 0.0f) ? 1.0f / this.inverseMassMatrixLimit : 0.0f;
			}
			
			// If the lower limit is violated
			if (this.isLowerLimitViolated) {
				
				// Compute the Lagrange multiplier lambda for the lower limit raint
				final float lambdaLowerLimit = this.inverseMassMatrixLimit * (-lowerLimitError);
				
				// Compute the impulse P=J^T * lambda for the lower limit raint of body 1
				final Vector3f linearImpulseBody1_tmp = this.sliderAxisWorld.multiplyNew(-lambdaLowerLimit);
				final Vector3f angularImpulseBody1_tmp = this.R1PlusUCrossSliderAxis.multiplyNew(-lambdaLowerLimit);
				
				// Apply the impulse to the body 1
				final Vector3f v1_tmp = linearImpulseBody1_tmp.multiplyNew(inverseMassBody1);
				final Vector3f w1_tmp = this.i1.multiplyNew(angularImpulseBody1_tmp);
				
				// Update the body position/orientation of body 1
				x1.add(v1_tmp);
				q1.add((new Quaternion(0, w1_tmp)).multiply(q1).multiply(0.5f));
				q1.normalize();
				
				// Compute the impulse P=J^T * lambda for the lower limit raint of body 2
				final Vector3f linearImpulseBody2_tmp = this.sliderAxisWorld.multiplyNew(lambdaLowerLimit);
				final Vector3f angularImpulseBody2_tmp = this.R2CrossSliderAxis.multiplyNew(lambdaLowerLimit);
				
				// Apply the impulse to the body 2
				final Vector3f v2_tmp = linearImpulseBody2_tmp.multiplyNew(inverseMassBody2);
				final Vector3f w2_tmp = this.i2.multiplyNew(angularImpulseBody2_tmp);
				
				// Update the body position/orientation of body 2
				x2.add(v2_tmp);
				q2.add((new Quaternion(0, w2_tmp)).multiply(q2).multiply(0.5f));
				q2.normalize();
			}
			
			// If the upper limit is violated
			if (this.isUpperLimitViolated) {
				
				// Compute the Lagrange multiplier lambda for the upper limit raint
				final float lambdaUpperLimit = this.inverseMassMatrixLimit * (-upperLimitError);
				
				// Compute the impulse P=J^T * lambda for the upper limit raint of body 1
				final Vector3f linearImpulseBody1_tmp = this.sliderAxisWorld.multiplyNew(lambdaUpperLimit);
				final Vector3f angularImpulseBody1_tmp = this.R1PlusUCrossSliderAxis.multiplyNew(lambdaUpperLimit);
				
				// Apply the impulse to the body 1
				final Vector3f v1_tmp = linearImpulseBody1_tmp.multiplyNew(inverseMassBody1);
				final Vector3f w1_tmp = this.i1.multiplyNew(angularImpulseBody1_tmp);
				
				// Update the body position/orientation of body 1
				x1.add(v1_tmp);
				q1.add((new Quaternion(0, w1_tmp)).multiply(q1).multiply(0.5f));
				q1.normalize();
				
				// Compute the impulse P=J^T * lambda for the upper limit raint of body 2
				final Vector3f linearImpulseBody2_tmp = this.sliderAxisWorld.multiplyNew(-lambdaUpperLimit);
				final Vector3f angularImpulseBody2_tmp = this.R2CrossSliderAxis.multiplyNew(-lambdaUpperLimit);
				
				// Apply the impulse to the body 2
				final Vector3f v2_tmp = linearImpulseBody2_tmp.multiplyNew(inverseMassBody2);
				final Vector3f w2_tmp = this.i2.multiplyNew(angularImpulseBody2_tmp);
				
				// Update the body position/orientation of body 2
				x2.add(v2_tmp);
				q2.add((new Quaternion(0, w2_tmp)).multiply(q2).multiply(0.5f));
				q2.normalize();
			}
		}
	}
	
	@Override
	public void solveVelocityConstraint(final ConstraintSolverData raintSolverData) {
		
		// Get the velocities
		final Vector3f v1 = raintSolverData.linearVelocities[this.indexBody1];
		final Vector3f v2 = raintSolverData.linearVelocities[this.indexBody2];
		final Vector3f w1 = raintSolverData.angularVelocities[this.indexBody1];
		final Vector3f w2 = raintSolverData.angularVelocities[this.indexBody2];
		
		// Get the inverse mass and inverse inertia tensors of the bodies
		final float inverseMassBody1 = this.body1.massInverse;
		final float inverseMassBody2 = this.body2.massInverse;
		
		// --------------- Translation Constraints --------------- //
		
		// Compute J*v for the 2 translation raints
		final float el1 = -this.N1.dot(v1) - w1.dot(this.R1PlusUCrossN1) + this.N1.dot(v2) + w2.dot(this.R2CrossN1);
		final float el2 = -this.N2.dot(v1) - w1.dot(this.R1PlusUCrossN2) + this.N2.dot(v2) + w2.dot(this.R2CrossN2);
		
		final Vector2f JvTranslation = new Vector2f(el1, el2);
		
		// Compute the Lagrange multiplier lambda for the 2 translation raints
		final Vector2f deltaLambda = this.inverseMassMatrixTranslationConstraint.multiplyNew(JvTranslation.multiplyNew(-1).less(this.bTranslation));
		this.impulseTranslation.add(deltaLambda);
		
		// Compute the impulse P=J^T * lambda for the 2 translation raints of body 1
		final Vector3f linearImpulseBody1 = this.N1.multiplyNew(-1).multiply(deltaLambda.x).less(this.N2.multiplyNew(deltaLambda.y));
		Vector3f angularImpulseBody1 = this.R1PlusUCrossN1.multiplyNew(-1).multiply(deltaLambda.x).less(this.R1PlusUCrossN2.multiplyNew(deltaLambda.y));
		
		// Apply the impulse to the body 1
		v1.add(linearImpulseBody1.multiplyNew(inverseMassBody1));
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda for the 2 translation raints of body 2
		final Vector3f linearImpulseBody2 = this.N1.multiplyNew(deltaLambda.x).add(this.N2.multiplyNew(deltaLambda.y));
		Vector3f angularImpulseBody2 = this.R2CrossN1.multiplyNew(deltaLambda.x).add(this.R2CrossN2.multiplyNew(deltaLambda.y));
		
		// Apply the impulse to the body 2
		v2.add(linearImpulseBody2.multiplyNew(inverseMassBody2));
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
		
		// --------------- Rotation Constraints --------------- //
		
		// Compute J*v for the 3 rotation raints
		final Vector3f JvRotation = w2.lessNew(w1);
		
		// Compute the Lagrange multiplier lambda for the 3 rotation raints
		final Vector3f deltaLambda2 = this.inverseMassMatrixRotationConstraint.multiplyNew(JvRotation.multiplyNew(-1).less(this.bRotation));
		this.impulseRotation.add(deltaLambda2);
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints of body 1
		angularImpulseBody1 = deltaLambda2.multiplyNew(-1);
		
		// Apply the impulse to the body to body 1
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints of body 2
		angularImpulseBody2 = deltaLambda2;
		
		// Apply the impulse to the body 2
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
		
		// --------------- Limits Constraints --------------- //
		
		if (this.isLimitEnabled) {
			
			// If the lower limit is violated
			if (this.isLowerLimitViolated) {
				
				// Compute J*v for the lower limit raint
				final float JvLowerLimit = this.sliderAxisWorld.dot(v2) + this.R2CrossSliderAxis.dot(w2) - this.sliderAxisWorld.dot(v1) - this.R1PlusUCrossSliderAxis.dot(w1);
				
				// Compute the Lagrange multiplier lambda for the lower limit raint
				float deltaLambdaLower = this.inverseMassMatrixLimit * (-JvLowerLimit - this.bLowerLimit);
				final float lambdaTemp = this.impulseLowerLimit;
				this.impulseLowerLimit = FMath.max(this.impulseLowerLimit + deltaLambdaLower, 0.0f);
				deltaLambdaLower = this.impulseLowerLimit - lambdaTemp;
				
				// Compute the impulse P=J^T * lambda for the lower limit raint of body 1
				final Vector3f linearImpulseBody1_tmp = this.sliderAxisWorld.multiplyNew(-deltaLambdaLower);
				final Vector3f angularImpulseBody1_tmp = this.R1PlusUCrossSliderAxis.multiplyNew(-deltaLambdaLower);
				
				// Apply the impulse to the body 1
				v1.add(linearImpulseBody1_tmp.multiplyNew(inverseMassBody1));
				w1.add(this.i1.multiplyNew(angularImpulseBody1_tmp));
				
				// Compute the impulse P=J^T * lambda for the lower limit raint of body 2
				final Vector3f linearImpulseBody2_tmp = this.sliderAxisWorld.multiplyNew(deltaLambdaLower);
				final Vector3f angularImpulseBody2_tmp = this.R2CrossSliderAxis.multiplyNew(deltaLambdaLower);
				
				// Apply the impulse to the body 2
				v2.add(linearImpulseBody2_tmp.multiplyNew(inverseMassBody2));
				w2.add(this.i2.multiplyNew(angularImpulseBody2_tmp));
			}
			
			// If the upper limit is violated
			if (this.isUpperLimitViolated) {
				
				// Compute J*v for the upper limit raint
				final float JvUpperLimit = this.sliderAxisWorld.dot(v1) + this.R1PlusUCrossSliderAxis.dot(w1) - this.sliderAxisWorld.dot(v2) - this.R2CrossSliderAxis.dot(w2);
				
				// Compute the Lagrange multiplier lambda for the upper limit raint
				float deltaLambdaUpper = this.inverseMassMatrixLimit * (-JvUpperLimit - this.bUpperLimit);
				final float lambdaTemp = this.impulseUpperLimit;
				this.impulseUpperLimit = FMath.max(this.impulseUpperLimit + deltaLambdaUpper, 0.0f);
				deltaLambdaUpper = this.impulseUpperLimit - lambdaTemp;
				
				// Compute the impulse P=J^T * lambda for the upper limit raint of body 1
				final Vector3f linearImpulseBody1_tmp = this.sliderAxisWorld.multiplyNew(deltaLambdaUpper);
				final Vector3f angularImpulseBody1_tmp = this.R1PlusUCrossSliderAxis.multiplyNew(deltaLambdaUpper);
				
				// Apply the impulse to the body 1
				v1.add(linearImpulseBody1_tmp.multiplyNew(inverseMassBody1));
				w1.add(this.i1.multiplyNew(angularImpulseBody1_tmp));
				
				// Compute the impulse P=J^T * lambda for the upper limit raint of body 2
				final Vector3f linearImpulseBody2_tmp = this.sliderAxisWorld.multiplyNew(-deltaLambdaUpper);
				final Vector3f angularImpulseBody2_tmp = this.R2CrossSliderAxis.multiplyNew(-deltaLambdaUpper);
				
				// Apply the impulse to the body 2
				v2.add(linearImpulseBody2_tmp.multiplyNew(inverseMassBody2));
				w2.add(this.i2.multiplyNew(angularImpulseBody2_tmp));
			}
		}
		
		// --------------- Motor --------------- //
		
		if (this.isMotorEnabled) {
			
			// Compute J*v for the motor
			final float JvMotor = this.sliderAxisWorld.dot(v1) - this.sliderAxisWorld.dot(v2);
			
			// Compute the Lagrange multiplier lambda for the motor
			final float maxMotorImpulse = this.maxMotorForce * raintSolverData.timeStep;
			float deltaLambdaMotor = this.inverseMassMatrixMotor * (-JvMotor - this.motorSpeed);
			final float lambdaTemp = this.impulseMotor;
			this.impulseMotor = FMath.clamp(this.impulseMotor + deltaLambdaMotor, -maxMotorImpulse, maxMotorImpulse);
			deltaLambdaMotor = this.impulseMotor - lambdaTemp;
			
			// Compute the impulse P=J^T * lambda for the motor of body 1
			final Vector3f linearImpulseBody1_tmp = this.sliderAxisWorld.multiplyNew(deltaLambdaMotor);
			
			// Apply the impulse to the body 1
			v1.add(linearImpulseBody1_tmp.multiplyNew(inverseMassBody1));
			
			// Compute the impulse P=J^T * lambda for the motor of body 2
			final Vector3f linearImpulseBody2_tmp = this.sliderAxisWorld.multiplyNew(-deltaLambdaMotor);
			
			// Apply the impulse to the body 2
			v2.add(linearImpulseBody2_tmp.multiplyNew(inverseMassBody2));
		}
	}
	
	@Override
	public void warmstart(final ConstraintSolverData raintSolverData) {
		// Get the velocities
		final Vector3f v1 = raintSolverData.linearVelocities[this.indexBody1];
		final Vector3f v2 = raintSolverData.linearVelocities[this.indexBody2];
		final Vector3f w1 = raintSolverData.angularVelocities[this.indexBody1];
		final Vector3f w2 = raintSolverData.angularVelocities[this.indexBody2];
		
		// Get the inverse mass and inverse inertia tensors of the bodies
		final float inverseMassBody1 = this.body1.massInverse;
		final float inverseMassBody2 = this.body2.massInverse;
		
		// Compute the impulse P=J^T * lambda for the lower and upper limits raints of body 1
		final float impulseLimits = this.impulseUpperLimit - this.impulseLowerLimit;
		final Vector3f linearImpulseLimits = this.sliderAxisWorld.multiplyNew(impulseLimits);
		
		// Compute the impulse P=J^T * lambda for the motor raint of body 1
		final Vector3f impulseMotor = this.sliderAxisWorld.multiplyNew(this.impulseMotor);
		
		// Compute the impulse P=J^T * lambda for the 2 translation raints of body 1
		final Vector3f linearImpulseBody1 = this.N1.multiplyNew(-1).multiply(this.impulseTranslation.x).less(this.N2.multiplyNew(this.impulseTranslation.y));
		final Vector3f angularImpulseBody1 = this.R1PlusUCrossN1.multiplyNew(-1).multiply(this.impulseTranslation.x).less(this.R1PlusUCrossN2.multiplyNew(this.impulseTranslation.y));
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints of body 1
		angularImpulseBody1.add(this.impulseRotation.multiplyNew(-1));
		
		// Compute the impulse P=J^T * lambda for the lower and upper limits raints of body 1
		linearImpulseBody1.add(linearImpulseLimits);
		angularImpulseBody1.add(this.R1PlusUCrossSliderAxis.multiplyNew(impulseLimits));
		
		// Compute the impulse P=J^T * lambda for the motor raint of body 1
		linearImpulseBody1.add(impulseMotor);
		
		// Apply the impulse to the body 1
		v1.add(linearImpulseBody1.multiplyNew(inverseMassBody1));
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda for the 2 translation raints of body 2
		final Vector3f linearImpulseBody2 = this.N1.multiplyNew(this.impulseTranslation.x).add(this.N2.multiplyNew(this.impulseTranslation.y));
		final Vector3f angularImpulseBody2 = this.R2CrossN1.multiplyNew(this.impulseTranslation.x).add(this.R2CrossN2.multiplyNew(this.impulseTranslation.y));
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints of body 2
		angularImpulseBody2.add(this.impulseRotation);
		
		// Compute the impulse P=J^T * lambda for the lower and upper limits raints of body 2
		linearImpulseBody2.less(linearImpulseLimits);
		angularImpulseBody2.add(this.R2CrossSliderAxis.multiplyNew(-impulseLimits));
		
		// Compute the impulse P=J^T * lambda for the motor raint of body 2
		linearImpulseBody2.add(impulseMotor.multiplyNew(-1));
		
		// Apply the impulse to the body 2
		v2.add(linearImpulseBody2.multiplyNew(inverseMassBody2));
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
	}
	
}
