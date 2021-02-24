package org.atriasoft.ephysics;

import org.atriasoft.ephysics.collision.ProxyShape;
import org.atriasoft.ephysics.mathematics.Ray;

public class RaycastTest {
	
	public RaycastCallback userCallback; //!< User callback class
	/// Constructor
	
	public RaycastTest(final RaycastCallback _callback) {
		this.userCallback = _callback;
	}
	
	/// Ray cast test against a proxy shape
	public float raycastAgainstShape(final ProxyShape _shape, final Ray _ray) {
		// Ray casting test against the collision shape
		final RaycastInfo raycastInfo = new RaycastInfo();
		final boolean isHit = _shape.raycast(_ray, raycastInfo);
		// If the ray hit the collision shape
		if (isHit) {
			// Report the hit to the user and return the
			// user hit fraction value
			return this.userCallback.notifyRaycastHit(raycastInfo);
		}
		return _ray.maxFraction;
	}
}
