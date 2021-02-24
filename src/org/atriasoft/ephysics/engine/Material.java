package org.atriasoft.ephysics.engine;

import org.atriasoft.ephysics.Configuration;

public class Material {
	
	private float frictionCoefficient; //!< Friction coefficient (positive value)
	private float rollingResistance; //!< Rolling resistance factor (positive value)
	private float bounciness; //!< Bounciness during collisions (between 0 and 1) where 1 is for a very bouncy body
	
	/// Constructor
	public Material() {
		this.frictionCoefficient = Configuration.DEFAULT_FRICTION_COEFFICIENT;
		this.rollingResistance = Configuration.DEFAULT_ROLLING_RESISTANCE;
		this.bounciness = Configuration.DEFAULT_BOUNCINESS;
	}
	
	/// Copy-ructor
	public Material(final Material material) {
		this.frictionCoefficient = material.frictionCoefficient;
		this.rollingResistance = material.rollingResistance;
		this.bounciness = material.bounciness;
	}
	
	/// Return the bounciness
	/// @return Bounciness factor (between 0 and 1) where 1 is very bouncy
	public float getBounciness() {
		return this.bounciness;
	}
	
	/**Return the friction coefficient
	 * @return Friction coefficient (positive value)
	 */
	public float getFrictionCoefficient() {
		return this.frictionCoefficient;
	}
	
	/** Return the rolling resistance factor. If this value is larger than zero,
	 * it will be used to slow down the body when it is rolling
	 * against another body.
	 * @return The rolling resistance factor (positive value)
	 */
	public float getRollingResistance() {
		return this.rollingResistance;
	}
	
	/// Overloaded assignment operator
	public Material set(final Material material) {
		
		// Check for self-assignment
		if (this != material) {
			this.frictionCoefficient = material.frictionCoefficient;
			this.bounciness = material.bounciness;
			this.rollingResistance = material.rollingResistance;
		}
		
		// Return this material
		return this;
	}
	
	/** Set the bounciness.
	 * The bounciness should be a value between 0 and 1. The value 1 is used for a
	 * very bouncy body and zero is used for a body that is not bouncy at all.
	 * @param bounciness Bounciness factor (between 0 and 1) where 1 is very bouncy
	 */
	public void setBounciness(final float bounciness) {
		assert (bounciness >= 0.0f && bounciness <= 1.0f);
		this.bounciness = bounciness;
	}
	
	/** Set the friction coefficient.
	 * The friction coefficient has to be a positive value. The value zero is used for no
	 * friction at all.
	 * @param frictionCoefficient Friction coefficient (positive value)
	 */
	public void setFrictionCoefficient(final float frictionCoefficient) {
		assert (frictionCoefficient >= 0.0f);
		this.frictionCoefficient = frictionCoefficient;
	}
	
	/** Set the rolling resistance factor. If this value is larger than zero,
	 * it will be used to slow down the body when it is rolling
	 * against another body.
	 * @param rollingResistance The rolling resistance factor
	 */
	public void setRollingResistance(final float rollingResistance) {
		assert (rollingResistance >= 0);
		this.rollingResistance = rollingResistance;
	}
	
}
