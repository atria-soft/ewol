/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */


package org.ewol;

import android.app.Activity;
import android.content.Context;
import android.Manifest;
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
import android.Manifest;

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
import android.util.Log;
// copy past events :
import android.content.ClipboardManager;
import android.content.ClipData;

import java.io.IOException;
//import activityRootView

import org.ewol.Ewol;



/**
 * @brief Class : 
 *
 */
public abstract class EwolActivity extends Activity implements EwolCallback, EwolConstants {
	private static Context mContext;
	private EwolSurfaceViewGL mGLView;
	private EwolAudioTask     mStreams;
	private Thread            mAudioThread;
	private Ewol              EWOL;
	private String tmpClipBoard; // TODO : Remove this ==> clipboard acces does not work
	static {
		try {
			System.loadLibrary("ewol");
		} catch (UnsatisfiedLinkError e) {
			Log.e("EwolActivity", "error getting lib(): " + e);
		}
	}
	
	public static Context getAppContext() {
		return EwolActivity.mContext;
	}
	
	public EwolActivity() {
		// set the java evironement in the C sources :
		EWOL = new Ewol(this, EWOL_APPL_TYPE_ACTIVITY);
		tmpClipBoard = "";
	}
	
	protected void initApkPath(String org, String vendor, String project) {
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
		EWOL.paramSetArchiveDir(0, apkFilePath);
	}
	
	@Override protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//setListnerToRootView();
		
		EwolActivity.mContext = getApplicationContext();
		
		// Load the application directory
		EWOL.paramSetArchiveDir(1, getFilesDir().toString());
		EWOL.paramSetArchiveDir(2, getCacheDir().toString());
		// to enable extarnal storage: add in the manifest the restriction needed ...
		//packageManager.checkPermission("android.permission.READ_SMS", myPackage) == PERMISSION_GRANTED; 
		//Ewol.paramSetArchiveDir(3, getExternalCacheDir().toString());
		
		DisplayMetrics metrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metrics);
		EWOL.displayPropertyMetrics(metrics.xdpi, metrics.ydpi);
		
		// call C init ...
		EWOL.onCreate();
		
		// Remove the title of the current display : 
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		// set full screen Mode : 
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		// display keyboard:
		//getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
		// hide keyboard : 
		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		
		// create bsurface system
		mGLView = new EwolSurfaceViewGL(this, EWOL);
		
		// create element audio ...
		mStreams = new EwolAudioTask(EWOL);
		
		setContentView(mGLView);
	}
	
	@Override protected void onStart() {
		super.onStart();
		// call C
		EWOL.onStart();
	}
	
	@Override protected void onRestart() {
		super.onRestart();
		// call C
		EWOL.onReStart();
	}
	
	@Override protected void onResume() {
		super.onResume();
		mGLView.onResume();
		/*
		mAudioThread = new Thread(mStreams);
		if (mAudioThread != null) {
			mAudioThread.start();
		}
		*/
		// call C
		EWOL.onResume();
	}
	
	@Override protected void onPause() {
		super.onPause();
		mGLView.onPause();
		/*
		if (mAudioThread != null) {
			// request audio stop
			mStreams.AutoStop();
			// wait the thread ended ...
			try {
				mAudioThread.join();
			} catch(InterruptedException e) { }
		}
		*/
		// call C
		EWOL.onPause();
	}
	
	@Override protected void onStop() {
		super.onStop();
		// call C
		EWOL.onStop();
	}
	@Override protected void onDestroy() {
		super.onDestroy();
		// call C
		EWOL.onDestroy();
		// Remove the java Virtual machine pointer form the C code
		EWOL.setJavaVirtualMachineStop();
	}
	
	@Override protected void finalize() throws Throwable {
		super.finalize();
		// cleanup your object here
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		Log.e("EwolActivity", "Receive event ... ");
		super.onConfigurationChanged(newConfig);
		// Checks whether a hardware keyboard is available
		if (newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_NO) {
			EWOL.setHardKeyboardHidden(false);
			Log.e("EwolActivity", "HARD Keyboard active = " + !EWOL.getHardKeyboardHidden() + " (visible)");
		} else if (newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_YES) {
			EWOL.setHardKeyboardHidden(true);
			Log.e("EwolActivity", "HARD Keyboard active = " + !EWOL.getHardKeyboardHidden() + " (hidden)");
		}
	}
	
	public void keyboardUpdate(boolean show) {
		Log.i("EwolActivity", "set keyboard status visibility :" + show);
		final InputMethodManager imm;
		try {
			imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
		} catch(Exception e) {
			Log.e("EwolActivity", "Can not get keyboard manager ...");
			return;
		}
		Log.i("EwolActivity", "Get input manager done");
		if(show == true) {
			try {
				imm.showSoftInput(mGLView, InputMethodManager.SHOW_IMPLICIT);
			} catch(Exception e) {
				Log.e("EwolActivity", "Can not set keyboard state ... (exeption !!!!)");
			}
			Log.i("EwolActivity", "Display it Done");
		} else {
			// this is a little sutid this ==> display keyboard to be sure that it toggle in the hide state ...
			try {
				imm.showSoftInput(mGLView, InputMethodManager.SHOW_IMPLICIT);
			} catch(Exception e) {
				Log.e("EwolActivity", "Can not set keyboard state ... (exeption !!!!)");
			}
			Log.i("EwolActivity", "Display it Done");
			try {
				imm.toggleSoftInput(0 ,InputMethodManager.HIDE_IMPLICIT_ONLY + InputMethodManager.HIDE_NOT_ALWAYS);
			} catch(Exception e) {
				Log.e("EwolActivity", "Can not set keyboard state ... (exeption !!!!)");
			}
			Log.i("EwolActivity", "Toggle it Done");
		}
	}
	
	public void eventNotifier(String[] args) {
		// TODO : ...
	}
	
	public void orientationUpdate(int screenMode) {
		Context localContext = getAppContext();
		int result = localContext.checkCallingOrSelfPermission(Manifest.permission.SET_ORIENTATION);
		if (result != PackageManager.PERMISSION_GRANTED) {
			if (screenMode == EWOL_ORIENTATION_LANDSCAPE) {
				//Force landscape
				//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
				setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
			} else if (screenMode == EWOL_ORIENTATION_PORTRAIT) {
				//Force portrait
				//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
				setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
			} else {
				//Force auto Rotation
				setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
			}
		} else {
			Log.e("EwolActivity", "Not the right 'SET_ORIENTATION' to access on the screen orientation...");
		}
	}
	
	public void titleSet(String value) {
		setTitle(value);
	}
	
	public void stop() {
		// end application is requested ...
		finish();
	}
	
	//http://developer.android.com/guide/topics/text/copy-paste.html
	public String getClipBoardString() {
		return tmpClipBoard;
		// TODO : Rework this it does not work
		/*
		// Gets a handle to the clipboard service.
		ClipboardManager clipboard = (ClipboardManager)getSystemService(Context.CLIPBOARD_SERVICE);
		// If the clipboard doesn't contain data, disable the paste menu item.
		// If it does contain data, decide if you can handle the data.
		if (!(clipboard.hasPrimaryClip())) {
			return "";
		}
		// Examines the item on the clipboard. If getText() does not return null, the clip item contains the
		// text. Assumes that this application can only handle one item at a time.
		ClipData.Item item = clipboard.getPrimaryClip().getItemAt(0);
		if (item == null) {
			return "";
		}
		// Gets the clipboard as text.
		String pasteData = item.getText().toString();;
		// If the string contains data, then the paste operation is done
		if (pasteData != null) {
			return pasteData;
		}
		return "";
		*/
	}
	
	public void setClipBoardString(String data) {
		tmpClipBoard = data;
		return;
		// TODO : Rework this it does not work
		/*
		// Gets a handle to the clipboard service.
		ClipboardManager clipboard = (ClipboardManager)getSystemService(Context.CLIPBOARD_SERVICE);
		// Creates a new text clip to put on the clipboard
		ClipData clip = ClipData.newPlainText("simple text", data);
		// Set the clipboard's primary clip.
		clipboard.setPrimaryClip(clip);
		*/
	}
	
}



