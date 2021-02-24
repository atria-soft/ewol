package org.atriasoft.ephysics.engine;

import java.util.Map;

import org.atriasoft.etk.math.Quaternion;
import org.atriasoft.etk.math.Vector3f;

import org.atriasoft.ephysics.body.RigidBody;

/**
 * This structure contains data from the raint solver that are used to solve
 * each joint raint.
 */
public class ConstraintSolverData {
	
	public float timeStep; //!< Current time step of the simulation
	public Vector3f[] linearVelocities = null; //!< Array with the bodies linear velocities
	public Vector3f[] angularVelocities = null; //!< Array with the bodies angular velocities
	public Vector3f[] positions = null; //!< Reference to the bodies positions
	public Quaternion[] orientations = null; //!< Reference to the bodies orientations
	public Map<RigidBody, Integer> mapBodyToConstrainedVelocityIndex; //!< Reference to the map that associates rigid body to their index in the rained velocities array
	public boolean isWarmStartingActive; //!< True if warm starting of the solver is active
	
	public ConstraintSolverData(final Map<RigidBody, Integer> refMapBodyToConstrainedVelocityIndex) {
		this.mapBodyToConstrainedVelocityIndex = refMapBodyToConstrainedVelocityIndex;
		
	}
}
