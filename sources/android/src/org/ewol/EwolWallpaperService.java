

package org.ewol;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Build;
import android.service.wallpaper.WallpaperService;
import android.util.Log;
import android.view.SurfaceHolder;

//import com.learnopengles.android.util.LoggerConfig;

public abstract class EwolWallpaperService extends WallpaperService
{
	private EwolSurfaceViewGL mGLView;
	/*
	// moi ...
	@Override
	public abstract Engine onCreateEngine(Context context) {
		return new GLEngine(Context context);
	}
	*/
	/*
	static {
		System.loadLibrary("ewol");
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
		Ewol.paramSetArchiveDir(0, apkFilePath);
	}
	*/
	
	public class GLEngine extends Engine
	{
		class WallpaperGLSurfaceView extends GLSurfaceView
		{
			private static final String TAG = "WallpaperGLSurfaceView";
			WallpaperGLSurfaceView(Context context)
			{
				super(context);
				Log.d(TAG, "WallpaperGLSurfaceView(" + context + ")");
			}
			@Override
			public SurfaceHolder getHolder()
			{
				Log.d(TAG, "getHolder(): returning " + getSurfaceHolder());
				return getSurfaceHolder();
			}
			public void onDestroy()
			{
				Log.d(TAG, "onDestroy()");
				super.onDetachedFromWindow();
			}
		}
		
		private static final String TAG = "GLEngine";
		private WallpaperGLSurfaceView glSurfaceView;
		private boolean rendererHasBeenSet;	
		
		@Override
		public void onCreate(SurfaceHolder surfaceHolder)
		{
			Log.d(TAG, "onCreate(" + surfaceHolder + ")");
			super.onCreate(surfaceHolder);
			glSurfaceView = new WallpaperGLSurfaceView(EwolWallpaperService.this);
		}
		@Override
		public void onVisibilityChanged(boolean visible)
		{
			Log.d(TAG, "onVisibilityChanged(" + visible + ")");
			super.onVisibilityChanged(visible);
			if (rendererHasBeenSet) {
				if (visible) {
					glSurfaceView.onResume();
				} else {
					glSurfaceView.onPause();
				}
			}
		}
		@Override
		public void onDestroy()
		{
			Log.d(TAG, "onDestroy()");
			super.onDestroy();
			glSurfaceView.onDestroy();
		}
		protected void setRenderer(Renderer renderer)
		{
			Log.d(TAG, "setRenderer(" + renderer + ")");
			glSurfaceView.setRenderer(renderer);
			rendererHasBeenSet = true;
		}
		protected void setPreserveEGLContextOnPause(boolean preserve)
		{
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
				Log.d(TAG, "setPreserveEGLContextOnPause(" + preserve + ")");
				glSurfaceView.setPreserveEGLContextOnPause(preserve);
			}
		}
		protected void setEGLContextClientVersion(int version)
		{
			Log.d(TAG, "setEGLContextClientVersion(" + version + ")");
			glSurfaceView.setEGLContextClientVersion(version);
		}
	}
}

