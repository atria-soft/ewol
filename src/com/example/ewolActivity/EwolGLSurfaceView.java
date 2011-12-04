

package com.example.ewolAbstraction;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;


/**
 * @brief Class : 
 *
 */
class EwolGLSurfaceView extends GLSurfaceView {
	private static native void nativeApplicationInit();
	private static native void nativePause();
	private static native void nativeApplicationUnInit();
	private static native void nativeEventInputMotion(int pointerID, float x, float y);
	private static native void nativeEventInputState(int pointerID, boolean isDown);
	private static native void nativeEventUnknow(int eventID);
	
	public EwolGLSurfaceView(Context context) {
		super(context);
		mRenderer = new EwolRenderer();
		setRenderer(mRenderer);
		nativeApplicationInit();
	}

	private boolean InputDown1 = false;
	private boolean InputDown2 = false;
	private boolean InputDown3 = false;
	
	public boolean onTouchEvent(final MotionEvent event) {
		// TODO : unneed code : 
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			nativePause();
		}
		// Wrapper on input events : 
		int tmpActionType = event.getAction();
		
		if (tmpActionType == MotionEvent.ACTION_MOVE) {
			final int pointerCount = event.getPointerCount();
			for (int p = 0; p < pointerCount; p++) {
				nativeEventInputMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
			}
		} else if(    tmpActionType == MotionEvent.ACTION_POINTER_1_DOWN 
		           || tmpActionType == MotionEvent.ACTION_DOWN) {
			nativeEventInputState(0, true);
			InputDown1 = true;
			nativeEventInputMotion(event.getPointerId(0), event.getX(0), event.getY(0));
		} else if(tmpActionType == MotionEvent.ACTION_POINTER_1_UP) {
			nativeEventInputState(0, false);
			InputDown1 = false;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_2_DOWN) {
			nativeEventInputState(1, true);
			InputDown2 = true;
			nativeEventInputMotion(event.getPointerId(1), event.getX(1), event.getY(1));
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_2_UP) {
			nativeEventInputState(1, false);
			InputDown2 = false;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_3_DOWN) {
			nativeEventInputState(2, true);
			InputDown3 = true;
			nativeEventInputMotion(event.getPointerId(2), event.getX(2), event.getY(2));
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_3_UP) {
			nativeEventInputState(2, false);
			InputDown3 = false;
		} else if(tmpActionType == MotionEvent.ACTION_UP){
			if (InputDown1) {
				nativeEventInputState(0, false);
				InputDown1 = false;
			} else if (InputDown2) {
				nativeEventInputState(1, false);
				InputDown2 = false;
			} else {
				nativeEventInputState(2, false);
				InputDown3 = false;
			}
		} else {
			nativeEventUnknow(tmpActionType);
		}
		return true;
	}
	
	EwolRenderer mRenderer;
}