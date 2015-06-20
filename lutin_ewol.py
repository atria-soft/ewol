#!/usr/bin/python
import lutin.module as module
import lutin.tools as tools
import lutin.debug as debug
import os
import lutin.multiprocess as lutinMultiprocess

def get_desc():
	return "ewol is a main library to use widget in the openGl environement and manage all the wraping os"

def get_license():
	return "APACHE v2.0"

def create(target):
	# set the ewol folder for Android basic sources ...
	# TODO: Remove this really bad interface :
	target.set_ewol_folder(tools.get_current_path(__file__))
	
	# module name is 'edn' and type binary.
	myModule = module.Module(__file__, 'ewol', 'LIBRARY')
	
	# add extra compilation flags :
	myModule.add_extra_compile_flags()
	# add the file to compile:
	myModule.add_src_file([
		'ewol/ewol.cpp',
		'ewol/debug.cpp',
		'ewol/Padding.cpp',
		'ewol/Dimension.cpp',
		'ewol/translate.cpp'
		])
	
	# compositing :
	myModule.add_src_file([
		'ewol/compositing/Compositing.cpp',
		'ewol/compositing/TextBase.cpp',
		'ewol/compositing/Text.cpp',
		'ewol/compositing/TextDF.cpp',
		'ewol/compositing/Drawing.cpp',
		'ewol/compositing/Image.cpp',
		'ewol/compositing/Sprite.cpp',
		'ewol/compositing/Shaper.cpp',
		'ewol/compositing/Area.cpp'
		])
	
	# context :
	myModule.add_src_file([
		'ewol/context/clipBoard.cpp',
		'ewol/context/commandLine.cpp',
		'ewol/context/ConfigFont.cpp',
		'ewol/context/Context.cpp',
		'ewol/context/cursor.cpp',
		'ewol/context/InputManager.cpp'
		])
	if target.name=="Linux":
		myModule.add_src_file('ewol/context/X11/Context.cpp')
	elif target.name=="Windows":
		myModule.add_src_file('ewol/context/Windows/Context.cpp')
	elif target.name=="Android":
		myModule.add_src_file('ewol/context/Android/Context.cpp')
		myModule.add_src_file([
			'android/src/org/ewol/EwolAudioTask.java',
			'android/src/org/ewol/EwolCallback.java',
			'android/src/org/ewol/EwolConstants.java',
			'android/src/org/ewol/Ewol.java',
			'android/src/org/ewol/EwolRendererGL.java',
			'android/src/org/ewol/EwolSurfaceViewGL.java',
			'android/src/org/ewol/EwolActivity.java',
			'android/src/org/ewol/EwolWallpaper.java',
			'org.ewol.EwolConstants.javah'
			])
		myModule.add_path(tools.get_current_path(__file__) + '/android/src/', type='java')
	elif target.name=="MacOs":
		myModule.add_src_file([
			'ewol/context/MacOs/Context.mm',
			'ewol/context/MacOs/Interface.mm',
			'ewol/context/MacOs/Windows.mm',
			'ewol/context/MacOs/OpenglView.mm',
			'ewol/context/MacOs/AppDelegate.mm'])
	elif target.name=="IOs":
		myModule.add_src_file([
			'ewol/context/IOs/Context.cpp',
			'ewol/context/IOs/Interface.m',
			'ewol/context/IOs/OpenglView.mm',
			'ewol/context/IOs/AppDelegate.mm'])
	else:
		debug.error("unknow mode...")
	
	# event properties :
	myModule.add_src_file([
		'ewol/event/Entry.cpp',
		'ewol/event/Time.cpp',
		'ewol/event/Input.cpp'
		])
	
	# Key properties :
	myModule.add_src_file([
		'ewol/key/keyboard.cpp',
		'ewol/key/Special.cpp',
		'ewol/key/status.cpp',
		'ewol/key/type.cpp'
		])
	
	# object :
	myModule.add_src_file([
		'ewol/object/Manager.cpp',
		'ewol/object/Object.cpp',
		'ewol/object/Worker.cpp'
		])
	# parameter :
	myModule.add_src_file([
		'ewol/parameter/Parameter.cpp',
		'ewol/parameter/Interface.cpp',
		])
	# Signal :
	myModule.add_src_file([
		'ewol/signal/Interface.cpp',
		'ewol/signal/Base.cpp'
		])
	
	# OpenGL interface :
	myModule.add_src_file([
		'ewol/openGL/openGL.cpp'
		])
	
	# resources :
	myModule.add_src_file([
		'ewol/resource/Colored3DObject.cpp',
		'ewol/resource/ColorFile.cpp',
		'ewol/resource/ConfigFile.cpp',
		'ewol/resource/FontFreeType.cpp',
		'ewol/resource/Image.cpp',
		'ewol/resource/ImageDF.cpp',
		'ewol/resource/Manager.cpp',
		'ewol/resource/Program.cpp',
		'ewol/resource/Resource.cpp',
		'ewol/resource/Shader.cpp',
		'ewol/resource/Texture.cpp',
		'ewol/resource/TexturedFont.cpp',
		'ewol/resource/DistanceFieldFont.cpp',
		'ewol/resource/VirtualBufferObject.cpp'
		])
	
	# widget :
	myModule.add_src_file([
		'ewol/widget/ButtonColor.cpp',
		'ewol/widget/Button.cpp',
		'ewol/widget/CheckBox.cpp',
		'ewol/widget/ColorBar.cpp',
		'ewol/widget/Composer.cpp',
		'ewol/widget/Container.cpp',
		'ewol/widget/Container2.cpp',
		'ewol/widget/ContainerN.cpp',
		'ewol/widget/ContextMenu.cpp',
		'ewol/widget/Entry.cpp',
		'ewol/widget/Gird.cpp',
		'ewol/widget/Image.cpp',
		'ewol/widget/Joystick.cpp',
		'ewol/widget/Label.cpp',
		'ewol/widget/Layer.cpp',
		'ewol/widget/List.cpp',
		'ewol/widget/ListFileSystem.cpp',
		'ewol/widget/Manager.cpp',
		'ewol/widget/Menu.cpp',
		'ewol/widget/meta/ColorChooser.cpp',
		'ewol/widget/meta/FileChooser.cpp',
		'ewol/widget/meta/Parameter.cpp',
		'ewol/widget/meta/ParameterList.cpp',
		'ewol/widget/meta/StdPopUp.cpp',
		'ewol/widget/PopUp.cpp',
		'ewol/widget/ProgressBar.cpp',
		'ewol/widget/Scroll.cpp',
		'ewol/widget/Sizer.cpp',
		'ewol/widget/Slider.cpp',
		'ewol/widget/Spacer.cpp',
		'ewol/widget/Widget.cpp',
		'ewol/widget/WidgetScrolled.cpp',
		'ewol/widget/Windows.cpp',
		'ewol/widget/WSlider.cpp',
		])
	
	myModule.copy_folder('data/theme/shape/square/*','theme/shape/square')
	myModule.copy_folder('data/theme/shape/round/*','theme/shape/round')
	myModule.copy_folder('data/theme/color/black/*','theme/color/black')
	myModule.copy_folder('data/theme/color/white/*','theme/color/white')
	myModule.copy_folder('data/textured.*','')
	myModule.copy_folder('data/texturedNoMaterial.*','')
	myModule.copy_folder('data/text.*','')
	myModule.copy_folder('data/simple3D.*','')
	myModule.copy_folder('data/color.*','')
	myModule.copy_folder('data/color3.*','')
	myModule.copy_folder('data/textured3D2.*','')
	myModule.copy_folder('data/textured3D.*','')
	myModule.copy_folder('data/texturedDF.*','')
	myModule.copy_folder('data/fontDistanceField/*','fontDistanceField')
	myModule.copy_folder('data/translate/*','translate/ewol/')
	
	# name of the dependency
	myModule.add_module_depend(['etk', 'freetype', 'exml', 'ejson', 'egami', 'edtaa3', 'date'])
	
	myModule.add_export_path(tools.get_current_path(__file__))

	tagFile = tools.get_current_path(__file__) + "/tag"
	ewolVersionID = tools.file_read_data(tagFile)
	myModule.compile_flags('c++', [
		"-DEWOL_VERSION=\"\\\""+ewolVersionID+"\\\"\""
		])
	
	if target.name=="Linux":
		myModule.add_export_flag('link', '-lGL')
		#`pkg-config --cflags directfb` `pkg-config --libs directfb`
		#ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_X11__)","y")
		myModule.add_export_flag('link', '-lX11')
		#endif
		#ifeq ("$(CONFIG___EWOL_LINUX_GUI_MODE_DIRECT_FB__)","y")
		#myModule.add_export_flag('link', ['-L/usr/local/lib', '-ldirectfb', '-lfusion', '-ldirect'])
		#endif
	elif target.name=="Android":
		myModule.add_module_depend("SDK")
		myModule.add_export_flag('link', "-lGLESv2")
		
		myModule.add_export_flag('link', "-ldl")
		myModule.add_export_flag('link', "-llog")
		myModule.add_export_flag('link', "-landroid")
		# add tre creator of the basic java class ...
		target.add_action("PACKAGE", tool_generate_main_java_class)
	elif target.name=="Windows":
		myModule.add_module_depend("glew")
	elif target.name=="MacOs":
		myModule.add_export_flag('link', [
			"-framework Cocoa",
			"-framework OpenGL",
			"-framework QuartzCore",
			"-framework AppKit"])
	elif target.name=="IOs":
		myModule.add_export_flag('link', [
			"-framework OpenGLES",
			"-framework CoreGraphics",
			"-framework UIKit",
			"-framework GLKit",
			"-framework Foundation",
			"-framework QuartzCore"])
	
	return myModule



##################################################################
##
## Android specific section
##
##################################################################
def tool_generate_main_java_class(target, module, package_name):
	file_list = []
	
	debug.debug("------------------------------------------------------------------------")
	debug.info("Generate android wrapping for '" + package_name + "'")
	debug.debug("------------------------------------------------------------------------")
	application_name = package_name
	if target.config["mode"] == "debug":
		application_name += "debug"
	target.folder_java_project=   target.get_build_folder(package_name) \
	                            + "/src/" \
	                            + module.package_prop["COMPAGNY_TYPE"] \
	                            + "/" + module.package_prop["COMPAGNY_NAME2"] \
	                            + "/" + application_name + "/"
	
	java_file_wrapper = target.folder_java_project + "/" + application_name + ".java"
	
	android_package_name = module.package_prop["COMPAGNY_TYPE"]+"."+module.package_prop["COMPAGNY_NAME2"]+"." + application_name
	
	if "ADMOD_ID" in module.package_prop:
		module.package_prop["RIGHT"].append("INTERNET")
		module.package_prop["RIGHT"].append("ACCESS_NETWORK_STATE")
	
	
	debug.print_element("pkg", "absractionFile", "<==", "dynamic file")
	# Create folder :
	tools.create_directory_of_file(java_file_wrapper)
	debug.info("create file : '" + java_file_wrapper + "'")
	# Create file :
	tmpFile = open(java_file_wrapper + "_tmp", 'w')
	if module.package_prop["ANDROID_APPL_TYPE"]=="APPL":
		tmpFile.write( "/**\n")
		tmpFile.write( " * @author Edouard DUPIN, Kevin BILLONNEAU\n")
		tmpFile.write( " * @copyright 2011, Edouard DUPIN, all right reserved\n")
		tmpFile.write( " * @license APACHE v2.0 (see license file)\n")
		tmpFile.write( " * @note This file is autogenerate ==> see documantation to generate your own\n")
		tmpFile.write( " */\n")
		tmpFile.write( "package "+ android_package_name + ";\n")
		tmpFile.write( "import android.util.Log;\n")
		tmpFile.write( "import org.ewol.EwolActivity;\n")
		if "ADMOD_ID" in module.package_prop:
			tmpFile.write( "import com.google.android.gms.ads.AdRequest;\n")
			tmpFile.write( "import com.google.android.gms.ads.AdSize;\n")
			tmpFile.write( "import com.google.android.gms.ads.AdView;\n")
			tmpFile.write( "import android.widget.LinearLayout;\n")
			tmpFile.write( "import android.widget.Button;\n")
		tmpFile.write( "public class " + application_name + " extends EwolActivity {\n")
		if "ADMOD_ID" in module.package_prop:
			tmpFile.write( "	/** The view to show the ad. */\n")
			tmpFile.write( "	private AdView adView;\n")
			tmpFile.write( "	private LinearLayout mLayout = null;\n")
		tmpFile.write( "	\n")
		tmpFile.write( "	static {\n")
		tmpFile.write( "		try {\n")
		tmpFile.write( "			System.loadLibrary(\"" + package_name + "\");\n")
		tmpFile.write( "		} catch (UnsatisfiedLinkError e) {\n")
		tmpFile.write( "			Log.e(\"" + application_name + "\", \"error getting lib(): \" + e);\n")
		tmpFile.write( "		}\n")
		tmpFile.write( "	}\n")
		tmpFile.write( "	\n")
		tmpFile.write( "	public void onCreate(android.os.Bundle savedInstanceState) {\n")
		tmpFile.write( "		super.onCreate(savedInstanceState);\n")
		tmpFile.write( "		initApkPath(\"" + module.package_prop["COMPAGNY_TYPE"]+"\", \""+module.package_prop["COMPAGNY_NAME2"]+"\", \"" + application_name + "\");\n")
		if "ADMOD_ID" in module.package_prop:
			tmpFile.write( "		mLayout = new LinearLayout(this);\n")
			tmpFile.write( "		mLayout.setOrientation(android.widget.LinearLayout.VERTICAL);\n")
			tmpFile.write( "		LinearLayout.LayoutParams paramsWindows = new LinearLayout.LayoutParams(\n")
			tmpFile.write( "			LinearLayout.LayoutParams.FILL_PARENT,\n")
			tmpFile.write( "			LinearLayout.LayoutParams.FILL_PARENT);\n")
			tmpFile.write( "		\n")
			tmpFile.write( "		setContentView(mLayout, paramsWindows);\n")
			tmpFile.write( "		\n")
			tmpFile.write( "		LinearLayout.LayoutParams paramsAdds = new LinearLayout.LayoutParams(\n")
			tmpFile.write( "			LinearLayout.LayoutParams.FILL_PARENT,\n")
			tmpFile.write( "			LinearLayout.LayoutParams.WRAP_CONTENT);\n")
			tmpFile.write( "		paramsAdds.weight = 0;\n")
			tmpFile.write( "		\n")
			tmpFile.write( "		LinearLayout.LayoutParams paramsGLView = new LinearLayout.LayoutParams(\n")
			tmpFile.write( "			LinearLayout.LayoutParams.FILL_PARENT,\n")
			tmpFile.write( "			LinearLayout.LayoutParams.FILL_PARENT);\n")
			tmpFile.write( "		paramsGLView.weight = 1;\n")
			tmpFile.write( "		paramsGLView.height = 0;\n")
			tmpFile.write( "		\n")
			tmpFile.write( "		mLayout.setGravity(android.view.Gravity.TOP);\n")
			tmpFile.write( "		\n")
			tmpFile.write( "		// Create an adds.\n")
			tmpFile.write( "		adView = new AdView(this);\n")
			tmpFile.write( "		adView.setAdSize(AdSize.SMART_BANNER);\n")
			tmpFile.write( "		adView.setAdUnitId(\"" + module.package_prop["ADMOD_ID"] + "\");\n")
			tmpFile.write( "		\n")
			tmpFile.write( "		// Create an ad request. Check logcat output for the hashed device ID to get test ads on a physical device.\n")
			tmpFile.write( "		AdRequest adRequest = new AdRequest.Builder()\n")
			tmpFile.write( "			.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)\n")
			tmpFile.write( "			.build();\n")
			tmpFile.write( "		\n")
			tmpFile.write( "		// Add the AdView to the view hierarchy. The view will have no size until the ad is loaded.\n")
			if     "ADMOD_POSITION" in module.package_prop.keys() \
			   and module.package_prop["ADMOD_POSITION"] == "top":
				tmpFile.write( "		mLayout.addView(adView, paramsAdds);\n")
				tmpFile.write( "		mLayout.addView(mGLView, paramsGLView);\n")
			else:
				tmpFile.write( "		mLayout.addView(mGLView, paramsGLView);\n")
				tmpFile.write( "		mLayout.addView(adView, paramsAdds);\n")
			tmpFile.write( "		\n")
			tmpFile.write( "		// Start loading the ad in the background.\n")
			tmpFile.write( "		adView.loadAd(adRequest);\n")
		tmpFile.write( "	}\n")
		if "ADMOD_ID" in module.package_prop:
			tmpFile.write( "	@Override protected void onResume() {\n")
			tmpFile.write( "		super.onResume();\n")
			tmpFile.write( "		if (adView != null) {\n")
			tmpFile.write( "			adView.resume();\n")
			tmpFile.write( "		}\n")
			tmpFile.write( "	}\n")
			tmpFile.write( "	@Override protected void onPause() {\n")
			tmpFile.write( "		if (adView != null) {\n")
			tmpFile.write( "			adView.pause();\n")
			tmpFile.write( "		}\n")
			tmpFile.write( "		super.onPause();\n")
			tmpFile.write( "	}\n")
			tmpFile.write( "	@Override protected void onDestroy() {\n")
			tmpFile.write( "		// Destroy the AdView.\n")
			tmpFile.write( "		if (adView != null) {\n")
			tmpFile.write( "			adView.destroy();\n")
			tmpFile.write( "		}\n")
			tmpFile.write( "		super.onDestroy();\n")
			tmpFile.write( "	}\n")
		tmpFile.write( "}\n")
	else :
		# wallpaper mode ...
		tmpFile.write( "/**\n")
		tmpFile.write( " * @author Edouard DUPIN, Kevin BILLONNEAU\n")
		tmpFile.write( " * @copyright 2011, Edouard DUPIN, all right reserved\n")
		tmpFile.write( " * @license APACHE v2.0 (see license file)\n")
		tmpFile.write( " * @note This file is autogenerate ==> see documantation to generate your own\n")
		tmpFile.write( " */\n")
		tmpFile.write( "package "+ android_package_name + ";\n")
		tmpFile.write( "import android.util.Log;\n")
		tmpFile.write( "import org.ewol.EwolWallpaper;\n")
		tmpFile.write( "public class " + application_name + " extends EwolWallpaper {\n")
		tmpFile.write( "	public static final String SHARED_PREFS_NAME = \"" + application_name + "settings\";\n")
		tmpFile.write( "	\n")
		tmpFile.write( "	static {\n")
		tmpFile.write( "		try {\n")
		tmpFile.write( "			System.loadLibrary(\"" + package_name + "\");\n")
		tmpFile.write( "		} catch (UnsatisfiedLinkError e) {\n")
		tmpFile.write( "			Log.e(\"" + application_name + "\", \"error getting lib(): \" + e);\n")
		tmpFile.write( "		}\n")
		tmpFile.write( "	}\n")
		tmpFile.write( "	\n")
		tmpFile.write( "	public Engine onCreateEngine() {\n")
		tmpFile.write( "		Engine tmpEngine = super.onCreateEngine();\n")
		tmpFile.write( "		initApkPath(\"" + module.package_prop["COMPAGNY_TYPE"]+"\", \""+module.package_prop["COMPAGNY_NAME2"]+"\", \"" + application_name + "\");\n")
		tmpFile.write( "		return tmpEngine;\n")
		tmpFile.write( "	}\n")
		tmpFile.write( "}\n")
	tmpFile.flush()
	tmpFile.close()
	
	tools.move_if_needed(java_file_wrapper + "_tmp", java_file_wrapper);
	# add java file to build:
	module.add_src_file([java_file_wrapper])
	
	"""
	    ## todo:
	tools.create_directory_of_file(target.get_staging_folder(package_name) + "/res/drawable/icon.png");
	if     "ICON" in module.package_prop.keys() \
	   and module.package_prop["ICON"] != "":
		image.resize(module.package_prop["ICON"], target.get_staging_folder(package_name) + "/res/drawable/icon.png", 256, 256)
	else:
		# to be sure that we have all time a resource ...
		tmpFile = open(target.get_staging_folder(package_name) + "/res/drawable/plop.txt", 'w')
		tmpFile.write('plop\n')
		tmpFile.flush()
		tmpFile.close()
	
	"""
	if module.package_prop["ANDROID_MANIFEST"] == "":
		# force manifest file:
		module.package_prop["ANDROID_MANIFEST"] = target.get_build_folder(package_name) + "/AndroidManifest.xml";
		debug.info(" create file: '" + module.package_prop["ANDROID_MANIFEST"] + "'")
		if "VERSION_CODE" not in module.package_prop:
			module.package_prop["VERSION_CODE"] = "1"
		debug.print_element("pkg", "AndroidManifest.xml", "<==", "package configurations")
		tools.create_directory_of_file(module.package_prop["ANDROID_MANIFEST"])
		tmpFile = open(module.package_prop["ANDROID_MANIFEST"], 'w')
		tmpFile.write( '<?xml version="1.0" encoding="utf-8"?>\n')
		tmpFile.write( '<!-- Manifest is autoGenerated with Ewol ... do not patch it-->\n')
		tmpFile.write( '<manifest xmlns:android="http://schemas.android.com/apk/res/android" \n')
		tmpFile.write( '          package="' + android_package_name + '" \n')
		tmpFile.write( '          android:versionCode="'+module.package_prop["VERSION_CODE"]+'" \n')
		tmpFile.write( '          android:versionName="'+module.package_prop["VERSION"]+'"> \n')
		tmpFile.write( '	<uses-feature android:glEsVersion="0x00020000" android:required="true" />\n')
		tmpFile.write( '	<uses-sdk android:minSdkVersion="' + str(target.boardId) + '" \n')
		tmpFile.write( '	          android:targetSdkVersion="' + str(target.boardId) + '" /> \n')
		if module.package_prop["ANDROID_APPL_TYPE"]=="APPL":
			tmpFile.write( '	<application android:label="' + application_name + '" \n')
			if "ICON" in module.package_prop.keys():
				tmpFile.write( '	             android:icon="@drawable/icon" \n')
			if target.config["mode"] == "debug":
				tmpFile.write( '	             android:debuggable="true" \n')
			tmpFile.write( '	             >\n')
			if "ADMOD_ID" in module.package_prop:
				tmpFile.write( '		<meta-data android:name="com.google.android.gms.version" \n')
				tmpFile.write( '		           android:value="@integer/google_play_services_version"/>\n')
			
			tmpFile.write( '		<activity android:name=".' + application_name + '" \n')
			tmpFile.write( '		          android:label="' + module.package_prop['NAME'])
			if target.config["mode"] == "debug":
				tmpFile.write("-debug")
			tmpFile.write( '"\n')
			if "ICON" in module.package_prop.keys():
				tmpFile.write( '		          android:icon="@drawable/icon" \n')
			tmpFile.write( '		          android:hardwareAccelerated="true" \n')
			tmpFile.write( '		          android:configChanges="keyboard|keyboardHidden|orientation|screenSize"> \n')
			tmpFile.write( '			<intent-filter> \n')
			tmpFile.write( '				<action android:name="android.intent.action.MAIN" /> \n')
			tmpFile.write( '				<category android:name="android.intent.category.LAUNCHER" /> \n')
			tmpFile.write( '			</intent-filter> \n')
			tmpFile.write( '		</activity> \n')
			if "ADMOD_ID" in module.package_prop:
				tmpFile.write( '		<activity android:name="com.google.android.gms.ads.AdActivity"\n')
				tmpFile.write( '		          android:configChanges="keyboard|keyboardHidden|orientation|screenLayout|uiMode|screenSize|smallestScreenSize"/>\n')
			
			tmpFile.write( '	</application>\n')
		else:
			tmpFile.write( '	<application android:label="' + application_name + '" \n')
			tmpFile.write( '	             android:permission="android.permission.BIND_WALLPAPER" \n')
			if "ICON" in module.package_prop.keys():
				tmpFile.write( '	             android:icon="@drawable/icon"\n')
			tmpFile.write( '	             >\n')
			tmpFile.write( '		<service android:name=".' + application_name + '" \n')
			tmpFile.write( '		         android:label="' + module.package_prop['NAME'])
			if target.config["mode"] == "debug":
				tmpFile.write("-debug")
			tmpFile.write( '"\n')
			if "ICON" in module.package_prop.keys():
				tmpFile.write( '		         android:icon="@drawable/icon"\n')
			tmpFile.write( '		         >\n')
			tmpFile.write( '			<intent-filter>\n')
			tmpFile.write( '				<action android:name="android.service.wallpaper.WallpaperService" />\n')
			tmpFile.write( '			</intent-filter>\n')
			tmpFile.write( '			<meta-data android:name="android.service.wallpaper"\n')
			tmpFile.write( '			           android:resource="@xml/' + application_name + '_resource" />\n')
			tmpFile.write( '		</service>\n')
			if len(module.package_prop["ANDROID_WALLPAPER_PROPERTIES"])!=0:
				tmpFile.write( '		<activity android:label="Setting"\n')
				tmpFile.write( '		          android:name=".' + application_name + 'Settings"\n')
				tmpFile.write( '		          android:theme="@android:style/Theme.Light.WallpaperSettings"\n')
				tmpFile.write( '		          android:exported="true"\n')
				if "ICON" in module.package_prop.keys():
					tmpFile.write( '		          android:icon="@drawable/icon"\n')
				tmpFile.write( '		          >\n')
				tmpFile.write( '		</activity>\n')
			tmpFile.write( '	</application>\n')
		# write package autorisations :
		if True==target.check_right_package(module.package_prop, "WRITE_EXTERNAL_STORAGE"):
			tmpFile.write( '	<permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" /> \n')
		if True==target.check_right_package(module.package_prop, "CAMERA"):
			tmpFile.write( '	<permission android:name="android.permission.CAMERA" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.CAMERA" /> \n')
		if True==target.check_right_package(module.package_prop, "INTERNET"):
			tmpFile.write( '	<permission android:name="android.permission.INTERNET" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.INTERNET" /> \n')
		if True==target.check_right_package(module.package_prop, "ACCESS_NETWORK_STATE"):
			tmpFile.write( '	<permission android:name="android.permission.ACCESS_NETWORK_STATE" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" /> \n')
		if True==target.check_right_package(module.package_prop, "MODIFY_AUDIO_SETTINGS"):
			tmpFile.write( '	<permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" /> \n')
		if True==target.check_right_package(module.package_prop, "READ_CALENDAR"):
			tmpFile.write( '	<permission android:name="android.permission.READ_CALENDAR" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.READ_CALENDAR" /> \n')
		if True==target.check_right_package(module.package_prop, "READ_CONTACTS"):
			tmpFile.write( '	<permission android:name="android.permission.READ_CONTACTS" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.READ_CONTACTS" /> \n')
		if True==target.check_right_package(module.package_prop, "READ_FRAME_BUFFER"):
			tmpFile.write( '	<permission android:name="android.permission.READ_FRAME_BUFFER" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.READ_FRAME_BUFFER" /> \n')
		if True==target.check_right_package(module.package_prop, "READ_PROFILE"):
			tmpFile.write( '	<permission android:name="android.permission.READ_PROFILE" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.READ_PROFILE" /> \n')
		if True==target.check_right_package(module.package_prop, "RECORD_AUDIO"):
			tmpFile.write( '	<permission android:name="android.permission.RECORD_AUDIO" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.RECORD_AUDIO" /> \n')
		if True==target.check_right_package(module.package_prop, "SET_ORIENTATION"):
			tmpFile.write( '	<permission android:name="android.permission.SET_ORIENTATION" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.SET_ORIENTATION" /> \n')
		if True==target.check_right_package(module.package_prop, "VIBRATE"):
			tmpFile.write( '	<permission android:name="android.permission.VIBRATE" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.VIBRATE" /> \n')
		if True==target.check_right_package(module.package_prop, "ACCESS_COARSE_LOCATION"):
			tmpFile.write( '	<permission android:name="android.permission.ACCESS_COARSE_LOCATION" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" /> \n')
		if True==target.check_right_package(module.package_prop, "ACCESS_FINE_LOCATION"):
			tmpFile.write( '	<permission android:name="android.permission.ACCESS_FINE_LOCATION" /> \n')
			tmpFile.write( '	<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" /> \n')
		tmpFile.write( '</manifest>\n\n')
		tmpFile.flush()
		tmpFile.close()
		# end generating android manifest
		
		if module.package_prop["ANDROID_APPL_TYPE"]!="APPL":
			#create the Wallpaper sub files : (main element for the application
			debug.print_element("pkg", application_name + "_resource.xml", "<==", "package configurations")
			tools.create_directory_of_file(target.get_build_folder(package_name) + "/res/xml/" + application_name + "_resource.xml")
			tmpFile = open(target.get_build_folder(package_name) + "/res/xml/" + application_name + "_resource.xml", 'w')
			tmpFile.write( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
			tmpFile.write( "<wallpaper xmlns:android=\"http://schemas.android.com/apk/res/android\"\n")
			if len(module.package_prop["ANDROID_WALLPAPER_PROPERTIES"])!=0:
				tmpFile.write( "           android:settingsActivity=\""+android_package_name + "."+ application_name + "Settings\"\n")
			if "ICON" in module.package_prop.keys():
				tmpFile.write( "           android:thumbnail=\"@drawable/icon\"\n")
			tmpFile.write( "           />\n")
			tmpFile.flush()
			tmpFile.close()
			# create wallpaper setting if needed (class and config file)
			if len(module.package_prop["ANDROID_WALLPAPER_PROPERTIES"])!=0:
				tools.create_directory_of_file(target.folder_java_project + application_name + "Settings.java")
				debug.print_element("pkg", target.folder_java_project + application_name + "Settings.java", "<==", "package configurations")
				tmpFile = open(target.folder_java_project + application_name + "Settings.java", 'w');
				tmpFile.write( "package " + android_package_name + ";\n")
				tmpFile.write( "\n")
				tmpFile.write( "import " + android_package_name + ".R;\n")
				tmpFile.write( "\n")
				tmpFile.write( "import android.content.SharedPreferences;\n")
				tmpFile.write( "import android.os.Bundle;\n")
				tmpFile.write( "import android.preference.PreferenceActivity;\n")
				tmpFile.write( "\n")
				tmpFile.write( "public class " + application_name + "Settings extends PreferenceActivity implements SharedPreferences.OnSharedPreferenceChangeListener\n")
				tmpFile.write( "{\n")
				tmpFile.write( "	@Override protected void onCreate(Bundle icicle) {\n")
				tmpFile.write( "		super.onCreate(icicle);\n")
				tmpFile.write( "		getPreferenceManager().setSharedPreferencesName("+ application_name + ".SHARED_PREFS_NAME);\n")
				tmpFile.write( "		addPreferencesFromResource(R.xml."+ application_name  + "_settings);\n")
				tmpFile.write( "		getPreferenceManager().getSharedPreferences().registerOnSharedPreferenceChangeListener(this);\n")
				tmpFile.write( "	}\n")
				tmpFile.write( "	@Override protected void onResume() {\n")
				tmpFile.write( "		super.onResume();\n")
				tmpFile.write( "	}\n")
				tmpFile.write( "	@Override protected void onDestroy() {\n")
				tmpFile.write( "		getPreferenceManager().getSharedPreferences().unregisterOnSharedPreferenceChangeListener(this);\n")
				tmpFile.write( "		super.onDestroy();\n")
				tmpFile.write( "	}\n")
				tmpFile.write( "	public void onSharedPreferenceChanged(SharedPreferences sharedPreferences,String key) { }\n")
				tmpFile.write( "}\n")
				tmpFile.flush()
				tmpFile.close()
				
				debug.print_element("pkg", target.get_build_folder(package_name) + "/res/xml/" + application_name + "_settings.xml", "<==", "package configurations")
				tools.create_directory_of_file(target.get_build_folder(package_name) + "/res/xml/" + application_name + "_settings.xml")
				tmpFile = open(target.get_build_folder(package_name) + "/res/xml/" + application_name + "_settings.xml", 'w');
				tmpFile.write( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
				tmpFile.write( "<PreferenceScreen xmlns:android=\"http://schemas.android.com/apk/res/android\"\n")
				tmpFile.write( "                  android:title=\"Settings\"\n")
				tmpFile.write( "                  android:key=\"" + application_name  + "_settings\">\n")
				WALL_haveArray = False
				for WALL_type, WALL_key, WALL_title, WALL_summary, WALL_other in module.package_prop["ANDROID_WALLPAPER_PROPERTIES"]:
					debug.info("find : '" + WALL_type + "'");
					if WALL_type == "list":
						debug.info("    create : LIST");
						tmpFile.write( "	<ListPreference android:key=\"" + application_name + "_" + WALL_key + "\"\n")
						tmpFile.write( "	                android:title=\"" + WALL_title + "\"\n")
						tmpFile.write( "	                android:summary=\"" + WALL_summary + "\"\n")
						tmpFile.write( "	                android:entries=\"@array/" + application_name + "_" + WALL_key + "_names\"\n")
						tmpFile.write( "	                android:entryValues=\"@array/" + application_name + "_" + WALL_key + "_prefix\"/>\n")
						WALL_haveArray=True
					elif WALL_type == "bool":
						debug.info("    create : CHECKBOX");
						tmpFile.write( "	<CheckBoxPreference android:key=\"" + application_name + "_" + WALL_key + "\"\n")
						tmpFile.write( "	                    android:title=\"" + WALL_title + "\"\n")
						tmpFile.write( "	                    android:summary=\"" + WALL_summary + "\"\n")
						tmpFile.write( "	                    android:summaryOn=\"" + WALL_other[0] + "\"\n")
						tmpFile.write( "	                    android:summaryOff=\"" + WALL_other[1] + "\"/>\n")
				tmpFile.write( "</PreferenceScreen>\n")
				tmpFile.flush()
				tmpFile.close()
				if WALL_haveArray==True:
					for WALL_type, WALL_key, WALL_title, WALL_summary, WALL_other in module.package_prop["ANDROID_WALLPAPER_PROPERTIES"]:
						if WALL_type == "list":
							debug.print_element("pkg", target.get_build_folder(package_name) + "/res/values/" + WALL_key + ".xml", "<==", "package configurations")
							tools.create_directory_of_file(target.get_build_folder(package_name) + "/res/values/" + WALL_key + ".xml")
							tmpFile = open(target.get_build_folder(package_name) + "/res/values/" + WALL_key + ".xml", 'w');
							tmpFile.write( "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n")
							tmpFile.write( "<resources xmlns:xliff=\"urn:oasis:names:tc:xliff:document:1.2\">\n")
							tmpFile.write( "	<string-array name=\"" + application_name + "_" + WALL_key + "_names\">\n")
							for WALL_subKey, WALL_display in WALL_other:
								tmpFile.write( "		<item>" + WALL_display + "</item>\n")
							tmpFile.write( "	</string-array>\n")
							tmpFile.write( "	<string-array name=\"" + application_name + "_" + WALL_key + "_prefix\">\n")
							for WALL_subKey, WALL_display in WALL_other:
								tmpFile.write( "		<item>" + WALL_subKey + "</item>\n")
							tmpFile.write( "	</string-array>\n")
							tmpFile.write( "</resources>\n")
							tmpFile.flush()
							tmpFile.close()
		
	
	"""
	#add properties on wallpaper : 
	# myModule.pkg_add("ANDROID_WALLPAPER_PROPERTIES", ["list", key, title, summary, [["key","value display"],["key2","value display 2"]])
	# myModule.pkg_add("ANDROID_WALLPAPER_PROPERTIES", ["list", "testpattern", "Select test pattern", "Choose which test pattern to display", [["key","value display"],["key2","value display 2"]]])
	# myModule.pkg_add("ANDROID_WALLPAPER_PROPERTIES", ["bool", key, title, summary, ["enable string", "disable String"])
	# myModule.pkg_add("ANDROID_WALLPAPER_PROPERTIES", ["bool", "movement", "Motion", "Apply movement to test pattern", ["Moving test pattern", "Still test pattern"]
	#copy needed resources :
	for res_source, res_dest in module.package_prop["ANDROID_RESOURCES"]:
		if res_source == "":
			continue
		tools.copy_file(res_source , target.get_staging_folder(package_name) + "/res/" + res_dest + "/" + os.path.basename(res_source), force=True)
	"""
	"""
	# Doc :
	# http://asantoso.wordpress.com/2009/09/15/how-to-build-android-application-package-apk-from-the-command-line-using-the-sdk-tools-continuously-integrated-using-cruisecontrol/
	debug.print_element("pkg", "R.java", "<==", "Resources files")
	tools.create_directory_of_file(target.get_staging_folder(package_name) + "/src/noFile")
	androidToolPath = target.folder_sdk + "/build-tools/"
	# find android tool version
	dirnames = tools.get_list_sub_folder(androidToolPath)
	if len(dirnames) != 1:
		debug.error("an error occured when getting the tools for android")
	androidToolPath += dirnames[0] + "/"
	
	adModResouceFolder = ""
	if "ADMOD_ID" in module.package_prop:
		adModResouceFolder = " -S " + target.folder_sdk + "/extras/google/google_play_services/libproject/google-play-services_lib/res/ "
	cmdLine = androidToolPath + "aapt p -f " \
	          + "-M " + target.get_staging_folder(package_name) + "/AndroidManifest.xml " \
	          + "-F " + target.get_staging_folder(package_name) + "/resources.res " \
	          + "-I " + target.folder_sdk + "/platforms/android-" + str(target.boardId) + "/android.jar "\
	          + "-S " + target.get_staging_folder(package_name) + "/res/ " \
	          + adModResouceFolder \
	          + "-J " + target.get_staging_folder(package_name) + "/src/ "
	multiprocess.run_command(cmdLine)
	#aapt  package -f -M ${manifest.file} -F ${packaged.resource.file} -I ${path.to.android-jar.library} 
	#      -S ${android-resource-directory} [-m -J ${folder.to.output.the.R.java}]
	
	tools.create_directory_of_file(target.get_staging_folder(package_name) + "/build/classes/noFile")
	debug.print_element("pkg", "*.class", "<==", "*.java")
	# more information with : -Xlint
	#          + java_file_wrapper + " "\ # this generate ex: out/Android/debug/staging/tethys/src/com/edouarddupin/tethys/edn.java
	
	#generate android java files:
	filesString=""
	for element in module.package_prop["ANDROID_JAVA_FILES"]:
		if element=="DEFAULT":
			filesString += target.folder_ewol + "/android/src/org/ewol/EwolAudioTask.java "
			filesString += target.folder_ewol + "/android/src/org/ewol/EwolCallback.java "
			filesString += target.folder_ewol + "/android/src/org/ewol/EwolConstants.java "
			filesString += target.folder_ewol + "/android/src/org/ewol/Ewol.java "
			filesString += target.folder_ewol + "/android/src/org/ewol/EwolRendererGL.java "
			filesString += target.folder_ewol + "/android/src/org/ewol/EwolSurfaceViewGL.java "
			filesString += target.folder_ewol + "/android/src/org/ewol/EwolActivity.java "
			filesString += target.folder_ewol + "/android/src/org/ewol/EwolWallpaper.java "
		else:
			filesString += element + " "
	
	if "ADMOD_ID" in module.package_prop:
		filesString += target.folder_sdk + "/extras/google/google_play_services/libproject/google-play-services_lib/src/android/UnusedStub.java "
		
	if len(module.package_prop["ANDROID_WALLPAPER_PROPERTIES"])!=0:
		filesString += target.folder_java_project + application_name + "Settings.java "
	
	adModJarFile = ""
	if "ADMOD_ID" in module.package_prop:
		adModJarFile = ":" + target.folder_sdk + "/extras/google/google_play_services/libproject/google-play-services_lib/libs/google-play-services.jar"
	
	cmdLine = "javac " \
	          + "-d " + target.get_staging_folder(package_name) + "/build/classes " \
	          + "-classpath " + target.folder_sdk + "/platforms/android-" + str(target.boardId) + "/android.jar" \
	          + adModJarFile + " " \
	          + filesString \
	          + java_file_wrapper + " "  \
	          + target.get_staging_folder(package_name) + "/src/R.java "
	multiprocess.run_command(cmdLine)
	
	debug.print_element("pkg", ".dex", "<==", "*.class")
	cmdLine = androidToolPath + "dx " \
	          + "--dex --no-strict " \
	          + "--output=" + target.get_staging_folder(package_name) + "/build/" + application_name + ".dex " \
	          + target.get_staging_folder(package_name) + "/build/classes/ "
	
	if "ADMOD_ID" in module.package_prop:
		cmdLine += target.folder_sdk + "/extras/google/google_play_services/libproject/google-play-services_lib/libs/google-play-services.jar "
	multiprocess.run_command(cmdLine)
	"""
	return {"files":file_list}



