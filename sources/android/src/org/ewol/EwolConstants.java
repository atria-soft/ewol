/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */

package org.ewol;

public interface EwolConstants {
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
