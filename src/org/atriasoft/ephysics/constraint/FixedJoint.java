package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Quaternion;
import org.atriasoft.etk.math.Transform3D;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.BodyType;
import org.atriasoft.ephysics.engine.ConstraintSolverData;

public class FixedJoint extends Joint {
	
	protected static float BETA = 0.2f; //!< Beta value for the bias factor of position correction
	protected Vector3f localAnchorPointBody1 = new Vector3f(0, 0, 0); //!< Anchor point of body 1 (in local-space coordinates of body 1)
	protected Vector3f localAnchorPointBody2 = new Vector3f(0, 0, 0); //!< Anchor point of body 2 (in local-space coordinates of body 2)
	protected Vector3f r1World = new Vector3f(0, 0, 0); //!< Vector from center of body 2 to anchor point in world-space
	protected Vector3f r2World = new Vector3f(0, 0, 0); //!< Vector from center of body 2 to anchor point in world-space
	protected Matrix3f i1 = new Matrix3f(); //!< Inertia tensor of body 1 (in world-space coordinates)
	protected Matrix3f i2 = new Matrix3f(); //!< Inertia tensor of body 2 (in world-space coordinates)
	protected Vector3f impulseTranslation = new Vector3f(0, 0, 0); //!< Accumulated impulse for the 3 translation raints
	protected Vector3f impulseRotation = new Vector3f(0, 0, 0); //!< Accumulate impulse for the 3 rotation raints
	protected Matrix3f inverseMassMatrixTranslation = new Matrix3f(); //!< Inverse mass matrix K=JM^-1J^-t of the 3 translation raints (3x3 matrix)
	protected Matrix3f inverseMassMatrixRotation = new Matrix3f(); //!< Inverse mass matrix K=JM^-1J^-t of the 3 rotation raints (3x3 matrix)
	protected Vector3f biasTranslation = new Vector3f(0, 0, 0); //!< Bias vector for the 3 translation raints
	protected Vector3f biasRotation = new Vector3f(0, 0, 0); //!< Bias vector for the 3 rotation raints
	protected Quaternion initOrientationDifferenceInv = new Quaternion(); //!< Inverse of the initial orientation difference between the two bodies
	
	/// Constructor
	public FixedJoint(final FixedJointInfo jointInfo) {
		super(jointInfo);
		// Compute the local-space anchor point for each body
		final Transform3D transform1 = this.body1.getTransform();
		final Transform3D transform2 = this.body2.getTransform();
		this.localAnchorPointBody1 = transform1.inverseNew().multiply(jointInfo.anchorPointWorldSpace);
		this.localAnchorPointBody2 = transform2.inverseNew().multiply(jointInfo.anchorPointWorldSpace);
		
		// Compute the inverse of the initial orientation difference between the two bodies
		this.initOrientationDifferenceInv = transform2.getOrientation().multiplyNew(transform1.getOrientation().inverseNew());
		this.initOrientationDifferenceInv.normalize();
		this.initOrientationDifferenceInv.inverse();
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
		
		// Compute the corresponding skew-symmetric matrices
		final Matrix3f skewSymmetricMatrixU1 = Matrix3f.computeSkewSymmetricMatrixForCrossProduct(this.r1World);
		final Matrix3f skewSymmetricMatrixU2 = Matrix3f.computeSkewSymmetricMatrixForCrossProduct(this.r2World);
		
		// Compute the matrix K=JM^-1J^t (3x3 matrix) for the 3 translation raints
		final float inverseMassBodies = this.body1.massInverse + this.body2.massInverse;
		final Matrix3f massMatrix = new Matrix3f(inverseMassBodies, 0, 0, 0, inverseMassBodies, 0, 0, 0, inverseMassBodies);
		massMatrix.add(skewSymmetricMatrixU1.multiplyNew(this.i1).multiply(skewSymmetricMatrixU1.transposeNew()));
		massMatrix.add(skewSymmetricMatrixU2.multiplyNew(this.i2).multiply(skewSymmetricMatrixU2.transposeNew()));
		
		// Compute the inverse mass matrix K^-1 for the 3 translation raints
		this.inverseMassMatrixTranslation.setZero();
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixTranslation = massMatrix.inverseNew();
		}
		
		// Compute the bias "b" of the raint for the 3 translation raints
		final float biasFactor = (BETA / raintSolverData.timeStep);
		this.biasTranslation.setZero();
		if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
			this.biasTranslation = x2.addNew(this.r2World).less(x1).less(this.r1World).multiply(biasFactor);
		}
		
		// Compute the inverse of the mass matrix K=JM^-1J^t for the 3 rotation
		// contraints (3x3 matrix)
		this.inverseMassMatrixRotation = this.i1.addNew(this.i2);
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixRotation = this.inverseMassMatrixRotation.inverseNew();
		}
		
		// Compute the bias "b" for the 3 rotation raints
		this.biasRotation.setZero();
		if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
			final Quaternion currentOrientationDifference = orientationBody2.multiplyNew(orientationBody1.inverseNew());
			currentOrientationDifference.normalize();
			final Quaternion qError = currentOrientationDifference.multiplyNew(this.initOrientationDifferenceInv);
			this.biasRotation = qError.getVectorV().multiply(biasFactor * 2.0f);
		}
		
		// If warm-starting is not enabled
		if (!raintSolverData.isWarmStartingActive) {
			// Reset the accumulated impulses
			this.impulseTranslation.setZero();
			this.impulseRotation.setZero();
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
		
		// Compute the corresponding skew-symmetric matrices
		final Matrix3f skewSymmetricMatrixU1 = Matrix3f.computeSkewSymmetricMatrixForCrossProduct(this.r1World);
		final Matrix3f skewSymmetricMatrixU2 = Matrix3f.computeSkewSymmetricMatrixForCrossProduct(this.r2World);
		
		// --------------- Translation Constraints --------------- //
		
		// Compute the matrix K=JM^-1J^t (3x3 matrix) for the 3 translation raints
		final float inverseMassBodies = this.body1.massInverse + this.body2.massInverse;
		final Matrix3f massMatrix = new Matrix3f(inverseMassBodies, 0, 0, 0, inverseMassBodies, 0, 0, 0, inverseMassBodies);
		massMatrix.add(skewSymmetricMatrixU1.multiplyNew(this.i1).add(skewSymmetricMatrixU1.transposeNew()));
		massMatrix.add(skewSymmetricMatrixU2.multiplyNew(this.i2).add(skewSymmetricMatrixU2.transposeNew()));
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
		final Vector3f angularImpulseBody2 = lambdaTranslation.cross(this.r2World).multiply(-1);
		
		// Compute the pseudo velocity of body 2
		final Vector3f v2 = lambdaTranslation.multiplyNew(inverseMassBody2);
		Vector3f w2 = this.i2.multiplyNew(angularImpulseBody2);
		
		// Update the body position/orientation of body 2
		x2.add(v2);
		q2.add((new Quaternion(0, w2)).multiply(q2).multiply(0.5f));
		q2.normalize();
		
		// --------------- Rotation Constraints --------------- //
		
		// Compute the inverse of the mass matrix K=JM^-1J^t for the 3 rotation
		// contraints (3x3 matrix)
		this.inverseMassMatrixRotation = this.i1.addNew(this.i2);
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrixRotation = this.inverseMassMatrixRotation.inverseNew();
		}
		
		// Compute the position error for the 3 rotation raints
		final Quaternion currentOrientationDifference = q2.multiplyNew(q1.inverseNew());
		currentOrientationDifference.normalize();
		final Quaternion qError = currentOrientationDifference.multiplyNew(this.initOrientationDifferenceInv);
		final Vector3f errorRotation = qError.getVectorV().multiply(2.0f);
		
		// Compute the Lagrange multiplier lambda for the 3 rotation raints
		final Vector3f lambdaRotation = this.inverseMassMatrixRotation.multiplyNew(errorRotation.multiplyNew(-1));
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints of body 1
		angularImpulseBody1 = lambdaRotation.multiplyNew(-1);
		
		// Compute the pseudo velocity of body 1
		w1 = this.i1.multiplyNew(angularImpulseBody1);
		
		// Update the body position/orientation of body 1
		q1.add((new Quaternion(0, w1)).multiply(q1).multiply(0.5f));
		q1.normalize();
		
		// Compute the pseudo velocity of body 2
		w2 = this.i2.multiplyNew(lambdaRotation);
		
		// Update the body position/orientation of body 2
		q2.add((new Quaternion(0, w2)).multiply(q2).multiply(0.5f));
		q2.normalize();
	}
	
	@Override
	public void solveVelocityConstraint(final ConstraintSolverData raintSolverData) {
		
		// Get the velocities
		final Vector3f v1 = raintSolverData.linearVelocities[this.indexBody1];
		final Vector3f v2 = raintSolverData.linearVelocities[this.indexBody2];
		final Vector3f w1 = raintSolverData.angularVelocities[this.indexBody1];
		final Vector3f w2 = raintSolverData.angularVelocities[this.indexBody2];
		
		// Get the inverse mass of the bodies
		final float inverseMassBody1 = this.body1.massInverse;
		final float inverseMassBody2 = this.body2.massInverse;
		
		// --------------- Translation Constraints --------------- //
		
		// Compute J*v for the 3 translation raints
		final Vector3f JvTranslation = v2.addNew(w2.cross(this.r2World)).less(v1).less(w1.cross(this.r1World));
		
		// Compute the Lagrange multiplier lambda
		final Vector3f deltaLambda = this.inverseMassMatrixTranslation.multiplyNew(JvTranslation.multiplyNew(-1).less(this.biasTranslation));
		this.impulseTranslation.add(deltaLambda);
		
		// Compute the impulse P=J^T * lambda for body 1
		final Vector3f linearImpulseBody1 = deltaLambda.multiplyNew(-1);
		Vector3f angularImpulseBody1 = deltaLambda.cross(this.r1World);
		
		// Apply the impulse to the body 1
		v1.add(linearImpulseBody1.multiplyNew(inverseMassBody1));
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda  for body 2
		final Vector3f angularImpulseBody2 = deltaLambda.cross(this.r2World).multiply(-1);
		
		// Apply the impulse to the body 2
		v2.add(deltaLambda.multiplyNew(inverseMassBody2));
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
		
		// --------------- Rotation Constraints --------------- //
		
		// Compute J*v for the 3 rotation raints
		final Vector3f JvRotation = w2.lessNew(w1);
		
		// Compute the Lagrange multiplier lambda for the 3 rotation raints
		final Vector3f deltaLambda2 = this.inverseMassMatrixRotation.multiplyNew(JvRotation.multiplyNew(-1).less(this.biasRotation));
		this.impulseRotation.add(deltaLambda2);
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints for body 1
		angularImpulseBody1 = deltaLambda2.multiplyNew(-1);
		
		// Apply the impulse to the body 1
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Apply the impulse to the body 2
		w2.add(this.i2.multiplyNew(deltaLambda2));
	}
	
	@Override
	public void warmstart(final ConstraintSolverData raintSolverData) {
		
		// Get the velocities
		final Vector3f v1 = raintSolverData.linearVelocities[this.indexBody1];
		final Vector3f v2 = raintSolverData.linearVelocities[this.indexBody2];
		final Vector3f w1 = raintSolverData.angularVelocities[this.indexBody1];
		final Vector3f w2 = raintSolverData.angularVelocities[this.indexBody2];
		
		// Get the inverse mass of the bodies
		final float inverseMassBody1 = this.body1.massInverse;
		final float inverseMassBody2 = this.body2.massInverse;
		
		// Compute the impulse P=J^T * lambda for the 3 translation raints for body 1
		final Vector3f linearImpulseBody1 = this.impulseTranslation.multiplyNew(-1);
		final Vector3f angularImpulseBody1 = this.impulseTranslation.cross(this.r1World);
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints for body 1
		angularImpulseBody1.add(this.impulseRotation.multiplyNew(-1));
		
		// Apply the impulse to the body 1
		v1.add(linearImpulseBody1.multiplyNew(inverseMassBody1));
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda for the 3 translation raints for body 2
		final Vector3f angularImpulseBody2 = this.impulseTranslation.cross(this.r2World).multiplyNew(-1);
		
		// Compute the impulse P=J^T * lambda for the 3 rotation raints for body 2
		angularImpulseBody2.add(this.impulseRotation);
		
		// Apply the impulse to the body 2
		v2.add(this.impulseTranslation.multiplyNew(inverseMassBody2));
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
		
	}
	
}
