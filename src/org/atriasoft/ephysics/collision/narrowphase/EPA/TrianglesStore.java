package org.atriasoft.ephysics.collision.narrowphase.EPA;

import java.util.ArrayList;
import java.util.List;

import org.atriasoft.ephysics.internal.Log;
import org.atriasoft.etk.math.Vector3f;

public class TrianglesStore {
	private static int MAX_TRIANGLES = 200;
	
	public static void shrinkTo(final List list, final int newSize) {
		final int size = list.size();
		if (newSize >= size) {
			return;
		}
		for (int i = newSize; i < size; i++) {
			list.remove(list.size() - 1);
		}
	}
	
	private final List<TriangleEPA> triangles = new ArrayList<>();
	
	/// Clear all the storage
	public void clear() {
		this.triangles.clear();
	}
	
	/// Access operator
	public TriangleEPA get(final int _id) {
		return this.triangles.get(_id);
	}
	
	/// Return the number of triangles
	public int getNbTriangles() {
		return this.triangles.size();
	}
	
	/// Return the last triangle
	public TriangleEPA last() {
		return this.triangles.get(this.triangles.size() - 1);
	}
	
	/// Create a new triangle
	public TriangleEPA newTriangle(final Vector3f[] _vertices, final int _v0, final int _v1, final int _v2) {
		//Log.info("newTriangle: " + _v0 + ", " + _v1 + ", " + _v2);
		// If we have not reached the maximum number of triangles
		if (this.triangles.size() < MAX_TRIANGLES) {
			
			final TriangleEPA tmp = new TriangleEPA(_v0, _v1, _v2);
			if (!tmp.computeClosestPoint(_vertices)) {
				return null;
			}
			this.triangles.add(tmp);
			//Log.info(" ==> retrurn Triangle: " + tmp.get(0) + ", " + tmp.get(1) + ", " + tmp.get(2));
			return tmp;
		}
		// We are at the limit (internal)
		return null;
		
	}
	
	/// Set the number of triangles
	public void resize(final int _backup) {
		if (_backup > this.triangles.size()) {
			Log.error("RESIZE BIGGER : " + _backup + " > " + this.triangles.size());
		}
		shrinkTo(this.triangles, _backup);
	}
}
