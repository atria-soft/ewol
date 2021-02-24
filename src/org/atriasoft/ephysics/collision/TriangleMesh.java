package org.atriasoft.ephysics.collision;

import java.util.ArrayList;
import java.util.List;

public class TriangleMesh {
	/// All the triangle arrays of the mesh (one triangle array per part)
	protected List<TriangleVertexArray> triangleArrays = new ArrayList<>();
	
	/**
	 *  Constructor
	 */
	public TriangleMesh() {}
	
	/**
	 *  Add a subpart of the mesh
	 */
	public void addSubpart(final TriangleVertexArray _triangleVertexArray) {
		this.triangleArrays.add(_triangleVertexArray);
	}
	
	/**
	 *  Get the number of subparts of the mesh
	 */
	public int getNbSubparts() {
		return this.triangleArrays.size();
	}
	
	/**
	 *  Get a pointer to a given subpart (triangle vertex array) of the mesh
	 */
	public TriangleVertexArray getSubpart(final int _indexSubpart) {
		assert (_indexSubpart < this.triangleArrays.size());
		return this.triangleArrays.get(_indexSubpart);
	}
}
