package org.atriasoft.ephysics.collision;

import org.atriasoft.etk.math.Vector3f;

public class Triangle {
	public final Vector3f[] value = new Vector3f[3];
	
	public Vector3f get(final int _id) {
		return this.value[_id];
	}
	
}
