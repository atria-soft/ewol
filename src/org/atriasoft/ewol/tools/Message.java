/*
 * @author Edouard DUPIN
 * @file
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package  org.atriasoft.ewol.tools;

import org.atriasoft.ewol.Ewol;

public class Message {

    /**
     * @brief Create a simple pop-up message on the screen for application error.
     * @param[in] _type Type of the error.
     * @param[in] _message message to display (decorated text)
     */
    private static void create(Type _type, String _message){
		StdPopUp tmpPopUp = new StdPopUp();
		switch (_type) {
			case Type.info -> tmpPopUp.propertyTitle.set("<bold>_T{Info}</bold>");
			case Type.warning -> tmpPopUp.propertyTitle.set("<bold><font color='orange'>_T{Warning}</font></bold>");
			case Type.error -> tmpPopUp.propertyTitle.set("<bold><font color='red'>_T{Error}</font></bold>");
			case Type.critical -> tmpPopUp.propertyTitle.set("<bold><font colorBg='red'>_T{Critical}</font></bold>");
		}
		tmpPopUp.propertyComment.set(_message);
		tmpPopUp.addButton("_T{close}", true);
		tmpPopUp.propertyCloseOutEvent.set(true);
		// get windows:
		EwolContext context = Ewol.getContext();
		Windows windows = context.getWindows();
		if (windows == null) {
			Log.error("can not get the current windows ... ==> can not display message : " + _message);
			return;
		}
		windows.popUpWidgetPush(tmpPopUp);
	}

	/**
     * @brief Create a simple information message
     * @param[in] _message message to display (decorated text)
     */
    public static void displayInfo(String _message){
		create(Type.info, _message);
	}

    /**
     * @brief Create a simple warning message
     * @param[in] _message message to display (decorated text)
     */
    public static void displayWarning(String _message) {
		create(Type.warning, _message);
	}

    /**
     * @brief Create a simple error message
     * @param[in] _message message to display (decorated text)
     */
    public static void displayError(String _message) {
		create(Type.error,_message);
	}
    /**
     * @brief Create a simple critical message
     * @param[in] _message message to display (decorated text)
     */
    public static void displayCritical(String _message){
		create(Type.critical, _message);
	}

    private enum Type {
        info, //!< information message pop-up
        warning, //!< warning message pop-up
        error, //!< Error message pop-up
        critical //!< Critical message pop-up
    }


}

