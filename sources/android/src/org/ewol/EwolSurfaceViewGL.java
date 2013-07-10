/**
*******************************************************************************
* @file EwolSurfaceViewGL.java
* @brief Java interface of the java Surface viewer code.
* @author Edouard DUPIN, Kevin BILLONNEAU
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

// import the ewol package :
/* No need in same package... */
//import org.ewol.Ewol;
//import org.ewol.EwolRendererGL;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.KeyEvent;

import static org.ewol.Ewol.EWOL;

/**
 * @brief Class : 
 *
 */
public class EwolSurfaceViewGL extends GLSurfaceView implements EwolConstants {
    public static final int  SDK_VERSION   = android.os.Build.VERSION.SDK_INT;
    private EwolRendererGL   m_ewolDrawer  = null;
    private boolean          inputDown1    = false;
    private boolean          inputDown2    = false;
    private boolean          inputDown3    = false;
	
    public EwolSurfaceViewGL(Context context, int openGlVersion) {
	// super must be first statement in constructor
	super(context);
	/*
	  List of the Android API :
	  Android 4.1, 4.1.1          16  JELLY_BEAN               Platform Highlights
	  Android 4.0.3, 4.0.4        15  ICE_CREAM_SANDWICH_MR1   Platform Highlights
	  Android 4.0, 4.0.1, 4.0.2   14  ICE_CREAM_SANDWICH
	  Android 3.2                 13  HONEYCOMB_MR2
	  Android 3.1.x               12  HONEYCOMB_MR1            Platform Highlights
	  Android 3.0.x               11  HONEYCOMB                Platform Highlights
	  Android 2.3.4
	  Android 2.3.3               10  GINGERBREAD_MR1          Platform Highlights
	  Android 2.3.2
	  Android 2.3.1
	  Android 2.3                  9  GINGERBREAD
	  Android 2.2.x                8  FROYO                    Platform Highlights
	  Android 2.1.x                7  ECLAIR_MR1               Platform Highlights
	  Android 2.0.1                6  ECLAIR_0_1
	  Android 2.0                  5  ECLAIR
	  Android 1.6                  4  DONUT                    Platform Highlights
	  Android 1.5                  3  CUPCAKE                  Platform Highlights
	  Android 1.1                  2  BASE_1_1
	  Android 1.0                  1  BASE
	*/
	// Create an OpenGL ES 2.0 context
	if (openGlVersion == 2) {
	    setEGLContextClientVersion(2);
	}
		
	// je n'ai pas compris ...
	m_ewolDrawer = new EwolRendererGL();
	setRenderer(m_ewolDrawer);
		
	// Can get the focus ==> get keyboard from JAVA : 
	setFocusable(true);
	setFocusableInTouchMode(true);
    }
	
    public boolean onTouchEvent(final MotionEvent event) {
	// Wrapper on input events : 
		
	int tmpActionType = event.getAction();
		
	if (tmpActionType == MotionEvent.ACTION_MOVE) {
	    final int pointerCount = event.getPointerCount();
	    for (int p = 0; p < pointerCount; p++) {
		if (SDK_VERSION>=14) {
		    final int typeOfPointer = event.getToolType(p);
		    if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
			  || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
			EWOL.inputEventMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
		    } else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
			EWOL.mouseEventMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
		    }
		} else {
		    EWOL.inputEventMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
		}
	    }
	} else if(    tmpActionType == MotionEvent.ACTION_POINTER_1_DOWN 
		      || tmpActionType == MotionEvent.ACTION_DOWN) {
	    if (SDK_VERSION>=14) {
		final int typeOfPointer = event.getToolType(0);
		if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
		      || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
		    EWOL.inputEventState(event.getPointerId(0), true, (float)event.getX(0), (float)event.getY(0));
		} else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
		    EWOL.mouseEventState(event.getPointerId(0), true, (float)event.getX(0), (float)event.getY(0));
		}
	    } else {
		EWOL.inputEventState(event.getPointerId(0), true, (float)event.getX(0), (float)event.getY(0));
	    }
	    inputDown1 = true;
	} else if(tmpActionType == MotionEvent.ACTION_POINTER_1_UP) {
	    if (SDK_VERSION>=14) {
		final int typeOfPointer = event.getToolType(0);
		if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
		      || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
		    EWOL.inputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		} else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
		    EWOL.mouseEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		}
	    } else {
		EWOL.inputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
	    }
	    inputDown1 = false;
	} else if (tmpActionType == MotionEvent.ACTION_POINTER_2_DOWN) {
	    if (SDK_VERSION>=14) {
		final int typeOfPointer = event.getToolType(1);
		if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
		      || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
		    EWOL.inputEventState(event.getPointerId(1), true, (float)event.getX(1), (float)event.getY(1));
		} else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
		    EWOL.mouseEventState(event.getPointerId(1), true, (float)event.getX(1), (float)event.getY(1));
		}
	    } else {
		EWOL.inputEventState(event.getPointerId(1), true, (float)event.getX(1), (float)event.getY(1));
	    }
	    inputDown2 = true;
	} else if (tmpActionType == MotionEvent.ACTION_POINTER_2_UP) {
	    if (SDK_VERSION>=14) {
		final int typeOfPointer = event.getToolType(1);
		if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
		      || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
		    EWOL.inputEventState(event.getPointerId(1), false, (float)event.getX(1), (float)event.getY(1));
		} else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
		    EWOL.mouseEventState(event.getPointerId(1), false, (float)event.getX(1), (float)event.getY(1));
		}
	    } else {
		EWOL.inputEventState(event.getPointerId(1), false, (float)event.getX(1), (float)event.getY(1));
	    }
	    inputDown2 = false;
	} else if (tmpActionType == MotionEvent.ACTION_POINTER_3_DOWN) {
	    if (SDK_VERSION>=14) {
		final int typeOfPointer = event.getToolType(2);
		if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
		      || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
		    EWOL.inputEventState(event.getPointerId(2), true, (float)event.getX(2), (float)event.getY(2));
		} else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
		    EWOL.mouseEventState(event.getPointerId(2), true, (float)event.getX(2), (float)event.getY(2));
		}
	    } else {
		EWOL.inputEventState(event.getPointerId(2), true, (float)event.getX(2), (float)event.getY(2));
	    }
	    inputDown3 = true;
	} else if (tmpActionType == MotionEvent.ACTION_POINTER_3_UP) {
	    if (SDK_VERSION>=14) {
		final int typeOfPointer = event.getToolType(2);
		if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
		      || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
		    EWOL.inputEventState(event.getPointerId(2), false, (float)event.getX(2), (float)event.getY(2));
		} else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
		    EWOL.mouseEventState(event.getPointerId(2), false, (float)event.getX(2), (float)event.getY(2));
		}

	    } else {
		EWOL.inputEventState(event.getPointerId(2), false, (float)event.getX(2), (float)event.getY(2));
	    }
	    inputDown3 = false;
	} else if(tmpActionType == MotionEvent.ACTION_UP){
	    if (inputDown1) {
		if (SDK_VERSION>=14) {
		    final int typeOfPointer = event.getToolType(0);
		    if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
			  || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
			EWOL.inputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		    } else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
			EWOL.mouseEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		    }
		} else {
		    EWOL.inputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		}
		inputDown1 = false;
	    } else if (inputDown2) {
		if (SDK_VERSION>=14) {
		    final int typeOfPointer = event.getToolType(0);
		    if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
			  || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
			EWOL.inputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		    } else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
			EWOL.mouseEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		    }
		} else {
		    EWOL.inputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		}
		inputDown2 = false;
	    } else {
		if (SDK_VERSION>=14) {
		    final int typeOfPointer = event.getToolType(0);
		    if(   typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
			  || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
			EWOL.inputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		    } else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
			EWOL.mouseEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		    }
		} else {
		    EWOL.inputEventState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
		}
		inputDown3 = false;
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
		EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_VOLUME_DOWN, isDown);
		return false;
	    case KeyEvent.KEYCODE_VOLUME_UP:
		EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_VOLUME_UP, isDown);
		return false;
	    case KeyEvent.KEYCODE_MENU:
		EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_MENU, isDown);
		return false;
	    case KeyEvent.KEYCODE_CAMERA:
		EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_CAMERA, isDown);
		return false;
	    case KeyEvent.KEYCODE_HOME:
		EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_HOME, isDown);
		return false;
	    case KeyEvent.KEYCODE_POWER:
		EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_POWER, isDown);
		return false;
	    case KeyEvent.KEYCODE_BACK:
		// the back key is wrapped in the <esc> key to simplify PC validation ...
		EWOL.keyboardEventKey(EWOL_SYSTEM_KEY_BACK, isDown);
		return false;
	    case KeyEvent.KEYCODE_DEL:
		EWOL.keyboardEventKey(EWOL_SYSTEM_KEY_DEL, isDown);
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
		// pb on the return methode ... in java it is like windows ...
		if (uchar == '\r') {
		    uchar = '\n';
		}
		// send it to ewol ...
		EWOL.keyboardEventKey(uchar, isDown);
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
