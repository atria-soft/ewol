/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license BSD v3 (see license file)
 */

package org.ewol;
import android.util.Log;

public interface EwolCallback {
	public void openURI(String uri);
	public void keyboardUpdate(boolean show);
	public void eventNotifier(String[] args);
	public void orientationUpdate(int screenMode);
	public void titleSet(String value);
	public void stop();
	public int audioGetDeviceCount();
	public String audioGetDeviceProperty(int idDevice);
	public boolean audioOpenDevice(int idDevice, int freq, int nbChannel, int format);
	public boolean audioCloseDevice(int idDevice);
}
