/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license APACHE v2.0 (see license file)
 */

package org.ewol;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.KeyEvent;
import android.util.Log;

import org.ewol.Ewol;

/**
 * @brief Class : 
 *
 */
public class EwolSurfaceViewGL extends GLSurfaceView implements EwolConstants {
	public static final int SDK_VERSION = android.os.Build.VERSION.SDK_INT;
	private EwolRendererGL m_ewolDrawer = null;
	private boolean inputDown1 = false;
	private boolean inputDown2 = false;
	private boolean inputDown3 = false;
	private Ewol EWOL;
		
	public EwolSurfaceViewGL(Context context, Ewol ewolInstance) {
		// super must be first statement in constructor
		super(context);
		EWOL = ewolInstance;
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
		setEGLContextClientVersion(2);
		
		// je n'ai pas compris ...
		m_ewolDrawer = new EwolRendererGL(EWOL);
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
					if(    typeOfPointer == MotionEvent.TOOL_TYPE_FINGER
					    || typeOfPointer == MotionEvent.TOOL_TYPE_STYLUS) {
						EWOL.inputEventMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
					} else if(typeOfPointer == MotionEvent.TOOL_TYPE_MOUSE) {
						EWOL.mouseEventMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
					}
				} else {
					EWOL.inputEventMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
				}
			}
		} else if(	tmpActionType == MotionEvent.ACTION_POINTER_1_DOWN 
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
	
	private boolean keyboardEvent(int keyCode, KeyEvent event, boolean isDown) {
		int actionDone = event.getAction();
		Log.i("Surface GL", "get event : " + keyCode + " is down : " + isDown);
		switch(keyCode) {
			case KeyEvent.KEYCODE_VOLUME_DOWN:
				return EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_VOLUME_DOWN, isDown);
			case KeyEvent.KEYCODE_VOLUME_UP:
				return EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_VOLUME_UP, isDown);
			case KeyEvent.KEYCODE_MENU:
				return EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_MENU, isDown);
			case KeyEvent.KEYCODE_CAMERA:
				return EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_CAMERA, isDown);
			case KeyEvent.KEYCODE_HOME:
				return EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_HOME, isDown);
			case KeyEvent.KEYCODE_POWER:
				return EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_POWER, isDown);
			case KeyEvent.KEYCODE_BACK:
				// the back key is wrapped in the <esc> key to simplify PC validation ...
				return EWOL.keyboardEventKeySystem(EWOL_SYSTEM_KEY_BACK, isDown);
				/*
				EWOL.keyboardEventKey(EWOL_SYSTEM_KEY_BACK, isDown);
				return false;
				*/
			case KeyEvent.KEYCODE_DEL:
				EWOL.keyboardEventKey(EWOL_SYSTEM_KEY_DEL, isDown);
				return true;
				// Joystick event :
			case KeyEvent.KEYCODE_DPAD_UP:    EWOL.keyboardEventMove(EWOL_MOVE_KEY_UP, isDown);        return true;
			case KeyEvent.KEYCODE_DPAD_DOWN:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_DOWN, isDown);      return true;
			case KeyEvent.KEYCODE_DPAD_LEFT:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_LEFT, isDown);      return true;
			case KeyEvent.KEYCODE_DPAD_RIGHT: EWOL.keyboardEventMove(EWOL_MOVE_KEY_RIGHT, isDown);     return true;
			case KeyEvent.KEYCODE_PAGE_UP:    EWOL.keyboardEventMove(EWOL_MOVE_KEY_PAGE_UP, isDown);   return true;
			case KeyEvent.KEYCODE_PAGE_DOWN:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_PAGE_DOWN, isDown); return true;
			case KeyEvent.KEYCODE_MOVE_HOME:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_START, isDown);     return true;
			case KeyEvent.KEYCODE_MOVE_END:   EWOL.keyboardEventMove(EWOL_MOVE_KEY_END, isDown);       return true;
			case KeyEvent.KEYCODE_SYSRQ:      EWOL.keyboardEventMove(EWOL_MOVE_KEY_PRINT, isDown);     return true;
			//case KeyEvent.:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_STOP_DEFIL, isDown); return true;
			case KeyEvent.KEYCODE_BREAK:      EWOL.keyboardEventMove(EWOL_MOVE_KEY_WAIT, isDown);      return true;
			case KeyEvent.KEYCODE_INSERT:     EWOL.keyboardEventMove(EWOL_MOVE_KEY_INSERT, isDown);    return true;
			case KeyEvent.KEYCODE_F1:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F1, isDown);  return true;
			case KeyEvent.KEYCODE_F2:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F2, isDown);  return true;
			case KeyEvent.KEYCODE_F3:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F3, isDown);  return true;
			case KeyEvent.KEYCODE_F4:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F4, isDown);  return true;
			case KeyEvent.KEYCODE_F5:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F5, isDown);  return true;
			case KeyEvent.KEYCODE_F6:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F6, isDown);  return true;
			case KeyEvent.KEYCODE_F7:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F7, isDown);  return true;
			case KeyEvent.KEYCODE_F8:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F8, isDown);  return true;
			case KeyEvent.KEYCODE_F9:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_F9, isDown);  return true;
			case KeyEvent.KEYCODE_F10: EWOL.keyboardEventMove(EWOL_MOVE_KEY_F10, isDown); return true;
			case KeyEvent.KEYCODE_F11: EWOL.keyboardEventMove(EWOL_MOVE_KEY_F11, isDown); return true;
			case KeyEvent.KEYCODE_F12: EWOL.keyboardEventMove(EWOL_MOVE_KEY_F12, isDown); return true;
			case KeyEvent.KEYCODE_CAPS_LOCK:   EWOL.keyboardEventMove(EWOL_MOVE_KEY_CAP_LOCK, isDown);     return true;
			case KeyEvent.KEYCODE_SHIFT_LEFT:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_SHIFT_LEFT, isDown);   return true;
			case KeyEvent.KEYCODE_SHIFT_RIGHT: EWOL.keyboardEventMove(EWOL_MOVE_KEY_SHIFT_RIGHT, isDown);  return true;
			case KeyEvent.KEYCODE_CTRL_LEFT:   EWOL.keyboardEventMove(EWOL_MOVE_KEY_CTRL_LEFT, isDown);    return true;
			case KeyEvent.KEYCODE_CTRL_RIGHT:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_CTRL_RIGHT, isDown);   return true;
			case KeyEvent.KEYCODE_META_LEFT:   EWOL.keyboardEventMove(EWOL_MOVE_KEY_META_LEFT, isDown);    return true;
			case KeyEvent.KEYCODE_META_RIGHT:  EWOL.keyboardEventMove(EWOL_MOVE_KEY_META_RIGHT, isDown);   return true;
			case KeyEvent.KEYCODE_ALT_LEFT:    EWOL.keyboardEventMove(EWOL_MOVE_KEY_ALT, isDown);          return true;
			case KeyEvent.KEYCODE_ALT_RIGHT:   EWOL.keyboardEventMove(EWOL_MOVE_KEY_ALT_GR, isDown);       return true;
			//case KeyEvent.KEYCODE_MENU:        EWOL.keyboardEventMove(EWOL_MOVE_KEY_CONTEXT_MENU, isDown); return true;
			case KeyEvent.KEYCODE_NUM_LOCK:    EWOL.keyboardEventMove(EWOL_MOVE_KEY_NUM_LOCK, isDown);     return true;
			default:
				break;
			}
		// key wrapping :
		if(	(actionDone == KeyEvent.ACTION_DOWN)
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
	
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		return keyboardEvent(keyCode, event, true);
	}
	
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		return keyboardEvent(keyCode, event, false);
	}
}
