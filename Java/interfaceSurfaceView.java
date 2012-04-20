/**
 *******************************************************************************
 * @file ewol interfaceSurfaceView.java
 * @brief Java interface of the java Surface viewer code.
 * @author Edouard DUPIN
 * @date 20/04/2012
 * @par Project
 * ewol
 *
 * @par Copyright
 * Copyright 2011 Edouard DUPIN, all right reserved
 *
 * This software is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY.
 *
 * Licence summary : 
 *    You can modify and redistribute the sources code and binaries.
 *    You can send me the bug-fix
 *
 * Term of the licence in in the file licence.txt.
 *
 *******************************************************************************
 */

package org.ewol;

// inport the ewol package :
import org.ewol.interfaceJNI;
import org.ewol.interfaceOpenGL;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.KeyEvent;


/**
 * @brief Class : 
 *
 */
public class interfaceSurfaceView extends GLSurfaceView {
	
	private interfaceOpenGL m_ewolDrawer;
	
	public interfaceSurfaceView(Context context) {
		// super must be first statement in constructor
		super(context);
		
		// je n'ai pas compris ...
		m_ewolDrawer = new interfaceOpenGL();
		setRenderer(m_ewolDrawer);
		
		// Can get the focus ==> get keyboard from JAVA : 
		setFocusable(true);
		setFocusableInTouchMode(true);
	}
	
	private boolean InputDown1 = false;
	private boolean InputDown2 = false;
	private boolean InputDown3 = false;
	
	public boolean onTouchEvent(final MotionEvent event) {
		// Wrapper on input events : 
		int tmpActionType = event.getAction();
		
		if (tmpActionType == MotionEvent.ACTION_MOVE) {
			final int pointerCount = event.getPointerCount();
			for (int p = 0; p < pointerCount; p++) {
				interfaceJNI.IOInputEventMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
			}
		} else if(    tmpActionType == MotionEvent.ACTION_POINTER_1_DOWN 
		           || tmpActionType == MotionEvent.ACTION_DOWN) {
			interfaceJNI.IOInputEventState(event.getPointerId(0), true, (float)event.getX(0), (float)event.getY(0));
			InputDown1 = true;
		} else if(tmpActionType == MotionEvent.ACTION_POINTER_1_UP) {
			interfaceJNI.IOInputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
			InputDown1 = false;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_2_DOWN) {
			interfaceJNI.IOInputEventState(event.getPointerId(1), true, (float)event.getX(1), (float)event.getY(1));
			InputDown2 = true;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_2_UP) {
			interfaceJNI.IOInputEventState(event.getPointerId(1), false, (float)event.getX(1), (float)event.getY(1));
			InputDown2 = false;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_3_DOWN) {
			interfaceJNI.IOInputEventState(event.getPointerId(2), true, (float)event.getX(2), (float)event.getY(2));
			InputDown3 = true;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_3_UP) {
			interfaceJNI.IOInputEventState(event.getPointerId(2), false, (float)event.getX(2), (float)event.getY(2));
			InputDown3 = false;
		} else if(tmpActionType == MotionEvent.ACTION_UP){
			if (InputDown1) {
				interfaceJNI.IOInputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
				InputDown1 = false;
			} else if (InputDown2) {
				interfaceJNI.IOInputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
				InputDown2 = false;
			} else {
				interfaceJNI.IOInputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
				InputDown3 = false;
			}
		}
		return true;
	}
	
	private boolean keyboardEvent(int keyCode, KeyEvent event, boolean isDown)
	{
		int actionDone = event.getAction();
		switch(keyCode)
		{
			case KeyEvent.KEYCODE_VOLUME_DOWN:
				interfaceJNI.IOKeyboardEventKeySystem(interfaceJNI.EWOL_SYSTEM_KEY__VOLUME_DOWN, isDown);
				return true;
			case KeyEvent.KEYCODE_VOLUME_UP:
				interfaceJNI.IOKeyboardEventKeySystem(interfaceJNI.EWOL_SYSTEM_KEY__VOLUME_UP, isDown);
				return true;
			case KeyEvent.KEYCODE_MENU:
				interfaceJNI.IOKeyboardEventKeySystem(interfaceJNI.EWOL_SYSTEM_KEY__MENU, isDown);
				return true;
			case KeyEvent.KEYCODE_CAMERA:
				interfaceJNI.IOKeyboardEventKeySystem(interfaceJNI.EWOL_SYSTEM_KEY__CAMERA, isDown);
				return true;
			case KeyEvent.KEYCODE_HOME:
				interfaceJNI.IOKeyboardEventKeySystem(interfaceJNI.EWOL_SYSTEM_KEY__HOME, isDown);
				return true;
			case KeyEvent.KEYCODE_POWER:
				interfaceJNI.IOKeyboardEventKeySystem(interfaceJNI.EWOL_SYSTEM_KEY__POWER, isDown);
				return true;
			case KeyEvent.KEYCODE_BACK:
				// the back key is wrapped in the <esc> key to simplify PC validation ...
				interfaceJNI.IOKeyboardEventKey(0x1B, isDown);
				return true;
			case KeyEvent.KEYCODE_DEL:
				interfaceJNI.IOKeyboardEventKey(0x08, isDown);
				return true;
			case KeyEvent.KEYCODE_ALT_LEFT:
				// TODO : ...
				break;
			case KeyEvent.KEYCODE_SHIFT_LEFT:
				// TODO : ...
				break;
			case KeyEvent.KEYCODE_ENTER:
				// TODO : ...
				break;
			// Joystick event :
			case KeyEvent.KEYCODE_DPAD_UP:
				break;
			case KeyEvent.KEYCODE_DPAD_DOWN:
				break;
			case KeyEvent.KEYCODE_DPAD_LEFT:
				break;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
				break;
			case KeyEvent.KEYCODE_DPAD_CENTER:
				break;
			default:
				break;
		}
		// key wrapping :
		if(    (actionDone == KeyEvent.ACTION_DOWN)
		    || (actionDone == KeyEvent.ACTION_MULTIPLE)
		    || (actionDone == KeyEvent.ACTION_UP))
		{
			// convert the key in UniChar to prevent errors ...
			int uchar = event.getUnicodeChar();
			// send it to ewol ...
			interfaceJNI.IOKeyboardEventKey(uchar, isDown);
			return true;
		}
		return false;
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		return keyboardEvent(keyCode, event, true);
	}
	
	public boolean onKeyUp(int keyCode, KeyEvent event)
	{
		return keyboardEvent(keyCode, event, false);
	}
	
}
