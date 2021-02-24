package org.atriasoft.ephysics.constraint;

/// Position correction technique used in the raint solver (for joints).
/// BAUMGARTE_JOINTS : Faster but can be innacurate in some situations.
/// NON_LINEAR_GAUSS_SEIDEL : Slower but more precise. This is the option used by default.
public enum JointsPositionCorrectionTechnique {
	BAUMGARTE_JOINTS,
	NON_LINEAR_GAUSS_SEIDEL
}
