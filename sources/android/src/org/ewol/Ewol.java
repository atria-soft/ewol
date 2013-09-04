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
	private int instanceID = -1; // local and private instance ID
	
	public <T extends EwolCallback> Ewol(T activityInstance, int typeApplication)
	{
		instanceID = -1;
		instanceID = EWsetJavaVirtualMachineStart(activityInstance, typeApplication);
		Log.d("Ewol", "new : " + instanceID);
	}
	
	public void setJavaVirtualMachineStop()
	{
		EWsetJavaVirtualMachineStop(instanceID);
	}
	public void paramSetArchiveDir(int mode, String myString)
	{
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
		EWdisplayPropertyMetrics(instanceID, ratioX, ratioY);
	}
	// IO native function :
	// Specific for the type of input : TOOL_TYPE_FINGER and TOOL_TYPE_STYLUS (work as the same)
	public void inputEventMotion(int pointerID, float x, float y)
	{
		EWinputEventMotion(instanceID, pointerID, x, y);
	}
	public void inputEventState(int pointerID, boolean isDown, float x, float y)
	{
		EWinputEventState(instanceID, pointerID, isDown, x, y);
	}
	// Specific for the type of input : TOOL_TYPE_MOUSE
	public void mouseEventMotion(int pointerID, float x, float y)
	{
		EWmouseEventMotion(instanceID, pointerID, x, y);
	}
	public void mouseEventState(int pointerID, boolean isDown, float x, float y)
	{
		EWmouseEventState(instanceID, pointerID, isDown, x, y);
	}
	// other unknow event ...
	public void unknowEvent(int eventID)
	{
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
		EWrenderInit(instanceID);
	}
	public void renderResize(int w, int h)
	{
		EWrenderResize(instanceID, w, h);
	}
	public void renderDraw()
	{
		EWrenderDraw(instanceID);
	}
	
	
	private native <T extends EwolCallback> int EWsetJavaVirtualMachineStart(T activityInstance, int typeApplication);
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

