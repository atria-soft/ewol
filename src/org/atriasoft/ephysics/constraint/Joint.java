package org.atriasoft.ephysics.constraint;

import org.atriasoft.ephysics.body.RigidBody;
import org.atriasoft.ephysics.engine.ConstraintSolverData;

/**
 *  It represents a joint between two bodies.
 */
public abstract class Joint {
	
	protected RigidBody body1; //!< Pointer to the first body of the joint
	protected RigidBody body2; //!< Pointer to the second body of the joint
	protected JointType type; //!< Type of the joint
	protected int indexBody1; //!< Body 1 index in the velocity array to solve the raint
	protected int indexBody2; //!< Body 2 index in the velocity array to solve the raint
	protected JointsPositionCorrectionTechnique positionCorrectionTechnique; //!< Position correction technique used for the raint (used for joints)
	protected boolean isCollisionEnabled; //!< True if the two bodies of the raint are allowed to collide with each other
	public boolean isAlreadyInIsland; //!< True if the joint has already been added into an island
	
	/// Constructor
	public Joint(final JointInfo jointInfo) {
		this.body1 = jointInfo.body1;
		this.body2 = jointInfo.body2;
		this.type = jointInfo.type;
		this.positionCorrectionTechnique = jointInfo.positionCorrectionTechnique;
		this.isCollisionEnabled = jointInfo.isCollisionEnabled;
		this.isAlreadyInIsland = false;
		assert (this.body1 != null);
		assert (this.body2 != null);
	}
	
	/// Return the reference to the body 1
	public RigidBody getBody1() {
		return this.body1;
	}
	
	/// Return the reference to the body 2
	public RigidBody getBody2() {
		return this.body2;
	}
	
	/** Return the type of the raint
	* @return The type of the joint
	*/
	public JointType getType() {
		return this.type;
	}
	
	/// Initialize before solving the joint
	public abstract void initBeforeSolve(final ConstraintSolverData raintSolverData);
	
	/** Return true if the joint is active
	* @return True if the joint is active
	*/
	public boolean isActive() {
		return (this.body1.isActive() && this.body2.isActive());
	}
	
	/// Return true if the joint has already been added into an island
	public boolean isAlreadyInIsland() {
		return this.isAlreadyInIsland;
	}
	
	/**Return true if the collision between the two bodies of the joint is enabled
	 * @return True if the collision is enabled between the two bodies of the joint
	 *			  is enabled and false otherwise
	 */
	public boolean isCollisionEnabled() {
		return this.isCollisionEnabled;
	}
	
	/// Solve the position raint
	public abstract void solvePositionConstraint(final ConstraintSolverData raintSolverData);
	
	/// Solve the velocity raint
	public abstract void solveVelocityConstraint(final ConstraintSolverData raintSolverData);
	
	/// Warm start the joint (apply the previous impulse at the beginning of the step)
	public abstract void warmstart(final ConstraintSolverData raintSolverData);
}
