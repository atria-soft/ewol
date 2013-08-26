/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */


package org.ewol;

import android.service.wallpaper.WallpaperService;
//import android.app.Activity;
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
import android.util.DisplayMetrics;
import android.view.SurfaceHolder;


import java.io.IOException;

import static org.ewol.Ewol.EWOL;

/**
 * @brief Class : 
 *
 */
public abstract class EwolWallpaper extends WallpaperService
{
	private TestPatternEngine tmpEngine;
	
	static {
		System.loadLibrary("ewol");
	}
	
	@Override public void onCreate()
	{
		super.onCreate();
	}
	
	@Override public void onDestroy()
	{
		super.onDestroy();
	}
	
	@Override public Engine onCreateEngine()
	{
		tmpEngine = new TestPatternEngine();
		return tmpEngine;
	}
	
	protected void initApkPath(String org, String vendor, String project)
	{
		tmpEngine.initApkPath(org, vendor, project);
	}
	
	class TestPatternEngine extends Engine implements EwolCallback, EwolConstants // implements SharedPreferences.OnSharedPreferenceChangeListener
	{
		private EwolSurfaceViewGL mGLView;
		
		
		public void initApkPath(String org, String vendor, String project) {
			StringBuilder sb = new StringBuilder();
			sb.append(org).append(".");
			sb.append(vendor).append(".");
			sb.append(project);
			String apkFilePath = null;
			ApplicationInfo appInfo = null;
			PackageManager packMgmr = getPackageManager();
			try {
				appInfo = packMgmr.getApplicationInfo(sb.toString(), 0);
			} catch (NameNotFoundException e) {
				e.printStackTrace();
				throw new RuntimeException("Unable to locate assets, aborting...");
			}
			apkFilePath = appInfo.sourceDir;
			Ewol.paramSetArchiveDir(0, apkFilePath);
		}
		
		@Override public void onCreate(SurfaceHolder surfaceHolder)
		{
			super.onCreate(surfaceHolder);
			
			// set the java evironement in the C sources :
			Ewol.setJavaVirtualMachineStartWallpaperEngine(this);
			
			// Load the application directory
			Ewol.paramSetArchiveDir(1, getFilesDir().toString());
			Ewol.paramSetArchiveDir(2, getCacheDir().toString());
			// to enable extarnal storage: add in the manifest the restriction needed ...
			//packageManager.checkPermission("android.permission.READ_SMS", myPackage) == PERMISSION_GRANTED; 
			//Ewol.paramSetArchiveDir(3, getExternalCacheDir().toString());
			
			DisplayMetrics metrics = new DisplayMetrics();
			//!getWindowManager().getDefaultDisplay().getMetrics(metrics);
			//!EWOL.displayPropertyMetrics(metrics.xdpi, metrics.ydpi);
			
			// call C init ...
			EWOL.onCreate();
			
			// Remove the title of the current display : 
			//!requestWindowFeature(Window.FEATURE_NO_TITLE);
			// set full screen Mode : 
			//!getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
			// display keyboard:
			//getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
			// hide keyboard : 
			//!getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
			
			// create bsurface system
			mGLView = new EwolSurfaceViewGL(this, 2);
			
			setContentView(mGLView);
		}
		
		@Override public void onStart()
		{
			super.onStart();
			// call C
			EWOL.onStart();
		}
			
		@Override public void onRestart()
		{
			super.onRestart();
			// call C
			EWOL.onReStart();
		}
			
		@Override public void onResume()
		{
			super.onResume();
			mGLView.onResume();
			// call C
			EWOL.onResume();
		}
			
		@Override public void onPause()
		{
			super.onPause();
			mGLView.onPause();
			// call C
			EWOL.onPause();
		}
			
		@Override public void onStop()
		{
			super.onStop();
			// call C
			EWOL.onStop();
		}
		@Override public void onDestroy()
		{
			super.onDestroy();
			// call C
			EWOL.onDestroy();
			// Remove the java Virtual machine pointer form the C code
			Ewol.setJavaVirtualMachineStop();
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
			
		public void keyboardUpdate(boolean show)
		{
			final InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
			if(show) {
				//EWOL.touchEvent();
				imm.toggleSoftInput(InputMethodManager.SHOW_FORCED,0);
			} else {
			imm.toggleSoftInput(0 ,InputMethodManager.HIDE_IMPLICIT_ONLY + InputMethodManager.HIDE_NOT_ALWAYS);
			//imm.hideSoftInputFromWindow(view.getWindowToken(),0); 
			}
		}
			
		public void eventNotifier(String[] args)
		{
			// just for the test ...
			EWOL.touchEvent();
		}
		
		public void orientationUpdate(int screenMode)
		{
			/*
			if (screenMode == EWOL_ORIENTATION_LANDSCAPE) {
				//Force landscape
				setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
			} else if (screenMode == EWOL_ORIENTATION_PORTRAIT) {
				//Force portrait
				setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
			} else {
				//Force auto Rotation
				setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
			}
			*/
		}
	}
}



