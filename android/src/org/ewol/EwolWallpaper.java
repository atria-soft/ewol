/**
 * @author Edouard DUPIN
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license APACHE v2.0 (see license file)
 */

package org.ewol;

import android.app.ActivityManager;
import android.content.pm.ApplicationInfo;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Build;
import android.service.wallpaper.WallpaperService;
import android.util.Log;
import android.view.SurfaceHolder;
import org.ewol.EwolSurfaceViewGL;
import android.view.MotionEvent;

import android.net.Uri;
import android.content.Intent;
import android.content.ActivityNotFoundException;

import org.ewol.Ewol;

public abstract class EwolWallpaper extends WallpaperService implements EwolCallback, EwolConstants
{
	private GLEngine mGLView;
	private Ewol m_ewolNative;
	
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
	
	@Override public Engine onCreateEngine() {
		// set the java evironement in the C sources :
		m_ewolNative = new Ewol(this, EWOL_APPL_TYPE_WALLPAPER);
		
		// Load the application directory
		m_ewolNative.paramSetArchiveDir(1, getFilesDir().toString());
		m_ewolNative.paramSetArchiveDir(2, getCacheDir().toString());
		// to enable extarnal storage: add in the manifest the restriction needed ...
		//packageManager.checkPermission("android.permission.READ_SMS", myPackage) == PERMISSION_GRANTED; 
		//Ewol.paramSetArchiveDir(3, getExternalCacheDir().toString());
		
		
		//! DisplayMetrics metrics = new DisplayMetrics();
		//! getWindowManager().getDefaultDisplay().getMetrics(metrics);
		//! m_ewolNative.displayPropertyMetrics(metrics.xdpi, metrics.ydpi);
		
		// call C init ...
		m_ewolNative.onCreate();
		
		// create bsurface system
		mGLView = new GLEngine(m_ewolNative);
		
		return mGLView;
	}
	
	public class GLEngine extends Engine {
		private Ewol m_ewolNative;
		public GLEngine(Ewol _ewolInstance) {
			m_ewolNative = _ewolInstance;
		}
		
		class WallpaperGLSurfaceView extends EwolSurfaceViewGL {
			private static final String TAG = "WallpaperGLSurfaceView";
			WallpaperGLSurfaceView(Context _context, Ewol _ewolInstance) {
				super(_context, _ewolInstance);
				Log.d(TAG, "WallpaperGLSurfaceView(" + _context + ")");
			}
			@Override public SurfaceHolder getHolder() {
				Log.d(TAG, "getHolder(): returning " + getSurfaceHolder());
				return getSurfaceHolder();
			}
			public void onDestroy() {
				Log.d(TAG, "onDestroy()");
				super.onDetachedFromWindow();
			}
		}
		
		private static final String TAG = "GLEngine";
		private WallpaperGLSurfaceView glSurfaceView;
		
		@Override public void onCreate(SurfaceHolder _surfaceHolder) {
			Log.d(TAG, "onCreate(" + _surfaceHolder + ")");
			super.onCreate(_surfaceHolder);
			
			glSurfaceView = new WallpaperGLSurfaceView(EwolWallpaper.this, m_ewolNative);
			
			// Check if the system supports OpenGL ES 2.0.
			final ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
			final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
			final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;
			
			if (supportsEs2 == false) {
				Log.d("LiveWallpaper", "does not support board with only open GL ES 1");
				return;
			}
			// Request an OpenGL ES 2.0 compatible context.
			//setEGLContextClientVersion(2);
			
			// On Honeycomb+ devices, this improves the performance when
			// leaving and resuming the live wallpaper.
			//setPreserveEGLContextOnPause(true);
			
		}
		
		@Override public void onTouchEvent(MotionEvent _event) {
			glSurfaceView.onTouchEvent(_event);
		}
		
		@Override public void onVisibilityChanged(boolean _visible) {
			Log.d(TAG, "onVisibilityChanged(" + _visible + ")");
			super.onVisibilityChanged(_visible);
			if (_visible == true) {
				glSurfaceView.onResume();
				// call C
				m_ewolNative.onResume();
			} else {
				glSurfaceView.onPause();
				// call C
				m_ewolNative.onPause();
			}
		}
		
		@Override public void onDestroy() {
			Log.d(TAG, "onDestroy()");
			super.onDestroy();
			// call C
			m_ewolNative.onStop();
			m_ewolNative.onDestroy();
			glSurfaceView.onDestroy();
		}
		
		protected void setPreserveEGLContextOnPause(boolean _preserve) {
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
				Log.d(TAG, "setPreserveEGLContextOnPause(" + _preserve + ")");
				glSurfaceView.setPreserveEGLContextOnPause(_preserve);
			}
		}
		
		protected void setEGLContextClientVersion(int _version) {
			Log.d(TAG, "setEGLContextClientVersion(" + _version + ")");
			glSurfaceView.setEGLContextClientVersion(_version);
		}
	}
	
	public void keyboardUpdate(boolean _show) {
		// never display keyboard on wallpaer...
		Log.d("EwolCallback", "KEABOARD UPDATE is not implemented ...");
	}
	
	public void eventNotifier(String[] _args) {
		// just for the test ...
		Log.d("EwolCallback", "EVENT NOTIFIER is not implemented ...");
		
	}
	
	public void orientationUpdate(int _screenMode) {
		Log.d("EwolCallback", "SET ORIENTATION is not implemented ...");
	}
	
	public void titleSet(String _value) {
		// no title in the wallpaper ...
		Log.d("EwolCallback", "SET TITLE is not implemented ...");
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
		Log.d("EwolCallback", "STOP is not implemented ...");
	}
}


