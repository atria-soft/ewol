package org.atriasoft.ephysics.engine;

import java.util.List;
import java.util.Map;

import org.atriasoft.etk.math.Quaternion;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.RigidBody;
import org.atriasoft.ephysics.constraint.Joint;

/**
 *  This class represents the raint solver that is used to solve raints between
 * the rigid bodies. The raint solver is based on the "Sequential Impulse" technique
 * described by Erin Catto in his GDC slides (http://code.google.com/p/box2d/downloads/list).
 *
 * A raint between two bodies is represented by a function C(x) which is equal to zero
 * when the raint is satisfied. The condition C(x)=0 describes a valid position and the
 * condition dC(x)/dt=0 describes a valid velocity. We have dC(x)/dt = Jv + b = 0 where J is
 * the Jacobian matrix of the raint, v is a vector that contains the velocity of both
 * bodies and b is the raint bias. We are looking for a force Fc that will act on the
 * bodies to keep the raint satisfied. Note that from the  work principle, we have
 * Fc = J^t * lambda where J^t is the transpose of the Jacobian matrix and lambda is a
 * Lagrange multiplier. Therefore, finding the force Fc is equivalent to finding the Lagrange
 * multiplier lambda.

 * An impulse P = F * dt where F is a force and dt is the timestep. We can apply impulses a
 * body to change its velocity. The idea of the Sequential Impulse technique is to apply
 * impulses to bodies of each raints in order to keep the raint satisfied.
 *
 * --- Step 1 ---
 *
 * First, we integrate the applied force Fa acting of each rigid body (like gravity, ...) and
 * we obtain some new velocities v2' that tends to violate the raints.
 *
 * v2' = v1 + dt * M^-1 * Fa
 *
 * where M is a matrix that contains mass and inertia tensor information.
 *
 * --- Step 2 ---
 *
 * During the second step, we iterate over all the raints for a certain number of
 * iterations and for each raint we compute the impulse to apply to the bodies needed
 * so that the new velocity of the bodies satisfy Jv + b = 0. From the Newton law, we know that
 * M * deltaV = Pc where M is the mass of the body, deltaV is the difference of velocity and
 * Pc is the raint impulse to apply to the body. Therefore, we have
 * v2 = v2' + M^-1 * Pc. For each raint, we can compute the Lagrange multiplier lambda
 * using : lambda = -this.c (Jv2' + b) where this.c = 1 / (J * M^-1 * J^t). Now that we have the
 * Lagrange multiplier lambda, we can compute the impulse Pc = J^t * lambda * dt to apply to
 * the bodies to satisfy the raint.
 *
 * --- Step 3 ---
 *
 * In the third step, we integrate the new position x2 of the bodies using the new velocities
 * v2 computed in the second step with : x2 = x1 + dt * v2.
 *
 * Note that in the following code (as it is also explained in the slides from Erin Catto),
 * the value lambda is not only the lagrange multiplier but is the multiplication of the
 * Lagrange multiplier with the timestep dt. Therefore, in the following code, when we use
 * lambda, we mean (lambda * dt).
 *
 * We are using the accumulated impulse technique that is also described in the slides from
 * Erin Catto.
 *
 * We are also using warm starting. The idea is to warm start the solver at the beginning of
 * each step by applying the last impulstes for the raints that we already existing at the
 * previous step. This allows the iterative solver to converge faster towards the solution.
 *
 * For contact raints, we are also using split impulses so that the position correction
 * that uses Baumgarte stabilization does not change the momentum of the bodies.
 *
 * There are two ways to apply the friction raints. Either the friction raints are
 * applied at each contact point or they are applied only at the center of the contact manifold
 * between two bodies. If we solve the friction raints at each contact point, we need
 * two raints (two tangential friction directions) and if we solve the friction
 * raints at the center of the contact manifold, we need two raints for tangential
 * friction but also another twist friction raint to prevent spin of the body around the
 * contact manifold center.
 */
public class ConstraintSolver {
	
	private final Map<RigidBody, Integer> mapBodyToConstrainedVelocityIndex; //!< Reference to the map that associates rigid body to their index in the rained velocities array
	private float timeStep; //!< Current time step
	private boolean isWarmStartingActive; //!< True if the warm starting of the solver is active
	private final ConstraintSolverData raintSolverData; //!< Constraint solver data used to initialize and solve the raints
	/// Constructor
	
	public ConstraintSolver(final Map<RigidBody, Integer> mapBodyToVelocityIndex) {
		this.mapBodyToConstrainedVelocityIndex = mapBodyToVelocityIndex;
		this.isWarmStartingActive = true;
		this.raintSolverData = new ConstraintSolverData(mapBodyToVelocityIndex);
		
	}
	
	/// Return true if the Non-Linear-Gauss-Seidel position correction technique is active
	public boolean getIsNonLinearGaussSeidelPositionCorrectionActive() {
		return this.isWarmStartingActive;
	}
	
	/// Initialize the raint solver for a given island
	public void initializeForIsland(final float dt, final Island island) {
		assert (island != null);
		assert (island.getNbBodies() > 0);
		assert (island.getNbJoints() > 0);
		// Set the current time step
		this.timeStep = dt;
		// Initialize the raint solver data used to initialize and solve the raints
		this.raintSolverData.timeStep = this.timeStep;
		this.raintSolverData.isWarmStartingActive = this.isWarmStartingActive;
		// For each joint of the island
		final List<Joint> joints = island.getJoints();
		for (int iii = 0; iii < island.getNbJoints(); ++iii) {
			// Initialize the raint before solving it
			joints.get(iii).initBeforeSolve(this.raintSolverData);
			// Warm-start the raint if warm-starting is enabled
			if (this.isWarmStartingActive) {
				joints.get(iii).warmstart(this.raintSolverData);
			}
		}
	}
	
	/// Set the rained positions/orientations arrays
	public void setConstrainedPositionsArrays(final Vector3f[] rainedPositions, final Quaternion[] rainedOrientations) {
		assert (rainedPositions != null);
		assert (rainedOrientations != null);
		this.raintSolverData.positions = rainedPositions;
		this.raintSolverData.orientations = rainedOrientations;
	}
	
	/// Set the rained velocities arrays
	public void setConstrainedVelocitiesArrays(final Vector3f[] rainedLinearVelocities, final Vector3f[] rainedAngularVelocities) {
		assert (rainedLinearVelocities != null);
		assert (rainedAngularVelocities != null);
		this.raintSolverData.linearVelocities = rainedLinearVelocities;
		this.raintSolverData.angularVelocities = rainedAngularVelocities;
	}
	
	/// Enable/Disable the Non-Linear-Gauss-Seidel position correction technique.
	public void setIsNonLinearGaussSeidelPositionCorrectionActive(final boolean isActive) {
		this.isWarmStartingActive = isActive;
	}
	
	/// Solve the position raints
	public void solvePositionConstraints(final Island island) {
		assert (island != null);
		assert (island.getNbJoints() > 0);
		final List<Joint> joints = island.getJoints();
		for (int iii = 0; iii < joints.size(); ++iii) {
			joints.get(iii).solvePositionConstraint(this.raintSolverData);
		}
	}
	
	/// Solve the raints
	public void solveVelocityConstraints(final Island island) {
		assert (island != null);
		assert (island.getNbJoints() > 0);
		// For each joint of the island
		final List<Joint> joints = island.getJoints();
		for (int iii = 0; iii < island.getNbJoints(); ++iii) {
			joints.get(iii).solveVelocityConstraint(this.raintSolverData);
		}
	}
}
