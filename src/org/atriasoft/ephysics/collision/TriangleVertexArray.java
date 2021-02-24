package org.atriasoft.ephysics.collision;

import java.util.List;

import org.atriasoft.etk.math.Vector3f;

/**
 * This class is used to describe the vertices and faces of a triangular mesh.
 * A TriangleVertexArray represents a continuous array of vertices and indexes
 * of a triangular mesh. When you create a TriangleVertexArray, no data is copied
 * longo the array. It only stores pointer to the data. The purpose is to allow
 * the user to share vertices data between the physics engine and the rendering
 * part. Therefore, make sure that the data pointed by a TriangleVertexArray
 * remains valid during the TriangleVertexArray life.
 */
public class TriangleVertexArray {
	/// Vertice list
	protected final Vector3f[] vertices;
	/// List of triangle (3 pos for each triangle)
	protected final int[] triangles;
	
	public TriangleVertexArray(final List<Vector3f> _vertices, final List<Integer> _triangles) {
		this.vertices = _vertices.toArray(new Vector3f[0]);
		
		this.triangles = new int[_triangles.size()];
		for (int iii = 0; iii < this.triangles.length; iii++) {
			this.triangles[iii] = _triangles.get(iii);
		}
	}
	
	/**
	 * Constructor
	 * @param _vertices List Of all vertices
	 * @param _triangles List of all linked points
	 */
	public TriangleVertexArray(final Vector3f[] _vertices, final int[] _triangles) {
		this.vertices = _vertices;
		this.triangles = _triangles;
	}
	
	/**
	* Get The table of the triangle indice
	* @return reference on the triangle indice
	*/
	public int[] getIndices() {
		return this.triangles;
	}
	
	/**
	 * Get the number of triangle
	 * @return Number of triangles
	 */
	public int getNbTriangles() {
		return this.triangles.length / 3;
	}
	
	/**
	 * Get the number of vertices
	 * @return Number of vertices
	 */
	public int getNbVertices() {
		return this.vertices.length;
	}
	
	/**
	 * Get a triangle at the specific ID
	 * @return Buffer of 3 points
	 */
	public Triangle getTriangle(final int _id) {
		final Triangle out = new Triangle();
		out.value[0] = this.vertices[this.triangles[_id * 3]];
		out.value[1] = this.vertices[this.triangles[_id * 3 + 1]];
		out.value[2] = this.vertices[this.triangles[_id * 3 + 2]];
		return out;
	}
	
	/**
	* Get The table of the vertices
	* @return reference on the vertices
	*/
	public Vector3f[] getVertices() {
		return this.vertices;
	}
};