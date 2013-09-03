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

import android.util.Log;

public class Ewol {
	private static int counterInstance = -1; // Global instance increment at eveny new instance
	private int instanceID = -1; // local and private instance ID
	
	/* Default constructor (why not ?)*/
	public Ewol()
	{
		// TODO : Set an unique intance ID
		counterInstance ++;
		instanceID = counterInstance;
		Log.d("Ewol", "new " + counterInstance + " : " + instanceID);
	}
	// internal Acces not at the native function ...
	
	public <T extends Activity & EwolCallback> void setJavaVirtualMachineStart(T activityInstance)
	{
		// TODO : Get the instance ID here ...
		EWsetJavaVirtualMachineStart(activityInstance);
	}
	public <T extends WallpaperService & EwolCallback> void setJavaVirtualMachineStartWallpaperEngine(T serviceInstance)
	{
		// TODO : Get the instance ID here ...
		EWsetJavaVirtualMachineStartWallpaperEngine(serviceInstance);
	}
	public void setJavaVirtualMachineStop()
	{
		EWsetJavaVirtualMachineStop(instanceID);
	}
	public void paramSetArchiveDir(int mode, String myString)
	{
		Log.d("Ewol", "call : " + instanceID);
		EWparamSetArchiveDir(instanceID, mode, myString);
	}
	
	// activity status
	public void onCreate()  { EWonCreate(instanceID); }
	public void onStart()   { EWonStart(instanceID); }
	public void onReStart() { EWonReStart(instanceID); }
	public void onResume()  { EWonResume(instanceID); }
	public void onPause()   { EWonPause(instanceID); }
	public void onStop()    { EWonStop(instanceID); }
	public void onDestroy() { EWonDestroy(instanceID); }
	// set display properties :
	public void displayPropertyMetrics(float ratioX, float ratioY)
	{
		Log.d("Ewol", "call : " + instanceID);
		EWdisplayPropertyMetrics(instanceID, ratioX, ratioY);
	}
	// IO native function :
	// Specific for the type of input : TOOL_TYPE_FINGER and TOOL_TYPE_STYLUS (work as the same)
	public void inputEventMotion(int pointerID, float x, float y)
	{
		Log.d("Ewol", "call : " + instanceID);
		EWinputEventMotion(instanceID, pointerID, x, y);
	}
	public void inputEventState(int pointerID, boolean isDown, float x, float y)
	{
		Log.d("Ewol", "call : " + instanceID);
		EWinputEventState(instanceID, pointerID, isDown, x, y);
	}
	// Specific for the type of input : TOOL_TYPE_MOUSE
	public void mouseEventMotion(int pointerID, float x, float y)
	{
		Log.d("Ewol", "call : " + instanceID);
		EWmouseEventMotion(instanceID, pointerID, x, y);
	}
	public void mouseEventState(int pointerID, boolean isDown, float x, float y)
	{
		Log.d("Ewol", "call : " + instanceID);
		EWmouseEventState(instanceID, pointerID, isDown, x, y);
	}
	// other unknow event ...
	public void unknowEvent(int eventID)
	{
		Log.d("Ewol", "call : " + instanceID);
		EWunknowEvent(instanceID, eventID);
	}
	
	public void keyboardEventMove(int type, boolean isDown)
	{
		EWkeyboardEventMove(instanceID, type, isDown);
	}
	public void keyboardEventKey(int uniChar, boolean isDown)
	{
		EWkeyboardEventKey(instanceID, uniChar, isDown);
	}
	
	// Audio section ...
	public void audioPlayback(short[] bufferData, int frames, int nbChannels)
	{
		EWaudioPlayback(instanceID, bufferData, frames, nbChannels);
	}
	
	public void keyboardEventKeySystem(int keyVal, boolean isDown)
	{
		EWkeyboardEventKeySystem(instanceID, keyVal, isDown);
	}
	// renderer Event : 
	public void renderInit()
	{
		Log.d("Ewol", "call : " + instanceID);
		EWrenderInit(instanceID);
	}
	public void renderResize(int w, int h)
	{
		Log.d("Ewol", "call : " + instanceID);
		EWrenderResize(instanceID, w, h);
	}
	public void renderDraw()
	{
		Log.d("Ewol", "call : " + instanceID);
		EWrenderDraw(instanceID);
	}
	
	
	private native <T extends Activity & EwolCallback> void EWsetJavaVirtualMachineStart(T activityInstance);
	private native <T extends WallpaperService & EwolCallback> void EWsetJavaVirtualMachineStartWallpaperEngine(T serviceInstance);
	private native void EWsetJavaVirtualMachineStop(int instanceId);
	private native void EWparamSetArchiveDir(int instanceId, int mode, String myString);
	
	// activity status
	private native void EWonCreate(int instanceId);
	private native void EWonStart(int instanceId);
	private native void EWonReStart(int instanceId);
	private native void EWonResume(int instanceId);
	private native void EWonPause(int instanceId);
	private native void EWonStop(int instanceId);
	private native void EWonDestroy(int instanceId);
	// set display properties :
	private native void EWdisplayPropertyMetrics(int instanceId, float ratioX, float ratioY);
	// IO native function :
	// Specific for the type of input : TOOL_TYPE_FINGER and TOOL_TYPE_STYLUS (work as the same)
	private native void EWinputEventMotion(int instanceId, int pointerID, float x, float y);
	private native void EWinputEventState(int instanceId, int pointerID, boolean isDown, float x, float y);
	// Specific for the type of input : TOOL_TYPE_MOUSE
	private native void EWmouseEventMotion(int instanceId, int pointerID, float x, float y);
	private native void EWmouseEventState(int instanceId, int pointerID, boolean isDown, float x, float y);
	// other unknow event ...
	private native void EWunknowEvent(int instanceId, int eventID);
	
	private native void EWkeyboardEventMove(int instanceId, int type, boolean isDown);
	private native void EWkeyboardEventKey(int instanceId, int uniChar, boolean isDown);
	
	// Audio section ...
	public native void EWaudioPlayback(int instanceId, short[] bufferData, int frames, int nbChannels);
	
	private native void EWkeyboardEventKeySystem(int instanceId, int keyVal, boolean isDown);
	// renderer Event : 
	private native void EWrenderInit(int instanceId);
	private native void EWrenderResize(int instanceId, int w, int h);
	private native void EWrenderDraw(int instanceId);
}

