package org.atriasoft.ephysics.body;

public enum BodyType {
	STATIC, //!< A static body has infinite mass, zero velocity but the position can be changed manually. A static body does not collide with other static or kinematic bodies.
	KINEMATIC, //!< A kinematic body has infinite mass, the velocity can be changed manually and its position is computed by the physics engine. A kinematic body does not collide with other static or kinematic bodies.
	DYNAMIC //!< A dynamic body has non-zero mass, non-zero velocity determined by forces and its position is determined by the physics engine. A dynamic body can collide with other dynamic, static or kinematic bodies.
}
