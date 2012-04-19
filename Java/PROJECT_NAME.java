


package com.__PROJECT_VENDOR__.__PROJECT_PACKAGE__;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.KeyEvent;
// For No Title : 
import android.view.Window;

// For the full screen : 
import android.view.WindowManager;
// for the keyboard event :
import android.view.inputmethod.InputMethodManager;

import java.io.File;
import android.content.Context;
import android.content.res.Configuration;

// For the getting apk name : 
import android.content.pm.ActivityInfo;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;

/**
 * @brief Class : 
 *
 */
public class __PROJECT_NAME__ extends Activity {
	private static native void TouchEvent();
	private static native void ActivitySetJavaVortualMachineStart(__PROJECT_NAME__ ActivityInstance);
	//private static native void ActivitySetJavaVortualMachineStart();
	private static native void ActivitySetJavaVortualMachineStop();
	private static native void ActivityOnCreate();
	private static native void ActivityOnStart();
	private static native void ActivityOnReStart();
	private static native void ActivityOnResume();
	private static native void ActivityOnPause();
	private static native void ActivityOnStop();
	private static native void ActivityOnDestroy();
	private static native void ActivityParamSetArchiveDir(int mode, String myString);

	private GLSurfaceView mGLView;

	static {
		System.loadLibrary("__PROJECT_PACKAGE__");
		//ActivitySetInstance(this);
	}

	@Override protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		
		// set the java evironement in the C sources : 
		ActivitySetJavaVortualMachineStart(this);
		//ActivitySetJavaVortualMachineStart();
		
		// Load the application directory
		ActivityParamSetArchiveDir(1, getFilesDir().toString());
		ActivityParamSetArchiveDir(2, getCacheDir().toString());
		// to enable extarnal storage: add in the manifest the restriction needed ...
		//ActivityParamSetArchiveDir(3, getExternalCacheDir().toString());
		
		// return apk file path (or null on error)
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = getPackageManager();
		try {
			appInfo = packMgmr.getApplicationInfo("com.__PROJECT_VENDOR__.__PROJECT_PACKAGE__", 0);
		} catch (NameNotFoundException e) {
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
		apkFilePath = appInfo.sourceDir;
		ActivityParamSetArchiveDir(0, apkFilePath);
		
		
		// call C init ...
		ActivityOnCreate();
		
		// Remove the title of the current display : 
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		// set full screen Mode : 
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		// display keyboard:
		//getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
		// hide keyboard : 
		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		//Force landscape
		//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		// create bsurface system
		mGLView = new EwolGLSurfaceView(this);
		setContentView(mGLView);
	}

	@Override protected void onStart()
	{
		super.onStart();
		// call C
		ActivityOnStart();
	}
	
	@Override protected void onRestart()
	{
		super.onRestart();
		// call C
		ActivityOnReStart();
	}
	
	@Override protected void onResume()
	{
		super.onResume();
		mGLView.onResume();
		// call C
		ActivityOnResume();
	}
	
	@Override protected void onPause()
	{
		super.onPause();
		mGLView.onPause();
		// call C
		ActivityOnPause();
	}
	
	@Override protected void onStop()
	{
		super.onStop();
		// call C
		ActivityOnStop();
	}
	@Override protected void onDestroy()
	{
		super.onDestroy();
		// call C
		ActivityOnDestroy();
		// Remove the java Virtual machine pointer form the C code
		ActivitySetJavaVortualMachineStop();
	}
	
	@Override protected void finalize() throws Throwable
	{
		super.finalize();
		// cleanup your object here
	}
	
	public void onConfigurationChanged(Configuration newConfig)
	{
		super.onConfigurationChanged(newConfig);
	}
	
	public void CPP_keyboardShow()
	{
		TouchEvent();
		try{
			getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
		} catch (Exception e) {
			
		}
		TouchEvent();
	}
	
	public static void eventFromCPP(String[] args)
	{
		// just for the test ...
		TouchEvent();
	}
}


/**
 * @brief Class : 
 *
 */
class EwolGLSurfaceView extends GLSurfaceView {
	private static native void nativeApplicationInit();
	private static native void nativeApplicationUnInit();
	private static native void nativeEventInputMotion(int pointerID, float x, float y);
	private static native void nativeEventInputState(int pointerID, boolean isDown, float x, float y);
	private static native void nativeEventUnknow(int eventID);
	
	public EwolGLSurfaceView(Context context) {
		// super must be first statement in constructor
		super(context);
		
		// je n'ai pas compris ...
		mRenderer = new EwolRenderer();
		setRenderer(mRenderer);
		nativeApplicationInit();
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
				nativeEventInputMotion(event.getPointerId(p), (float)event.getX(p), (float)event.getY(p));
			}
		} else if(    tmpActionType == MotionEvent.ACTION_POINTER_1_DOWN 
		           || tmpActionType == MotionEvent.ACTION_DOWN) {
			nativeEventInputState(event.getPointerId(0), true, (float)event.getX(0), (float)event.getY(0));
			InputDown1 = true;
		} else if(tmpActionType == MotionEvent.ACTION_POINTER_1_UP) {
			nativeEventInputState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
			InputDown1 = false;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_2_DOWN) {
			nativeEventInputState(event.getPointerId(1), true, (float)event.getX(1), (float)event.getY(1));
			InputDown2 = true;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_2_UP) {
			nativeEventInputState(event.getPointerId(1), false, (float)event.getX(1), (float)event.getY(1));
			InputDown2 = false;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_3_DOWN) {
			nativeEventInputState(event.getPointerId(2), true, (float)event.getX(2), (float)event.getY(2));
			InputDown3 = true;
		} else if (tmpActionType == MotionEvent.ACTION_POINTER_3_UP) {
			nativeEventInputState(event.getPointerId(2), false, (float)event.getX(2), (float)event.getY(2));
			InputDown3 = false;
		} else if(tmpActionType == MotionEvent.ACTION_UP){
			if (InputDown1) {
				nativeEventInputState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
				InputDown1 = false;
			} else if (InputDown2) {
				nativeEventInputState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
				InputDown2 = false;
			} else {
				nativeEventInputState(event.getPointerId(0), false, (float)event.getX(0), (float)event.getY(0));
				InputDown3 = false;
			}
		}
		return true;
	}
	
	public boolean onKeyDown (int keyCode, KeyEvent event){
		// TODO ...
		return true;
	}
	
	EwolRenderer mRenderer;
}



/**
 * @brief Class : 
 *
 */
class EwolRenderer implements GLSurfaceView.Renderer {
	private static native void nativeInit();
	private static native void nativeResize(int w, int h);
	private static native void nativeRender();
	
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		nativeInit();
	}
	
	public void onSurfaceChanged(GL10 gl, int w, int h) {
		nativeResize(w, h);
	}
	
	public void onDrawFrame(GL10 gl) {
		nativeRender();
	}
}

