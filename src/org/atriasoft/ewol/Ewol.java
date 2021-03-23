package org.atriasoft.ewol;

import org.atriasoft.etk.Uri;
/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
import org.atriasoft.ewol.context.EwolApplication;
import org.atriasoft.ewol.context.EwolContext;

public class Ewol {
	static {
		Uri.addLibrary("ewol", Ewol.class);
	}
	
	public static EwolContext getContext() {
		// TODO Auto-generated method stub
		return EwolContext.getContext();
	}
	
	/**
	 * This is the only one things the User might done in his main();
	 * @note To answare you before you ask the question, this is really simple:
	 *       Due to the fect that the current system is multiple-platform, you "main"
	 *       Does not exist in the android platform, then ewol call other start 
	 *       and stop function, to permit to have only one code
	 * @note The main can not be in the ewol, due to the fact thet is an librairy
	 * @param _application just created instance of the applicationo
	 * @param _argc Standard argc
	 * @param _argv Standard argv
	 * @return normal error int for the application error management
	 */
	public static int run(final EwolApplication _application, String[] _argv);
}
