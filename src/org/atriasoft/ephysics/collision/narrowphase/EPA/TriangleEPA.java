package org.atriasoft.ephysics.collision.narrowphase.EPA;

import org.atriasoft.etk.math.Vector3f;

/**
 *  Class TriangleEPA
 * This class represents a triangle face of the current polytope in the EPA algorithm.
 */
public class TriangleEPA {
	/// Make an half link of an edge with another one from another triangle. An half-link
	/// between an edge "edge0" and an edge "edge1" represents the fact that "edge1" is an
	/// adjacent edge of "edge0" but not the opposite. The opposite edge connection will
	/// be made later.
	public static void halfLink(final EdgeEPA _edge0, final EdgeEPA _edge1) {
		assert (_edge0.getSourceVertexIndex() == _edge1.getTargetVertexIndex() && _edge0.getTargetVertexIndex() == _edge1.getSourceVertexIndex());
		_edge0.getOwnerTriangle().adjacentEdges[_edge0.getIndex()] = _edge1;
	}
	
	public static boolean link(final EdgeEPA _edge0, final EdgeEPA _edge1) {
		if (_edge0.getSourceVertexIndex() == _edge1.getTargetVertexIndex() && _edge0.getTargetVertexIndex() == _edge1.getSourceVertexIndex()) {
			_edge0.getOwnerTriangle().adjacentEdges[_edge0.getIndex()] = _edge1;
			_edge1.getOwnerTriangle().adjacentEdges[_edge1.getIndex()] = _edge0;
			return true;
		}
		return false;
	}
	
	private final int[] indicesVertices = new int[3]; //!< Indices of the vertices y_i of the triangle
	private final EdgeEPA[] adjacentEdges = new EdgeEPA[3]; //!< Three adjacent edges of the triangle (edges of other triangles)
	private boolean isObsolete; //!< True if the triangle face is visible from the new support point
	private float determinant; //!< Determinant
	private Vector3f closestPoint; //!< Point v closest to the origin on the affine hull of the triangle
	private float lambda1; //!< Lambda1 value such that v = lambda0 * y_0 + lambda1 * y_1 + lambda2 * y_2
	
	private float lambda2; //!< Lambda1 value such that v = lambda0 * y_0 + lambda1 * y_1 + lambda2 * y_2
	
	private float distSquare; //!< Square distance of the point closest point v to the origin
	
	/// Constructor
	/*
	public TriangleEPA() {
		this.adjacentEdges[0] = new EdgeEPA(this, 0);
		this.adjacentEdges[1] = new EdgeEPA(this, 1);
		this.adjacentEdges[2] = new EdgeEPA(this, 2);
		this.closestPoint = new Vector3f();
	}
	*/
	
	/// Constructor
	public TriangleEPA(final int _indexVertex1, final int _indexVertex2, final int _indexVertex3) {
		this.isObsolete = false;
		this.indicesVertices[0] = _indexVertex1;
		this.indicesVertices[1] = _indexVertex2;
		this.indicesVertices[2] = _indexVertex3;
		this.adjacentEdges[0] = new EdgeEPA();
		this.adjacentEdges[1] = new EdgeEPA();
		this.adjacentEdges[2] = new EdgeEPA();
		this.closestPoint = new Vector3f();
	}
	
	/// Private copy-ructor
	/*
	public TriangleEPA(final TriangleEPA _triangle) {
		this.indicesVertices[0] = _triangle.indicesVertices[0];
		this.indicesVertices[1] = _triangle.indicesVertices[1];
		this.indicesVertices[2] = _triangle.indicesVertices[2];
		this.adjacentEdges[0] = _triangle.adjacentEdges[0];
		this.adjacentEdges[1] = _triangle.adjacentEdges[1];
		this.adjacentEdges[2] = _triangle.adjacentEdges[2];
		this.isObsolete = _triangle.isObsolete;
		this.determinant = _triangle.determinant;
		this.closestPoint = _triangle.closestPoint;
		this.lambda1 = _triangle.lambda1;
		this.lambda2 = _triangle.lambda2;
		this.distSquare = _triangle.distSquare;
	}
	*/
	
	/// Compute the point v closest to the origin of this triangle
	public boolean computeClosestPoint(final Vector3f[] _vertices) {
		final Vector3f p0 = _vertices[this.indicesVertices[0]];
		final Vector3f v1 = _vertices[this.indicesVertices[1]].lessNew(p0);
		final Vector3f v2 = _vertices[this.indicesVertices[2]].lessNew(p0);
		final float v1Dotv1 = v1.dot(v1);
		final float v1Dotv2 = v1.dot(v2);
		final float v2Dotv2 = v2.dot(v2);
		final float p0Dotv1 = p0.dot(v1);
		final float p0Dotv2 = p0.dot(v2);
		// Compute determinant
		this.determinant = v1Dotv1 * v2Dotv2 - v1Dotv2 * v1Dotv2;
		// Compute lambda values
		this.lambda1 = p0Dotv2 * v1Dotv2 - p0Dotv1 * v2Dotv2;
		this.lambda2 = p0Dotv1 * v1Dotv2 - p0Dotv2 * v1Dotv1;
		// If the determinant is positive
		if (this.determinant > 0.0f) {
			// Compute the closest point v
			this.closestPoint = v1.multiplyNew(this.lambda1).add(v2.multiplyNew(this.lambda2)).multiply(1.0f / this.determinant).add(p0);
			// Compute the square distance of closest point to the origin
			this.distSquare = this.closestPoint.dot(this.closestPoint);
			return true;
		}
		return false;
	}
	
	/// Compute the point of an object closest to the origin
	public Vector3f computeClosestPointOfObject(final Vector3f[] _supportPointsOfObject) {
		final Vector3f p0 = _supportPointsOfObject[this.indicesVertices[0]].clone();
		final Vector3f tmp1 = _supportPointsOfObject[this.indicesVertices[1]].lessNew(p0).multiply(this.lambda1);
		final Vector3f tmp2 = _supportPointsOfObject[this.indicesVertices[2]].lessNew(p0).multiply(this.lambda2);
		return p0.add(tmp1.add(tmp2).multiply(1.0f / this.determinant));
	}
	
	// Execute the recursive silhouette algorithm from this triangle face.
	/// The parameter "vertices" is an array that contains the vertices of the current polytope and the
	/// parameter "indexNewVertex" is the index of the new vertex in this array. The goal of the
	/// silhouette algorithm is to add the new vertex in the polytope by keeping it convex. Therefore,
	/// the triangle faces that are visible from the new vertex must be removed from the polytope and we
	/// need to add triangle faces where each face contains the new vertex and an edge of the silhouette.
	/// The silhouette is the connected set of edges that are part of the border between faces that
	/// are seen and faces that are not seen from the new vertex. This method starts from the nearest
	/// face from the new vertex, computes the silhouette and create the new faces from the new vertex in
	/// order that we always have a convex polytope. The faces visible from the new vertex are set
	/// obselete and will not be idered as being a candidate face in the future.
	public boolean computeSilhouette(final Vector3f[] _vertices, final int _indexNewVertex, final TrianglesStore _triangleStore) {
		final int first = _triangleStore.getNbTriangles();
		// Mark the current triangle as obsolete because it
		setIsObsolete(true);
		// Execute recursively the silhouette algorithm for the adjacent edges of neighboring
		// triangles of the current triangle
		final boolean result = this.adjacentEdges[0].computeSilhouette(_vertices, _indexNewVertex, _triangleStore)
				&& this.adjacentEdges[1].computeSilhouette(_vertices, _indexNewVertex, _triangleStore) && this.adjacentEdges[2].computeSilhouette(_vertices, _indexNewVertex, _triangleStore);
		if (result) {
			int i, j;
			// For each triangle face that contains the new vertex and an edge of the silhouette
			for (i = first, j = _triangleStore.getNbTriangles() - 1; i != _triangleStore.getNbTriangles(); j = i++) {
				final TriangleEPA triangle = _triangleStore.get(i);
				halfLink(triangle.getAdjacentEdge(1), new EdgeEPA(triangle, 1));
				if (!link(new EdgeEPA(triangle, 0), new EdgeEPA(_triangleStore.get(j), 2))) {
					return false;
				}
			}
		}
		return result;
	}
	
	/// Access operator
	public int get(final int _pos) {
		assert (_pos >= 0 && _pos < 3);
		return this.indicesVertices[_pos];
	}
	/// Link an edge with another one. It means that the current edge of a triangle will
	/// be associated with the edge of another triangle in order that both triangles
	/// are neighbour aint both edges).
	
	/// Return an adjacent edge of the triangle
	public EdgeEPA getAdjacentEdge(final int _index) {
		assert (_index >= 0 && _index < 3);
		return this.adjacentEdges[_index];
	}
	
	/// Return the point closest to the origin
	public Vector3f getClosestPoint() {
		return this.closestPoint;
	}
	
	/// Return the square distance of the closest point to origin
	public float getDistSquare() {
		return this.distSquare;
	}
	
	/// Return true if the triangle face is obsolete
	public boolean getIsObsolete() {
		return this.isObsolete;
	}
	
	// Return true if the closest point on affine hull is inside the triangle
	public boolean isClosestPointInternalToTriangle() {
		return (this.lambda1 >= 0.0f && this.lambda2 >= 0.0 && (this.lambda1 + this.lambda2) <= this.determinant);
	}
	
	/// Return true if the triangle is visible from a given vertex
	public boolean isVisibleFromVertex(final Vector3f[] _vertices, final int _index) {
		final Vector3f closestToVert = _vertices[_index].lessNew(this.closestPoint);
		return (this.closestPoint.dot(closestToVert) > 0.0f);
	}
	
	/// Constructor
	public void set(final int _indexVertex1, final int _indexVertex2, final int _indexVertex3) {
		this.isObsolete = false;
		this.indicesVertices[0] = _indexVertex1;
		this.indicesVertices[1] = _indexVertex2;
		this.indicesVertices[2] = _indexVertex3;
	}
	
	/// Private assignment operator
	public TriangleEPA set(final TriangleEPA _triangle) {
		this.indicesVertices[0] = _triangle.indicesVertices[0];
		this.indicesVertices[1] = _triangle.indicesVertices[1];
		this.indicesVertices[2] = _triangle.indicesVertices[2];
		this.adjacentEdges[0] = _triangle.adjacentEdges[0];
		this.adjacentEdges[1] = _triangle.adjacentEdges[1];
		this.adjacentEdges[2] = _triangle.adjacentEdges[2];
		this.isObsolete = _triangle.isObsolete;
		this.determinant = _triangle.determinant;
		this.closestPoint = _triangle.closestPoint;
		this.lambda1 = _triangle.lambda1;
		this.lambda2 = _triangle.lambda2;
		this.distSquare = _triangle.distSquare;
		return this;
	}
	
	/// Set an adjacent edge of the triangle
	public void setAdjacentEdge(final int _index, final EdgeEPA _edge) {
		assert (_index >= 0 && _index < 3);
		this.adjacentEdges[_index] = _edge;
	}
	
	/// Set the isObsolete value
	public void setIsObsolete(final boolean _isObsolete) {
		this.isObsolete = _isObsolete;
	}
	
}
