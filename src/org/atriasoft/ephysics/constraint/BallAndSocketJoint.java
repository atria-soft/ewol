package org.atriasoft.ephysics.constraint;

import org.atriasoft.etk.math.Matrix3f;
import org.atriasoft.etk.math.Quaternion;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.BodyType;
import org.atriasoft.ephysics.engine.ConstraintSolverData;

public class BallAndSocketJoint extends Joint {
	
	private static float BETA = 0.2f; //!< Beta value for the bias factor of position correction
	private Vector3f localAnchorPointBody1 = new Vector3f(0, 0, 0); //!< Anchor point of body 1 (in local-space coordinates of body 1)
	private Vector3f localAnchorPointBody2 = new Vector3f(0, 0, 0); //!< Anchor point of body 2 (in local-space coordinates of body 2)
	private Vector3f r1World = new Vector3f(0, 0, 0); //!< Vector from center of body 2 to anchor point in world-space
	private Vector3f r2World = new Vector3f(0, 0, 0); //!< Vector from center of body 2 to anchor point in world-space
	private Matrix3f i1 = new Matrix3f(); //!< Inertia tensor of body 1 (in world-space coordinates)
	private Matrix3f i2 = new Matrix3f(); //!< Inertia tensor of body 2 (in world-space coordinates)
	private Vector3f biasVector = new Vector3f(0, 0, 0); //!< Bias vector for the raint
	private Matrix3f inverseMassMatrix = new Matrix3f(); //!< Inverse mass matrix K=JM^-1J^-t of the raint
	private final Vector3f impulse = new Vector3f(0, 0, 0); //!< Accumulated impulse
	/// Constructor
	
	public BallAndSocketJoint(final BallAndSocketJointInfo jointInfo) {
		super(jointInfo);
		// Compute the local-space anchor point for each body
		this.localAnchorPointBody1 = this.body1.getTransform().inverseNew().multiply(jointInfo.anchorPointWorldSpace);
		this.localAnchorPointBody2 = this.body2.getTransform().inverseNew().multiply(jointInfo.anchorPointWorldSpace);
	}
	
	@Override
	public void initBeforeSolve(final ConstraintSolverData raintSolverData) {
		
		// Initialize the bodies index in the velocity array
		this.indexBody1 = raintSolverData.mapBodyToConstrainedVelocityIndex.get(this.body1);
		this.indexBody2 = raintSolverData.mapBodyToConstrainedVelocityIndex.get(this.body2);
		
		// Get the bodies center of mass and orientations
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
		
		// Compute the matrix K=JM^-1J^t (3x3 matrix)
		final float inverseMassBodies = this.body1.massInverse + this.body2.massInverse;
		final Matrix3f massMatrix = new Matrix3f(inverseMassBodies, 0, 0, 0, inverseMassBodies, 0, 0, 0, inverseMassBodies);
		massMatrix.add(skewSymmetricMatrixU1.multiplyNew(this.i1).multiply(skewSymmetricMatrixU1.transposeNew()));
		massMatrix.add(skewSymmetricMatrixU2.multiplyNew(this.i2).multiply(skewSymmetricMatrixU2.transposeNew()));
		
		// Compute the inverse mass matrix K^-1
		this.inverseMassMatrix.setZero();
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrix = massMatrix.inverseNew();
		}
		
		// Compute the bias "b" of the raint
		this.biasVector.setZero();
		if (this.positionCorrectionTechnique == JointsPositionCorrectionTechnique.BAUMGARTE_JOINTS) {
			final float biasFactor = (BETA / raintSolverData.timeStep);
			this.biasVector = (x2.addNew(this.r2World).less(x1).less(this.r1World)).multiply(biasFactor);
		}
		
		// If warm-starting is not enabled
		if (!raintSolverData.isWarmStartingActive) {
			
			// Reset the accumulated impulse
			this.impulse.setZero();
		}
	}
	
	@Override
	public void solvePositionConstraint(final ConstraintSolverData raintSolverData) {
		
		// Get the velocities
		final Vector3f v1 = raintSolverData.linearVelocities[this.indexBody1];
		final Vector3f v2 = raintSolverData.linearVelocities[this.indexBody2];
		final Vector3f w1 = raintSolverData.angularVelocities[this.indexBody1];
		final Vector3f w2 = raintSolverData.angularVelocities[this.indexBody2];
		
		// Compute J*v
		final Vector3f Jv = v2.addNew(w2.cross(this.r2World)).less(v1).less(w1.cross(this.r1World));
		
		// Compute the Lagrange multiplier lambda
		final Vector3f deltaLambda = this.inverseMassMatrix.multiplyNew(Jv.multiplyNew(-1).less(this.biasVector));
		this.impulse.add(deltaLambda);
		
		// Compute the impulse P=J^T * lambda for the body 1
		final Vector3f linearImpulseBody1 = deltaLambda.multiplyNew(-1);
		final Vector3f angularImpulseBody1 = deltaLambda.cross(this.r1World);
		
		// Apply the impulse to the body 1
		v1.add(linearImpulseBody1.multiplyNew(this.body1.massInverse));
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda for the body 2
		final Vector3f angularImpulseBody2 = deltaLambda.cross(this.r2World).multiplyNew(-1);
		
		// Apply the impulse to the body 2
		v2.add(deltaLambda.multiplyNew(this.body2.massInverse));
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
	}
	
	@Override
	public void solveVelocityConstraint(final ConstraintSolverData raintSolverData) {
		
		// If the error position correction technique is not the non-linear-gauss-seidel, we do
		// do not execute this method
		if (this.positionCorrectionTechnique != JointsPositionCorrectionTechnique.NON_LINEAR_GAUSS_SEIDEL) {
			return;
		}
		
		// Get the bodies center of mass and orientations
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
		
		// Recompute the inverse mass matrix K=J^TM^-1J of of the 3 translation raints
		final float inverseMassBodies = inverseMassBody1 + inverseMassBody2;
		final Matrix3f massMatrix = new Matrix3f(inverseMassBodies, 0, 0, 0, inverseMassBodies, 0, 0, 0, inverseMassBodies);
		massMatrix.add(skewSymmetricMatrixU1.multiplyNew(this.i1).multiplyNew(skewSymmetricMatrixU1.transposeNew()));
		massMatrix.add(skewSymmetricMatrixU2.multiplyNew(this.i2).multiplyNew(skewSymmetricMatrixU2.transposeNew()));
		this.inverseMassMatrix.setZero();
		if (this.body1.getType() == BodyType.DYNAMIC || this.body2.getType() == BodyType.DYNAMIC) {
			this.inverseMassMatrix = massMatrix.inverseNew();
		}
		
		// Compute the raint error (value of the C(x) function)
		final Vector3f raintError = x2.addNew(this.r2World).less(x1).less(this.r1World);
		
		// Compute the Lagrange multiplier lambda
		// TODO : Do not solve the system by computing the inverse each time and multiplying with the
		//		right-hand side vector but instead use a method to directly solve the linear system.
		final Vector3f lambda = this.inverseMassMatrix.multiplyNew(raintError.multiplyNew(-1));
		
		// Compute the impulse of body 1
		final Vector3f linearImpulseBody1 = lambda.multiplyNew(-1);
		final Vector3f angularImpulseBody1 = lambda.cross(this.r1World);
		
		// Compute the pseudo velocity of body 1
		final Vector3f v1 = linearImpulseBody1.multiplyNew(inverseMassBody1);
		final Vector3f w1 = this.i1.multiplyNew(angularImpulseBody1);
		
		// Update the body center of mass and orientation of body 1
		x1.add(v1);
		q1.add((new Quaternion(0, w1)).multiplyNew(q1).multiplyNew(0.5f));
		q1.normalize();
		
		// Compute the impulse of body 2
		final Vector3f angularImpulseBody2 = lambda.cross(this.r2World).multiplyNew(-1);
		
		// Compute the pseudo velocity of body 2
		final Vector3f v2 = lambda.multiplyNew(inverseMassBody2);
		final Vector3f w2 = this.i2.multiplyNew(angularImpulseBody2);
		
		// Update the body position/orientation of body 2
		x2.add(v2);
		q2.add((new Quaternion(0, w2)).multiplyNew(q2).multiply(0.5f));
		q2.normalize();
	}
	
	@Override
	public void warmstart(final ConstraintSolverData raintSolverData) {
		
		// Get the velocities
		final Vector3f v1 = raintSolverData.linearVelocities[this.indexBody1];
		final Vector3f v2 = raintSolverData.linearVelocities[this.indexBody2];
		final Vector3f w1 = raintSolverData.angularVelocities[this.indexBody1];
		final Vector3f w2 = raintSolverData.angularVelocities[this.indexBody2];
		
		// Compute the impulse P=J^T * lambda for the body 1
		final Vector3f linearImpulseBody1 = this.impulse.multiplyNew(-1);
		final Vector3f angularImpulseBody1 = this.impulse.cross(this.r1World);
		
		// Apply the impulse to the body 1
		v1.add(linearImpulseBody1.multiplyNew(this.body1.massInverse));
		w1.add(this.i1.multiplyNew(angularImpulseBody1));
		
		// Compute the impulse P=J^T * lambda for the body 2
		final Vector3f angularImpulseBody2 = this.impulse.cross(this.r2World).multiplyNew(-1);
		
		// Apply the impulse to the body to the body 2
		v2.add(this.impulse.multiplyNew(this.body2.massInverse));
		w2.add(this.i2.multiplyNew(angularImpulseBody2));
	}
	
}
