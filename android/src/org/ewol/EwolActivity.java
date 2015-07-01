/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license APACHE v2.0 (see license file)
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

import android.net.Uri;
import android.content.Intent;
import android.content.ActivityNotFoundException;

import java.io.IOException;
//import activityRootView

import org.ewol.Ewol;


/**
 * @brief Class : 
 *
 */
public abstract class EwolActivity extends Activity implements EwolCallback, EwolConstants {
	private static Context m_context;
	protected EwolSurfaceViewGL m_glView = null;
	private Ewol m_ewolNative;
	// clipboard section
	private String tmpClipBoard; // TODO : Remove this ==> clipboard acces does not work
	
	public static Context getAppContext() {
		return EwolActivity.m_context;
	}
	
	public EwolActivity() {
		// set the java evironement in the C sources :
		m_ewolNative = new Ewol(this, EWOL_APPL_TYPE_ACTIVITY);
		tmpClipBoard = "";
	}
	
	protected void initApkPath(String _org, String _vendor, String _project) {
		StringBuilder sb = new StringBuilder();
		sb.append(_org).append(".");
		sb.append(_vendor).append(".");
		sb.append(_project);
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
		m_ewolNative.paramSetArchiveDir(0, apkFilePath);
	}
	
	@Override protected void onCreate(Bundle _savedInstanceState) {
		super.onCreate(_savedInstanceState);
		//setListnerToRootView();
		EwolActivity.m_context = getApplicationContext();
		
		// Load the application directory
		m_ewolNative.paramSetArchiveDir(1, getFilesDir().toString());
		m_ewolNative.paramSetArchiveDir(2, getCacheDir().toString());
		// to enable extarnal storage: add in the manifest the restriction needed ...
		//packageManager.checkPermission("android.permission.READ_SMS", myPackage) == PERMISSION_GRANTED; 
		//Ewol.paramSetArchiveDir(3, getExternalCacheDir().toString());
		
		DisplayMetrics metrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metrics);
		m_ewolNative.displayPropertyMetrics(metrics.xdpi, metrics.ydpi);
		
		// call C init ...
		m_ewolNative.onCreate();
		
		// Remove the title of the current display : 
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		// set full screen Mode:
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		// display keyboard:
		//getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_VISIBLE);
		// hide keyboard:
		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);
		
		// create bsurface system
		m_glView = new EwolSurfaceViewGL(this, m_ewolNative);
		
		setContentView(m_glView);
	}
	
	@Override protected void onStart() {
		Log.w("EwolActivity", "onStart (START)");
		super.onStart();
		m_ewolNative.onStart();
		Log.w("EwolActivity", "onStart (STOP)");
	}
	
	@Override protected void onRestart() {
		Log.w("EwolActivity", "onRestart (START)");
		super.onRestart();
		m_ewolNative.onReStart();
		Log.w("EwolActivity", "onRestart (STOP)");
	}
	
	@Override protected void onResume() {
		Log.w("EwolActivity", "onResume (START)");
		super.onResume();
		m_glView.onResume();
		m_ewolNative.onResume();
		Log.w("EwolActivity", "onResume (STOP)");
	}
	
	@Override protected void onPause() {
		Log.w("EwolActivity", "onPause (START)");
		super.onPause();
		m_glView.onPause();
		m_ewolNative.onPause();
		Log.w("EwolActivity", "onPause (STOP)");
	}
	
	@Override protected void onStop() {
		Log.w("EwolActivity", "onStop (START)");
		super.onStop();
		// call C
		m_ewolNative.onStop();
		Log.w("EwolActivity", "onStop (STOP)");
	}
	@Override protected void onDestroy() {
		Log.w("EwolActivity", "onDestroy (START)");
		super.onDestroy();
		// call C
		m_ewolNative.onDestroy();
		// Remove the java Virtual machine pointer form the C code
		m_ewolNative.setJavaVirtualMachineStop();
		Log.w("EwolActivity", "onDestroy (STOP)");
	}
	
	@Override protected void finalize() throws Throwable {
		super.finalize();
		// cleanup your object here
	}
	
	@Override
	public void onConfigurationChanged(Configuration _newConfig) {
		Log.e("EwolActivity", "Receive event ... ");
		super.onConfigurationChanged(_newConfig);
		// Checks whether a hardware keyboard is available
		if (_newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_NO) {
			m_ewolNative.setHardKeyboardHidden(false);
			Log.e("EwolActivity", "HARD Keyboard active = " + !m_ewolNative.getHardKeyboardHidden() + " (visible)");
		} else if (_newConfig.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_YES) {
			m_ewolNative.setHardKeyboardHidden(true);
			Log.e("EwolActivity", "HARD Keyboard active = " + !m_ewolNative.getHardKeyboardHidden() + " (hidden)");
		}
	}
	
	public void keyboardUpdate(boolean _show) {
		Log.i("EwolActivity", "set keyboard status visibility :" + _show);
		final InputMethodManager imm;
		try {
			imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
		} catch(Exception e) {
			Log.e("EwolActivity", "Can not get keyboard manager ...");
			return;
		}
		Log.i("EwolActivity", "Get input manager done");
		if(_show == true) {
			try {
				imm.showSoftInput(m_glView, InputMethodManager.SHOW_IMPLICIT);
			} catch(Exception e) {
				Log.e("EwolActivity", "Can not set keyboard state ... (exeption !!!!)");
			}
			Log.i("EwolActivity", "Display it Done");
		} else {
			// this is a little sutid this ==> display keyboard to be sure that it toggle in the hide state ...
			try {
				imm.showSoftInput(m_glView, InputMethodManager.SHOW_IMPLICIT);
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
	
	public void eventNotifier(String[] _args) {
		// TODO : ...
	}
	
	public void orientationUpdate(int _screenMode) {
		Context localContext = getAppContext();
		int result = localContext.checkCallingOrSelfPermission(Manifest.permission.SET_ORIENTATION);
		if (result != PackageManager.PERMISSION_GRANTED) {
			if (_screenMode == EWOL_ORIENTATION_LANDSCAPE) {
				//Force landscape
				//setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
				setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
			} else if (_screenMode == EWOL_ORIENTATION_PORTRAIT) {
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
	
	public void titleSet(String _value) {
		setTitle(_value);
	}
	
	public void openURI(String _uri) {
		try {
			Intent myIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(_uri));
			startActivity(myIntent);
		} catch (ActivityNotFoundException e) {
			Log.e("EwolActivity", "Can not request an URL");
		}
	}
	
	public void stop() {
		Log.w("EwolActivity", "Application stop requested (START)");
		// end application is requested ...
		finish();
		Log.w("EwolActivity", "Application stop requested (STOP)");
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
	
	public void setClipBoardString(String _data) {
		tmpClipBoard = _data;
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



