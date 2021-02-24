/*
 * ReactPhysics3D physics library, http://code.google.com/p/reactphysics3d/
 * Copyright (c) 2010-2013 Daniel Chappuis
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * This file has been modified during the port to Java and differ from the source versions.
 */
package org.atriasoft.ephysics.body;

/**
 * This class is an abstract class to represent a body of the physics engine.
 *
 * @author Jason Sorensen <sorensenj@smert.net>
 */
public abstract class Body {
	// Unique ID of the body
	protected int id; // TODO check where it came from ...
	// True if the body has already been added in an island (for sleeping technique)
	public boolean isAlreadyInIsland = false;
	// True if the body is allowed to go to sleep for better efficiency
	protected boolean isAllowedToSleep = true;
	/**
	 * True if the body is active.
	 * 
	 * An inactive body does not participate in collision detection, is not simulated and will not be hit in a ray casting query.
	 * A body is active by default. If you set this value to "false", all the proxy shapes of this body will be removed from the broad-phase.
	 * If you set this value to "true", all the proxy shapes will be added to the broad-phase.
	 * A joint connected to an inactive body will also be inactive.
	 */
	protected boolean isActive = true;
	// True if the body is sleeping (for sleeping technique)
	protected boolean isSleeping = false;
	// Elapsed time since the body velocity was bellow the sleep velocity
	public float sleepTime = 0;
	// user data
	protected Object userData = null;
	
	/**
	 * @brief Constructor
	 * @param[in] _id ID of the new body
	 */
	public Body(final int bodyID) {
		this.id = bodyID;
	}
	
	// Return the id of the body
	public int getID() {
		return this.id;
	}
	
	public float getSleepTime() {
		return this.sleepTime;
	}
	
	/**
	 * @brief Return a pointer to the user data attached to this body
	 * @return A pointer to the user data you have attached to the body
	 */
	Object getUserData() {
		return this.userData;
	}
	
	/**
	 * @brief Return the id of the body
	 * @return The ID of the body
	 */
	public boolean isActive() {
		return this.isActive;
	}
	
	/**
	 * @brief Set whether or not the body is allowed to go to sleep
	 * @param[in] _isAllowedToSleep True if the body is allowed to sleep
	 */
	public boolean isAllowedToSleep() {
		return this.isAllowedToSleep;
	}
	
	public boolean isAlreadyInIsland() {
		return this.isAlreadyInIsland;
	}
	
	/**
	 * @brief Return whether or not the body is sleeping
	 * @return True if the body is currently sleeping and false otherwise
	 */
	public boolean isSleeping() {
		return this.isSleeping;
	}
	
	/**
	 * @brief Set whether or not the body is active
	 * @param[in] _isActive True if you want to activate the body
	 */
	void setIsActive(final boolean isActive) {
		this.isActive = isActive;
	}
	
	// Set whether or not the body is allowed to go to sleep
	public void setIsAllowedToSleep(final boolean isAllowedToSleep) {
		this.isAllowedToSleep = isAllowedToSleep;
		
		if (!this.isAllowedToSleep) {
			setIsSleeping(false);
		}
	}
	
	public void setIsAlreadyInIsland(final boolean isAlreadyInIsland) {
		this.isAlreadyInIsland = isAlreadyInIsland;
	}
	
	/**
	 * @brief Set the variable to know whether or not the body is sleeping
	 * @param[in] _isSleeping Set the new status
	 */
	public void setIsSleeping(final boolean isSleeping) {
		if (isSleeping) {
			this.sleepTime = 0.0f;
		} else if (this.isSleeping) {
			this.sleepTime = 0.0f;
		}
		this.isSleeping = isSleeping;
	}
	
	public void setSleepTime(final float sleepTime) {
		this.sleepTime = sleepTime;
	}
	
	/**
	 * @brief Attach user data to this body
	 * @param[in] _userData A pointer to the user data you want to attach to the body
	 */
	public void setUserData(final Object userData) {
		this.userData = userData;
	}
	
}
