/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */

package org.ewol;

import android.app.Activity;
import android.service.wallpaper.WallpaperService;
import android.service.wallpaper.WallpaperService.Engine;

public class Ewol {
	public static final Ewol EWOL = getInstance();
	private static Ewol instance = null;
	
	/* Default constructor (why not ?)*/
	private Ewol() {}
	/* Use singleton */
	public static Ewol getInstance() {
		if(instance == null)
			instance = new Ewol();
		return instance;
	}
	
	public static native <T extends Activity & EwolCallback> void setJavaVirtualMachineStart(T activityInstance);
	public static native <T extends Engine & EwolCallback> void setJavaVirtualMachineStartWallpaperEngine(T serviceInstance);
	public static native void setJavaVirtualMachineStop();
	public static native void paramSetArchiveDir(int mode, String myString);
	
	public native void touchEvent();
	// activity status
	public native void onCreate();
	public native void onStart();
	public native void onReStart();
	public native void onResume();
	public native void onPause();
	public native void onStop();
	public native void onDestroy();
	// set display properties :
	public native void displayPropertyMetrics(float ratioX, float ratioY);
	// IO native function :
	// Specific for the type of input : TOOL_TYPE_FINGER and TOOL_TYPE_STYLUS (work as the same)
	public native void inputEventMotion(int pointerID, float x, float y);
	public native void inputEventState(int pointerID, boolean isDown, float x, float y);
	// Specific for the type of input : TOOL_TYPE_MOUSE
	public native void mouseEventMotion(int pointerID, float x, float y);
	public native void mouseEventState(int pointerID, boolean isDown, float x, float y);
	// other unknow event ...
	public native void unknowEvent(int eventID);
	
	public native void keyboardEventMove(int type, boolean isDown);
	public native void keyboardEventKey(int uniChar, boolean isDown);
	
	// Audio section ...
	public native void audioPlayback(short[] bufferData, int frames, int nbChannels);
	
	public native void keyboardEventKeySystem(int keyVal, boolean isDown);
	// renderer Event : 
	public native void renderInit();
	public native void renderResize(int w, int h);
	public native void renderDraw();
}

