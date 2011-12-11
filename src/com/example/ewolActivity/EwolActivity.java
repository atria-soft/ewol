


package com.example.ewolAbstraction;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
// For No Title : 
import android.view.Window;

// For the full screen : 
import android.view.WindowManager;


/**
 * @brief Class : 
 *
 */
public class EwolActivity extends Activity {

	private GLSurfaceView mGLView;

	static {
		System.loadLibrary("ewolabstraction");
	}

	@Override protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// Remove the title of the current display : 
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		// set full screen Mode : 
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		//Force landscape
		//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		// create bsurface system
		mGLView = new EwolGLSurfaceView(this);
		setContentView(mGLView);
	}

	@Override protected void onPause() {
		super.onPause();
		mGLView.onPause();
	}

	@Override protected void onResume() {
		super.onResume();
		mGLView.onResume();
	}
}


