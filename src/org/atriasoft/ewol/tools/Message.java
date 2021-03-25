/*
 * @author Edouard DUPIN
 * @file
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package org.atriasoft.ewol.tools;

import org.atriasoft.ewol.internal.Log;

public class Message {
	private enum Type {
		critical, //!< Critical message pop-up, //!< information message pop-up
		error, //!< warning message pop-up
		info, //!< Error message pop-up
		warning
	}
	
	/**
	 * Create a simple pop-up message on the screen for application error.
	 * @param type Type of the error.
	 * @param message message to display (decorated text)
	 */
	private static void create(final Type type, final String message) {
		Log.todo("Generic message display (simple interface...)");
		/*
		StdPopUp tmpPopUp = new StdPopUp();
		switch (type) {
			case Type.info -> tmpPopUp.propertyTitle.set("<bold>T{Info}</bold>");
			case Type.warning -> tmpPopUp.propertyTitle.set("<bold><font color='orange'>T{Warning}</font></bold>");
			case Type.error -> tmpPopUp.propertyTitle.set("<bold><font color='red'>T{Error}</font></bold>");
			case Type.critical -> tmpPopUp.propertyTitle.set("<bold><font colorBg='red'>T{Critical}</font></bold>");
		}
		tmpPopUp.propertyComment.set(message);
		tmpPopUp.addButton("T{close}", true);
		tmpPopUp.propertyCloseOutEvent.set(true);
		// get windows:
		EwolContext context = Ewol.getContext();
		Windows windows = context.getWindows();
		if (windows == null) {
			Log.error("can not get the current windows ... ==> can not display message : " + message);
			return;
		}
		windows.popUpWidgetPush(tmpPopUp);
		*/
	}
	
	/**
	 * Create a simple critical message
	 * @param message message to display (decorated text)
	 */
	public static void displayCritical(final String message) {
		Message.create(Type.critical, message);
	}
	
	/**
	 * Create a simple error message
	 * @param message message to display (decorated text)
	 */
	public static void displayError(final String message) {
		Message.create(Type.error, message);
	}
	
	/**
	 * Create a simple information message
	 * @param message message to display (decorated text)
	 */
	public static void displayInfo(final String message) {
		Message.create(Type.info, message);
	}
	
	/**
	 * Create a simple warning message
	 * @param message message to display (decorated text)
	 */
	public static void displayWarning(final String message) {
		Message.create(Type.warning, message);
	}
	
	private Message() {}
	
}
