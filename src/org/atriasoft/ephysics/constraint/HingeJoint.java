package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Constant;
import org.atriasoft.etk.math.FMath;
import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Quaternion;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.BodyType;
import org.atriasoft.ephysics.engine.ConstraintSolverData;
import org.atriasoft.ephysics.mathematics.Matrix2f;

/**
 *  It represents a hinge joint that allows arbitrary rotation
 * between two bodies around a single axis. This joint has one degree of freedom. It
 * can be useful to simulate doors or pendulumns.
 */
public class HingeJoint extends Joint {
	
	private static float BETA; //!< Beta value for the bias factor of position correction
	private final Vector3f localAnchorPointBody1; //!< Anchor point of body 1 (in local-space coordinates of body 1)
	private final Vector3f localAnchorPointBody2; //!< Anchor point of body 2 (in local-space coordinates of body 2)
	private final Vector3f hingeLocalAxisBody1; //!< Hinge rotation axis (in local-space coordinates of body 1)
	private final Vector3f hingeLocalAxisBody2; //!< Hinge rotation axis (in local-space coordiantes of body 2)
	private Matrix3f i1; //!< Inertia tensor of body 1 (in world-space coordinates)
	private Matrix3f i2; //!< Inertia tensor of body 2 (in world-space coordinates)
	private Vector3f mA1; //!< Hinge rotation axis (in world-space coordinates) computed from body 1
	private Vector3f r1World; //!< Vector from center of body 2 to anchor point in world-space
	private Vector3f r2World; //!< Vector from center of body 2 to anchor point in world-space
	private Vector3f b2CrossA1; //!< Cross product of vector b2 and a1
	private Vector3f c2CrossA1; //!< Cross product of vector c2 and a1;
	private final Vector3f impulseTranslation; //!< Impulse for the 3 translation raints
	private final Vector2f impulseRotation; //!< Impulse for the 2 rotation raints
	private float impulseLowerLimit; //!< Accumulated impulse for the lower limit raint
	private float impulseUpperLimit; //!< Accumulated impulse for the upper limit raint
	private float impulseMotor; //!< Accumulated impulse for the motor raint;
	private Matrix3f inverseMassMatrixTranslation; //!< Inverse mass matrix K=JM^-1J^t for the 3 translation raints
	private Matrix2f inverseMassMatrixRotation; //!< Inverse mass matrix K=JM^-1J^t for the 2 rotation raints
	private float inverseMassMatrixLimitMotor; //!< Inverse of mass matrix K=JM^-1J^t for the limits and motor raints (1x1 matrix)
	private float inverseMassMatrixMotor; //!< Inverse of mass matrix K=JM^-1J^t for the motor
	private Vector3f bTranslation; //!< Bias vector for the error correction for the translation raints
	private Vector2f bRotation; //!< Bias vector for the error correction for the rotation raints
	private float bLowerLimit; //!< Bias of the lower limit raint
	private float bUpperLimit; //!< Bias of the upper limit raint
	private final Quaternion initOrientationDifferenceInv; //!< Inverse of the initial orientation difference between the bodies
	private boolean isLimitEnabled; //!< True if the joint limits are enabled
	private boolean isMotorEnabled; //!< True if the motor of the joint in enabled
	private float lowerLimit; //!< Lower limit (minimum allowed rotation angle in radian)
	private float upperLimit; //!< Upper limit (maximum translation distance)
	private boolean isLowerLimitViolated; //!< True if the lower limit is violated
	private boolean isUpperLimitViolated; //!< True if the upper limit is violated
	private float motorSpeed; //!< Motor speed (in rad/s)
	private float maxMotorTorque; //!< Maximum motor torque (in Newtons) that can be applied to reach to desired motor speed
	/// Reset the limits
	
	/// Constructor
	public HingeJoint(final HingeJointInfo jointInfo) {
		super(jointInfo);
		this.impulseTranslation = new Vector3f(0, 0, 0);
		this.impulseRotation = new Vector2f(0, 0);
		this.impulseLowerLimit = 0;
		this.impulseUpperLimit = 0;
		this.impulseMotor = 0;
		this.isLimitEnabled = jointInfo.isLimitEnabled;
		this.isMotorEnabled = jointInfo.isMotorEnabled;
		this.lowerLimit = jointInfo.minAngleLimit;
		this.upperLimit = jointInfo.maxAngleLimit;
		
		this.isLowerLimitViolated = false;
		this.isUpperLimitViolated = false;
		this.motorSpeed = jointInfo.motorSpeed;
		this.maxMotorTorque = jointInfo.maxMotorTorque;
		
		assert (this.lowerLimit <= 0 && this.lowerLimit >= -2.0 * Constant.PI);
		assert (this.upperLimit >= 0 && this.upperLimit <= 2.0 * Constant.PI);
		
		// Compute the local-space anchor point for each body
		final Transform3D transform1 = this.body1.getTransform();
		final Transform3D transform2 = this.body2.getTransform();
		this.localAnchorPointBody1 = transform1.inverseNew().multiply(jointInfo.anchorPointWorldSpace);
		this.localAnchorPointBody2 = transform2.inverseNew().multiply(jointInfo.anchorPointWorldSpace);
		
		// Compute the local-space hinge axis
		this.hingeLocalAxisBody1 = transform1.getOrientation().inverseNew().multiply(jointInfo.rotationAxisWorld);
		this.hingeLocalAxisBody2 = transform2.getOrientation().inverseNew().multiply(jointInfo.rotationAxisWorld);
		this.hingeLocalAxisBody1.normalize();
		this.hingeLocalAxisBody2.normalize();
		
		// Compute the inverse of the initial orientation difference between the two bodies
		this.initOrientationDifferenceInv = transform2.getOrientation().multiplyNew(transform1.getOrientation().inverseNew());
		this.initOrientationDifferenceInv.normalize();
		this.initOrientationDifferenceInv.inverse();
	}
	
	/// Given an "inputAngle" in the range [-pi, pi], this method returns an
	/// angle (modulo 2*pi) in the range [-2*pi; 2*pi] that is closest to one of the
	/// two angle limits in arguments.
	private float computeCorrespondingAngleNearLimits(final float inputAngle, final float lowerLimitAngle, final float upperLimitAngle) {
		if (upperLimitAngle <= lowerLimitAngle) {
			return inputAngle;
		} else if (inputAngle > upperLimitAngle) {
			final float diffToUpperLimit = FMath.abs(computeNormalizedAngle(inputAngle - upperLimitAngle));
			final float diffToLowerLimit = FMath.abs(computeNormalizedAngle(inputAngle - lowerLimitAngle));
			return (diffToUpperLimit > diffToLowerLimit) ? (inputAngle - Constant.PI_2) : inputAngle;
		} else if (inputAngle < lowerLimitAngle) {
			final float diffToUpperLimit = FMath.abs(computeNormalizedAngle(upperLimitAngle - inputAngle));
			final float diffToLowerLimit = FMath.abs(computeNormalizedAngle(lowerLimitAngle - inputAngle));
			return (diffToUpperLimit > diffToLowerLimit) ? inputAngle : (inputAngle + Constant.PI_2);
		} else {
			return inputAngle;
		}
	}
	
	/// Compute the current angle around the hinge axis
	private float computeCurrentHingeAngle(final Quaternion orientationBody1, final Quaternion orientationBody2) {
		
		float hingeAngle;
		
		// Compute the current orientation difference between the two bodies
		final Quaternion currentOrientationDiff = orientationBody2.multiplyNew(orientationBody1.inverseNew());
		currentOrientationDiff.normalize();
		
		// Compute the relative rotation idering the initial orientation difference
		final Quaternion relativeRotation = currentOrientationDiff.multiplyNew(this.initOrientationDifferenceInv);
		relativeRotation.normalize();
		
		// A quaternion q = [cos(theta/2); sin(theta/2) * rotAxis] where rotAxis is a unit
		// length vector. We can extract cos(theta/2) with q.w and we can extract |sin(theta/2)| with :
		// |sin(theta/2)| = q.getVectorV().length() since rotAxis is unit length. Note that any
		// rotation can be represented by a quaternion q and -q. Therefore, if the relative rotation
		// axis is not pointing in the same direction as the hinge axis, we use the rotation -q which
		// has the same |sin(theta/2)| value but the value cos(theta/2) is sign inverted. Some details
		// about this trick is explained in the source code of OpenTissue (http://www.opentissue.org).
		final float cosHalfAngle = relativeRotation.getW();
		final float sinHalfAngleAbs = relativeRotation.getVectorV().length();
		
		// Compute the dot product of the relative rotation axis and the hinge axis
		final float dotProduct = relativeRotation.getVectorV().dot(this.mA1);
		
		// If the relative rotation axis and the hinge axis are pointing the same direction
		if (dotProduct >= 0.0f) {
			hingeAngle = 2.0f * FMath.atan2(sinHalfAngleAbs, cosHalfAngle);
		} else {
			hingeAngle = 2.0f * FMath.atan2(sinHalfAngleAbs, -cosHalfAngle);
		}
		
		// Convert the angle from range [-2*pi; 2*pi] into the range [-pi; pi]
		hingeAngle = computeNormalizedAngle(hingeAngle);
		
		// Compute and return the corresponding angle near one the two limits
		return computeCorrespondingAngleNearLimits(hingeAngle, this.lowerLimit, this.upperLimit);
	}
	
	/// Given an angle in radian, this method returns the corresponding
	/// angle in the range [-pi; pi]
	private float computeNormalizedAngle(float angle) {
		
		// Convert it into the range [-2*pi; 2*pi]
		angle = FMath.mod(angle, Constant.PI_2);
		
		// Convert it into the range [-pi; pi]
		if (angle < -Constant.PI) {
			return angle + Constant.PI_2;
		} else if (angle > Constant.PI) {
			return angle - Constant.PI_2;
		} else {
			return angle;
		}
	}
	
	/**Enable/Disable the limits of the joint
	 * @param isLimitEnabled True if you want to enable the limits of the joint and
	 *					   false otherwise
	 */
	public void enableLimit(final boolean isLimitEnabled) {
		
		if (this.isLimitEnabled != isLimitEnabled) {
			
			this.isLimitEnabled = isLimitEnabled;
			
			// Reset the limits
			resetLimits();
		}
	}
	
	/**Enable/Disable the motor of the joint
	 * @param isMotorEnabled True if you want to enable the motor of the joint and false otherwise
	 */
	public void enableMotor(final boolean isMotorEnabled) {
		
		this.isMotorEnabled = isMotorEnabled;
		this.impulseMotor = 0.0f;
		
		// Wake up the two bodies of the joint
		this.body1.setIsSleeping(false);
		this.body2.setIsSleeping(false);
	}
	
	/// Return the maximum angle limit
	/**
	 * @return The maximum limit angle of the joint (in radian)
	 */
	public float getMaxAngleLimit() {
		return this.upperLimit;
	}
	
	/// Return the maximum motor torque
	/**
	 * @return The maximum torque of the joint motor (in Newtons)
	 */
	public float getMaxMotorTorque() {
		return this.maxMotorTorque;
	}
	
	/// Return the minimum angle limit
	/**
	 * @return The minimum limit angle of the joint (in radian)
	 */
	public float getMinAngleLimit() {
		return this.lowerLimit;
	}
	
	/// Return the motor speed
	/**
	 * @return The current speed of the joint motor (in radian per second)
	 */
	public float getMotorSpeed() {
		return this.motorSpeed;
	}
	
	/// Return the intensity of the current torque applied for the joint motor
	/**
	 * @param timeStep The current time step (in seconds)
	 * @return The intensity of the current torque (in Newtons) of the joint motor
	 */
	public float getMotorTorque(final float timeStep) {
		return this.impulseMotor / timeStep;
	}
	
	@Override
	public void initBeforeSolve(final ConstraintSolverData raintSolverData) {
		
		// Initialize the bodies index in the velocity array
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
		
		// Compute the vector from body center to the anchor point in world-space
		this.r1World = orientationBody1.multiply(this.localAnchorPointBody1);
		this.r2World = orientationBody2.multiply(this.localAnchorPointBody2);
		
		// Compute the current angle around the hinge axis
		final float hingeAngle = computeCurrentHingeAngle(orientationBody1, orientationBody2);
		
		// Check if the limit raints are violated or not
		final float lowerLimitError = hingeAngle - this.lowerLimit;
		final float upperLimitError = this.upperLimit - hingeAngle;
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
		
		// Compute vectors needed in the Jacobian
		this.mA1 = orientationBody1.multiply(this.hingeLocalAxisBody1);
		final Vector3f a2 = orientationBody2.multiply(this.hingeLocalAxisBody2);
		this.mA1.normalize();
		a2.normalize();
		final Vector3f b2 = a2.getOrthoVector();
		final Vector3f c2 = a2.cross(b2);
		this.b2CrossA1 = b2.cross(this.mA1);
		this.c2CrossA1 = c2.cross(this.mA1);
		
		// Compute the corresponding skew-symmetric matrices
		final Matrix3f skewSymmetricMatrixU1 = Matrix3f.computeSkewSymmetricMatrixForCrossProduct(this.r1World);
		final Matrix3f skewSymmetricMatrixU2 = Matrix3f.computeSkewSymmetricMatrixForCrossProduct(this.r2World);
		
		// Compute the inverse mass matrix K=JM^-1J^t for the 3 translation raints (3x3 matrix)
		final float inverseMassBodies = this.body1.massInverse + this.body2.massInverse;
		final Matrix3f massMatrix = new Matrix3f(inverseMassBodies, 0, 0, 0, inverseMassBodies, 0, 0, 0, inverseMassBodies);
		massMatrix.add(skewSymmetricMatrixU1.multiplyNew(this.i1).multiply(skewSymmetricMatrixU1.transposeNew()));
		massMatrix.add(skewSymmetricMatrixU2.multiplyNew(this.i2).multiply(skewSymmetricMatrixU2.transposeNew()));
		this.inverseMassMatrixTranslation.setZero();
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixTranslation = massMatrix.inverseNew();
		}
		
		// Compute the bias "b" of the translation raints
		this.bTranslation.setZero();
		final float biasFactor = (BETA / raintSolverData.timeStep);
		if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
			this.bTranslation = x2.addNew(this.r2World).less(x1).less(this.r1World).multiply(biasFactor);
		}
		
		// Compute the inverse mass matrix K=JM^-1J^t for the 2 rotation raints (2x2 matrix)
		final Vector3f I1B2CrossA1 = this.i1.multiplyNew(this.b2CrossA1);
		final Vector3f I1C2CrossA1 = this.i1.multiplyNew(this.c2CrossA1);
		final Vector3f I2B2CrossA1 = this.i2.multiplyNew(this.b2CrossA1);
		final Vector3f I2C2CrossA1 = this.i2.multiplyNew(this.c2CrossA1);
		final float el11 = this.b2CrossA1.dot(I1B2CrossA1) + this.b2CrossA1.dot(I2B2CrossA1);
		final float el12 = this.b2CrossA1.dot(I1C2CrossA1) + this.b2CrossA1.dot(I2C2CrossA1);
		final float el21 = this.c2CrossA1.dot(I1B2CrossA1) + this.c2CrossA1.dot(I2B2CrossA1);
		final float el22 = this.c2CrossA1.dot(I1C2CrossA1) + this.c2CrossA1.dot(I2C2CrossA1);
		
		final Matrix2f matrixKRotation = new Matrix2f(el11, el12, el21, el22);
		this.inverseMassMatrixRotation.setZero();
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixRotation = matrixKRotation.inverseNew();
		}
		
		// Compute the bias "b" of the rotation raints
		this.bRotation.setZero();
		if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
			this.bRotation = new Vector2f(this.mA1.dot(b2) * biasFactor, this.mA1.dot(c2) * biasFactor);
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
		
		// If the motor or limits are enabled
		if (this.isMotorEnabled || (this.isLimitEnabled && (this.isLowerLimitViolated || this.isUpperLimitViolated))) {
			
			// Compute the inverse of the mass matrix K=JM^-1J^t for the limits and motor (1x1 matrix)
			this.inverseMassMatrixLimitMotor = this.mA1.dot(this.i1.multiplyNew(this.mA1)) + this.mA1.dot(this.i2.multiplyNew(this.mA1));
			this.inverseMassMatrixLimitMotor = (this.inverseMassMatrixLimitMotor > 0.0f) ? 1.0f / this.inverseMassMatrixLimitMotor : 0.0f;
			
			if (this.isLimitEnabled) {
				
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
		}
		
	}
	
	/// Return true if the limits or the joint are enabled
	/**
	 * @return True if the limits of the joint are enabled and false otherwise
	 */
	public boolean isLimitEnabled() {
		return this.isLimitEnabled;
	}
	
	/// Return true if the motor of the joint is enabled
	/**
	 * @return True if the motor of joint is enabled and false otherwise
	 */
	public boolean isMotorEnabled() {
		return this.isMotorEnabled;
	}
	
	private void resetLimits() {
		
		// Reset the accumulated impulses for the limits
		this.impulseLowerLimit = 0.0f;
		this.impulseUpperLimit = 0.0f;
		
		// Wake up the two bodies of the joint
		this.body1.setIsSleeping(false);
		this.body2.setIsSleeping(false);
	}
	
	/// Set the maximum angle limit
	/**
	 * @param upperLimit The maximum limit angle of the joint (in radian)
	 */
	public void setMaxAngleLimit(final float upperLimit) {
		
		assert (upperLimit >= 0 && upperLimit <= 2.0 * Constant.PI);
		
		if (upperLimit != this.upperLimit) {
			
			this.upperLimit = upperLimit;
			
			// Reset the limits
			resetLimits();
		}
	}
	
	/// Set the maximum motor torque
	/**
	 * @param maxMotorTorque The maximum torque (in Newtons) of the joint motor
	 */
	public void setMaxMotorTorque(final float maxMotorTorque) {
		
		if (maxMotorTorque != this.maxMotorTorque) {
			
			assert (this.maxMotorTorque >= 0.0f);
			this.maxMotorTorque = maxMotorTorque;
			
			// Wake up the two bodies of the joint
			this.body1.setIsSleeping(false);
			this.body2.setIsSleeping(false);
		}
	}
	
	/// Set the minimum angle limit
	/**
	 * @param lowerLimit The minimum limit angle of the joint (in radian)
	 */
	public void setMinAngleLimit(final float lowerLimit) {
		
		assert (this.lowerLimit <= 0 && this.lowerLimit >= -2.0 * Constant.PI);
		
		if (lowerLimit != this.lowerLimit) {
			
			this.lowerLimit = lowerLimit;
			
			// Reset the limits
			resetLimits();
		}
	}
	
	/// Set the motor speed
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
		
		// Recompute the inverse inertia tensors
		this.i1 = this.body1.getInertiaTensorInverseWorld();
		this.i2 = this.body2.getInertiaTensorInverseWorld();
		
		// Compute the vector from body center to the anchor point in world-space
		this.r1World = q1.multiply(this.localAnchorPointBody1);
		this.r2World = q2.multiply(this.localAnchorPointBody2);
		
		// Compute the current angle around the hinge axis
		final float hingeAngle = computeCurrentHingeAngle(q1, q2);
		
		// Check if the limit raints are violated or not
		final float lowerLimitError = hingeAngle - this.lowerLimit;
		final float upperLimitError = this.upperLimit - hingeAngle;
		this.isLowerLimitViolated = lowerLimitError <= 0;
		this.isUpperLimitViolated = upperLimitError <= 0;
		
		// Compute vectors needed in the Jacobian
		this.mA1 = q1.multiply(this.hingeLocalAxisBody1);
		final Vector3f a2 = q2.multiply(this.hingeLocalAxisBody2);
		this.mA1.normalize();
		a2.normalize();
		final Vector3f b2 = a2.getOrthoVector();
		final Vector3f c2 = a2.cross(b2);
		this.b2CrossA1 = b2.cross(this.mA1);
		this.c2CrossA1 = c2.cross(this.mA1);
		
		// Compute the corresponding skew-symmetric matrices
		final Matrix3f skewSymmetricMatrixU1 = Matrix3f.computeSkewSymmetricMatrixForCrossProduct(this.r1World);
		final Matrix3f skewSymmetricMatrixU2 = Matrix3f.computeSkewSymmetricMatrixForCrossProduct(this.r2World);
		
		// --------------- Translation Constraints --------------- //
		
		// Compute the matrix K=JM^-1J^t (3x3 matrix) for the 3 translation raints
		final float inverseMassBodies = this.body1.massInverse + this.body2.massInverse;
		final Matrix3f massMatrix = new Matrix3f(inverseMassBodies, 0, 0, 0, inverseMassBodies, 0, 0, 0, inverseMassBodies);
		massMatrix.add(skewSymmetricMatrixU1.multiplyNew(this.i1).multiply(skewSymmetricMatrixU1.transposeNew()));
		massMatrix.add(skewSymmetricMatrixU2.multiplyNew(this.i2).multiply(skewSymmetricMatrixU2.transposeNew()));
		this.inverseMassMatrixTranslation.setZero();
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixTranslation = massMatrix.inverseNew();
		}
		
		// Compute position error for the 3 translation raints
		final Vector3f errorTranslation = x2.addNew(this.r2World).less(x1).less(this.r1World);
		
		// Compute the Lagrange multiplier lambda
		final Vector3f lambdaTranslation = this.inverseMassMatrixTranslation.multiplyNew(errorTranslation.multiplyNew(-1));
		
		// Compute the impulse of body 1
		final Vector3f linearImpulseBody1 = lambdaTranslation.multiplyNew(-1);
		Vector3f angularImpulseBody1 = lambdaTranslation.cross(this.r1World);
		
		// Compute the pseudo velocity of body 1
		final Vector3f v1 = linearImpulseBody1.multiplyNew(inverseMassBody1);
		Vector3f w1 = this.i1.multiplyNew(angularImpulseBody1);
		
		// Update the body position/orientation of body 1
		x1.add(v1);
		q1.add((new Quaternion(0, w1)).multiply(q1).multiply(0.5f));
		q1.normalize();
		
		// Compute the impulse of body 2
		Vector3f angularImpulseBody2 = lambdaTranslation.cross(this.r2World).multiplyNew(-1);
		
		// Compute the pseudo velocity of body 2
		final Vector3f v2 = lambdaTranslation.multiplyNew(inverseMassBody2);
		Vector3f w2 = this.i2.multiplyNew(angularImpulseBody2);
		
		// Update the body position/orientation of body 2
		x2.add(v2);
		q2.add((new Quaternion(0, w2)).multiply(q2).multiply(0.5f));
		q2.normalize();
		
		// --------------- Rotation Constraints --------------- //
		
		// Compute the inverse mass matrix K=JM^-1J^t for the 2 rotation raints (2x2 matrix)
		final Vector3f I1B2CrossA1 = this.i1.multiplyNew(this.b2CrossA1);
		final Vector3f I1C2CrossA1 = this.i1.multiplyNew(this.c2CrossA1);
		final Vector3f I2B2CrossA1 = this.i2.multiplyNew(this.b2CrossA1);
		final Vector3f I2C2CrossA1 = this.i2.multiplyNew(this.c2CrossA1);
		final float el11 = this.b2CrossA1.dot(I1B2CrossA1) + this.b2CrossA1.dot(I2B2CrossA1);
		final float el12 = this.b2CrossA1.dot(I1C2CrossA1) + this.b2CrossA1.dot(I2C2CrossA1);
		final float el21 = this.c2CrossA1.dot(I1B2CrossA1) + this.c2CrossA1.dot(I2B2CrossA1);
		final float el22 = this.c2CrossA1.dot(I1C2CrossA1) + this.c2CrossA1.dot(I2C2CrossA1);
		
		final Matrix2f matrixKRotation = new Matrix2f(el11, el12, el21, el22);
		this.inverseMassMatrixRotation.setZero();
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixRotation = matrixKRotation.inverseNew();
		}
		
		// Compute the position error for the 3 rotation raints
		final Vector2f errorRotation = new Vector2f(this.mA1.dot(b2), this.mA1.dot(c2));
		
		// Compute the Lagrange multiplier lambda for the 3 rotation raints
		final Vector2f lambdaRotation = this.inverseMassMatrixRotation.multiplyNew(errorRotation.multiplyNew(-1));
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints of body 1
		angularImpulseBody1 = this.b2CrossA1.multiplyNew(-1).multiply(lambdaRotation.x).less(this.c2CrossA1.multiplyNew(lambdaRotation.y));
		
		// Compute the pseudo velocity of body 1
		w1 = this.i1.multiplyNew(angularImpulseBody1);
		
		// Update the body position/orientation of body 1
		q1.add((new Quaternion(0, w1)).multiply(q1).multiply(0.5f));
		q1.normalize();
		
		// Compute the impulse of body 2
		angularImpulseBody2 = this.b2CrossA1.multiplyNew(lambdaRotation.x).add(this.c2CrossA1.multiplyNew(lambdaRotation.y));
		
		// Compute the pseudo velocity of body 2
		w2 = this.i2.multiplyNew(angularImpulseBody2);
		
		// Update the body position/orientation of body 2
		q2.add((new Quaternion(0, w2)).multiply(q2).multiply(0.5f));
		q2.normalize();
		
		// --------------- Limits Constraints --------------- //
		
		if (this.isLimitEnabled) {
			
			if (this.isLowerLimitViolated || this.isUpperLimitViolated) {
				
				// Compute the inverse of the mass matrix K=JM^-1J^t for the limits (1x1 matrix)
				this.inverseMassMatrixLimitMotor = this.mA1.dot(this.i1.multiplyNew(this.mA1)) + this.mA1.dot(this.i2.multiplyNew(this.mA1));
				this.inverseMassMatrixLimitMotor = (this.inverseMassMatrixLimitMotor > 0.0f) ? 1.0f / this.inverseMassMatrixLimitMotor : 0.0f;
			}
			
			// If the lower limit is violated
			if (this.isLowerLimitViolated) {
				
				// Compute the Lagrange multiplier lambda for the lower limit raint
				final float lambdaLowerLimit = this.inverseMassMatrixLimitMotor * (-lowerLimitError);
				
				// Compute the impulse P=J^T * lambda of body 1
				final Vector3f angularImpulseBody1_tmp = this.mA1.multiplyNew(lambdaLowerLimit);
				
				// Compute the pseudo velocity of body 1
				final Vector3f w1_tmp = this.i1.multiplyNew(angularImpulseBody1_tmp);
				
				// Update the body position/orientation of body 1
				q1.add((new Quaternion(0, w1_tmp)).multiply(q1).multiply(0.5f));
				q1.normalize();
				
				// Compute the impulse P=J^T * lambda of body 2
				final Vector3f angularImpulseBody2_tmp = this.mA1.multiplyNew(lambdaLowerLimit);
				
				// Compute the pseudo velocity of body 2
				final Vector3f w2_tmp = this.i2.multiplyNew(angularImpulseBody2_tmp);
				
				// Update the body position/orientation of body 2
				q2.add((new Quaternion(0, w2_tmp)).multiply(q2).multiply(0.5f));
				q2.normalize();
			}
			
			// If the upper limit is violated
			if (this.isUpperLimitViolated) {
				
				// Compute the Lagrange multiplier lambda for the upper limit raint
				final float lambdaUpperLimit = this.inverseMassMatrixLimitMotor * (-upperLimitError);
				
				// Compute the impulse P=J^T * lambda of body 1
				final Vector3f angularImpulseBody1_tmp = this.mA1.multiplyNew(lambdaUpperLimit);
				
				// Compute the pseudo velocity of body 1
				final Vector3f w1_tmp = this.i1.multiplyNew(angularImpulseBody1_tmp);
				
				// Update the body position/orientation of body 1
				q1.add((new Quaternion(0, w1_tmp)).multiply(q1).multiply(0.5f));
				q1.normalize();
				
				// Compute the impulse P=J^T * lambda of body 2
				final Vector3f angularImpulseBody2_tmp = this.mA1.multiplyNew(-lambdaUpperLimit);
				
				// Compute the pseudo velocity of body 2
				final Vector3f w2_tmp = this.i2.multiplyNew(angularImpulseBody2_tmp);
				
				// Update the body position/orientation of body 2
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
		
		// Compute J*v
		final Vector3f JvTranslation = v2.addNew(w2.cross(this.r2World)).less(v1).less(w1.cross(this.r1World));
		
		// Compute the Lagrange multiplier lambda
		final Vector3f deltaLambdaTranslation = this.inverseMassMatrixTranslation.multiplyNew(JvTranslation.multiplyNew(-1).less(this.bTranslation));
		this.impulseTranslation.add(deltaLambdaTranslation);
		
		// Compute the impulse P=J^T * lambda of body 1
		final Vector3f linearImpulseBody1 = deltaLambdaTranslation.multiplyNew(-1);
		Vector3f angularImpulseBody1 = deltaLambdaTranslation.cross(this.r1World);
		
		// Apply the impulse to the body 1
		v1.add(linearImpulseBody1.multiplyNew(inverseMassBody1));
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda of body 2
		Vector3f angularImpulseBody2 = deltaLambdaTranslation.cross(this.r2World).multiplyNew(-1);
		
		// Apply the impulse to the body 2
		v2.add(deltaLambdaTranslation.multiplyNew(inverseMassBody2));
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
		
		// --------------- Rotation Constraints --------------- //
		
		// Compute J*v for the 2 rotation raints
		final Vector2f JvRotation = new Vector2f(-this.b2CrossA1.dot(w1) + this.b2CrossA1.dot(w2), -this.c2CrossA1.dot(w1) + this.c2CrossA1.dot(w2));
		
		// Compute the Lagrange multiplier lambda for the 2 rotation raints
		final Vector2f deltaLambdaRotation = this.inverseMassMatrixRotation.multiplyNew(JvRotation.multiplyNew(-1).less(this.bRotation));
		this.impulseRotation.add(deltaLambdaRotation);
		
		// Compute the impulse P=J^T * lambda for the 2 rotation raints of body 1
		angularImpulseBody1 = this.b2CrossA1.multiplyNew(-1).multiply(deltaLambdaRotation.x).less(this.c2CrossA1.multiplyNew(deltaLambdaRotation.y));
		
		// Apply the impulse to the body 1
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda for the 2 rotation raints of body 2
		angularImpulseBody2 = this.b2CrossA1.multiplyNew(deltaLambdaRotation.x).add(this.c2CrossA1.multiplyNew(deltaLambdaRotation.y));
		
		// Apply the impulse to the body 2
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
		
		// --------------- Limits Constraints --------------- //
		
		if (this.isLimitEnabled) {
			
			// If the lower limit is violated
			if (this.isLowerLimitViolated) {
				
				// Compute J*v for the lower limit raint
				final float JvLowerLimit = w2.lessNew(w1).dot(this.mA1);
				
				// Compute the Lagrange multiplier lambda for the lower limit raint
				float deltaLambdaLower = this.inverseMassMatrixLimitMotor * (-JvLowerLimit - this.bLowerLimit);
				final float lambdaTemp = this.impulseLowerLimit;
				this.impulseLowerLimit = FMath.max(this.impulseLowerLimit + deltaLambdaLower, 0.0f);
				deltaLambdaLower = this.impulseLowerLimit - lambdaTemp;
				
				// Compute the impulse P=J^T * lambda for the lower limit raint of body 1
				final Vector3f angularImpulseBody1_tmp = this.mA1.multiplyNew(-deltaLambdaLower);
				
				// Apply the impulse to the body 1
				w1.add(this.i1.multiplyNew(angularImpulseBody1_tmp));
				
				// Compute the impulse P=J^T * lambda for the lower limit raint of body 2
				final Vector3f angularImpulseBody2_tmp = this.mA1.multiplyNew(deltaLambdaLower);
				
				// Apply the impulse to the body 2
				w2.add(this.i2.multiplyNew(angularImpulseBody2_tmp));
			}
			
			// If the upper limit is violated
			if (this.isUpperLimitViolated) {
				
				// Compute J*v for the upper limit raint
				final float JvUpperLimit = w2.lessNew(w1).multiplyNew(-1).dot(this.mA1);
				
				// Compute the Lagrange multiplier lambda for the upper limit raint
				float deltaLambdaUpper = this.inverseMassMatrixLimitMotor * (-JvUpperLimit - this.bUpperLimit);
				final float lambdaTemp = this.impulseUpperLimit;
				this.impulseUpperLimit = FMath.max(this.impulseUpperLimit + deltaLambdaUpper, 0.0f);
				deltaLambdaUpper = this.impulseUpperLimit - lambdaTemp;
				
				// Compute the impulse P=J^T * lambda for the upper limit raint of body 1
				final Vector3f angularImpulseBody1_tmp = this.mA1.multiplyNew(deltaLambdaUpper);
				
				// Apply the impulse to the body 1
				w1.add(this.i1.multiplyNew(angularImpulseBody1_tmp));
				
				// Compute the impulse P=J^T * lambda for the upper limit raint of body 2
				final Vector3f angularImpulseBody2_tmp = this.mA1.multiplyNew(-deltaLambdaUpper);
				
				// Apply the impulse to the body 2
				w2.add(this.i2.multiplyNew(angularImpulseBody2_tmp));
			}
		}
		
		// --------------- Motor --------------- //
		
		// If the motor is enabled
		if (this.isMotorEnabled) {
			
			// Compute J*v for the motor
			final float JvMotor = this.mA1.dot(w1.lessNew(w2));
			
			// Compute the Lagrange multiplier lambda for the motor
			final float maxMotorImpulse = this.maxMotorTorque * raintSolverData.timeStep;
			float deltaLambdaMotor = this.inverseMassMatrixLimitMotor * (-JvMotor - this.motorSpeed);
			final float lambdaTemp = this.impulseMotor;
			this.impulseMotor = FMath.clamp(this.impulseMotor + deltaLambdaMotor, -maxMotorImpulse, maxMotorImpulse);
			deltaLambdaMotor = this.impulseMotor - lambdaTemp;
			
			// Compute the impulse P=J^T * lambda for the motor of body 1
			final Vector3f angularImpulseBody1_tmp = this.mA1.multiplyNew(-deltaLambdaMotor);
			
			// Apply the impulse to the body 1
			w1.add(this.i1.multiplyNew(angularImpulseBody1_tmp));
			
			// Compute the impulse P=J^T * lambda for the motor of body 2
			final Vector3f angularImpulseBody2_tmp = this.mA1.multiplyNew(deltaLambdaMotor);
			
			// Apply the impulse to the body 2
			w2.add(this.i2.multiplyNew(angularImpulseBody2_tmp));
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
		
		// Compute the impulse P=J^T * lambda for the 2 rotation raints
		final Vector3f rotationImpulse = this.b2CrossA1.multiplyNew(-1).multiply(this.impulseRotation.x).less(this.c2CrossA1.multiplyNew(this.impulseRotation.y));
		
		// Compute the impulse P=J^T * lambda for the lower and upper limits raints
		final Vector3f limitsImpulse = this.mA1.multiplyNew(this.impulseUpperLimit - this.impulseLowerLimit);
		
		// Compute the impulse P=J^T * lambda for the motor raint
		final Vector3f motorImpulse = this.mA1.multiplyNew(-this.impulseMotor);
		
		// Compute the impulse P=J^T * lambda for the 3 translation raints of body 1
		final Vector3f linearImpulseBody1 = this.impulseTranslation.multiplyNew(-1);
		final Vector3f angularImpulseBody1 = this.impulseTranslation.cross(this.r1World);
		
		// Compute the impulse P=J^T * lambda for the 2 rotation raints of body 1
		angularImpulseBody1.add(rotationImpulse);
		
		// Compute the impulse P=J^T * lambda for the lower and upper limits raints of body 1
		angularImpulseBody1.add(limitsImpulse);
		
		// Compute the impulse P=J^T * lambda for the motor raint of body 1
		angularImpulseBody1.add(motorImpulse);
		
		// Apply the impulse to the body 1
		v1.add(linearImpulseBody1.multiplyNew(inverseMassBody1));
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda for the 3 translation raints of body 2
		final Vector3f angularImpulseBody2 = this.impulseTranslation.cross(this.r2World).multiplyNew(-1);
		
		// Compute the impulse P=J^T * lambda for the 2 rotation raints of body 2
		angularImpulseBody2.add(rotationImpulse.multiplyNew(-1));
		
		// Compute the impulse P=J^T * lambda for the lower and upper limits raints of body 2
		angularImpulseBody2.add(limitsImpulse.multiplyNew(-1));
		
		// Compute the impulse P=J^T * lambda for the motor raint of body 2
		angularImpulseBody2.add(motorImpulse.multiplyNew(-1));
		
		// Apply the impulse to the body 2
		v2.add(this.impulseTranslation.multiplyNew(inverseMassBody2));
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
	}
	
}
