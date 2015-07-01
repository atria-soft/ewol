/**
 * @author Edouard DUPIN, Kevin BILLONNEAU
 *
 * @copyright 2011, Edouard DUPIN, all right reserved
 *
 * @license APACHE v2.0 (see license file)
 */

package org.ewol;
import android.util.Log;

public interface EwolCallback {
	public void openURI(String _uri);
	public void keyboardUpdate(boolean _show);
	public void eventNotifier(String[] _args);
	public void orientationUpdate(int _screenMode);
	public void titleSet(String _value);
	public void stop();
}
