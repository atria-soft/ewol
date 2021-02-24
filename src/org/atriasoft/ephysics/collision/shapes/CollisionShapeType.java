/** @file
 * Original ReactPhysics3D C++ library by Daniel Chappuis <http://www.reactphysics3d.com/> This code is re-licensed with permission from ReactPhysics3D author.
 * @author Daniel CHAPPUIS
 * @author Edouard DUPIN
 * @copyright 2010-2016, Daniel Chappuis
 * @copyright 2017-now, Edouard DUPIN
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ephysics.collision.shapes;

/**
 * Type of the collision shape
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
public enum CollisionShapeType {
	TRIANGLE(0),
	BOX(1),
	SPHERE(2),
	CONE(3),
	CYLINDER(4),
	CAPSULE(5),
	CONVEX_MESH(6),
	CONCAVE_MESH(7),
	HEIGHTFIELD(8);
	
	public static final int NB_COLLISION_SHAPE_TYPES = 9;
	
	public static CollisionShapeType getType(final int value) {
		switch (value) {
			case 0:
				return TRIANGLE;
			case 1:
				return BOX;
			case 2:
				return SPHERE;
			case 3:
				return CONE;
			case 4:
				return CYLINDER;
			case 5:
				return CAPSULE;
			case 6:
				return CONVEX_MESH;
			case 7:
				return CONCAVE_MESH;
			case 8:
				return HEIGHTFIELD;
		}
		return null;
	}
	
	public final int value;
	
	private CollisionShapeType(final int value) {
		this.value = value;
	}
}
