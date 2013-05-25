/**
*******************************************************************************
* @file ewol EwolConstants.java
* @brief Java Contants interface.
* @author Edouard DUPIN
* @date 29/06/2012
* @par Project
* ewol
*
* @par Copyright
* Copyright 2011 Edouard DUPIN, all right reserved
*
* This software is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY.
*
* Licence summary : 
*    You can modify and redistribute the sources code and binaries.
*    You can send me the bug-fix
*
* Term of the licence in in the file licence.txt.
*
*******************************************************************************
*/

package org.ewol;



public interface EwolConstants {
    public static final int  SDK_VERSION                      = android.os.Build.VERSION.SDK_INT;
    public static final int  NATIVE_AUDIO_BUFFER_SIZE         = 512;
    public static final int  EWOL_SYSTEM_KEY_VOLUME_UP        = 1;
    public static final int  EWOL_SYSTEM_KEY_VOLUME_DOWN      = 2;
    public static final int  EWOL_SYSTEM_KEY_MENU             = 3;
    public static final int  EWOL_SYSTEM_KEY_CAMERA           = 4;
    public static final int  EWOL_SYSTEM_KEY_HOME             = 5;
    public static final int  EWOL_SYSTEM_KEY_POWER            = 6;
    // the back key is wrapped in the <esc> key to simplify PC validation ...
    public static final int  EWOL_SYSTEM_KEY_BACK             = 0x1B;
    public static final int  EWOL_SYSTEM_KEY_DEL              = 0x08;

    public static final int  EWOL_ORIENTATION_AUTO            = 0;
    public static final int  EWOL_ORIENTATION_LANDSCAPE       = 1;
    public static final int  EWOL_ORIENTATION_PORTRAIT        = 2;
    
}
