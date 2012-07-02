/**
 *******************************************************************************
 * @file ewol interfaceJNI.java
 * @brief Java interface to the CPP code.
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

public class interfaceJNI {
	public static native void TouchEvent();
	// activity status
	public static native void ActivityOnCreate();
	public static native void ActivityOnStart();
	public static native void ActivityOnReStart();
	public static native void ActivityOnResume();
	public static native void ActivityOnPause();
	public static native void ActivityOnStop();
	public static native void ActivityOnDestroy();
	// IO native function :
	// Specific for the type of input : TOOL_TYPE_FINGER and TOOL_TYPE_STYLUS (work as the same)
	public static native void IOInputEventMotion(int pointerID, float x, float y);
	public static native void IOInputEventState(int pointerID, boolean isDown, float x, float y);
	// Specific for the type of input : TOOL_TYPE_MOUSE
	public static native void IOMouseEventMotion(int pointerID, float x, float y);
	public static native void IOMouseEventState(int pointerID, boolean isDown, float x, float y);
	// other unknow event ...
	public static native void IOUnknowEvent(int eventID);
	
	public static native void IOKeyboardEventMove(int type, boolean isDown);
	public static native void IOKeyboardEventKey(int uniChar, boolean isDown);
	
	// Audio section ...
	public static native void IOAudioPlayback(short[] bufferData, int frames, int nbChannels);
	
	public static final int EWOL_SYSTEM_KEY__VOLUME_UP        = 1;
	public static final int EWOL_SYSTEM_KEY__VOLUME_DOWN      = 2;
	public static final int EWOL_SYSTEM_KEY__MENU             = 3;
	public static final int EWOL_SYSTEM_KEY__CAMERA           = 4;
	public static final int EWOL_SYSTEM_KEY__HOME             = 5;
	public static final int EWOL_SYSTEM_KEY__POWER            = 6;
	public static native void IOKeyboardEventKeySystem(int keyVal, boolean isDown);
	// renderer Event : 
	public static native void RenderInit();
	public static native void RenderResize(int w, int h);
	public static native void RenderDraw();
}

