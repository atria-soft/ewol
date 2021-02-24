package org.atriasoft.ephysics.collision.broadphase;

import org.atriasoft.ephysics.mathematics.Ray;

public interface CallbackRaycast {
	public float callback(DTree _node, Ray _ray);
}