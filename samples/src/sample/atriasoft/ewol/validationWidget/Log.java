/** @file
 * @author Edouard DUPIN
 * @copyright 2021, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
package sample.atriasoft.ewol.validationWidget;

import org.atriasoft.reggol.LogLevel;
import org.atriasoft.reggol.Logger;

public class Log {
	private static final String LIB_NAME = "ejson-test";
	private static final String LIB_NAME_DRAW = Logger.getDrawableName(LIB_NAME);
	private static final boolean PRINT_CRITICAL = Logger.getNeedPrint(LIB_NAME, LogLevel.CRITICAL);
	private static final boolean PRINT_ERROR = Logger.getNeedPrint(LIB_NAME, LogLevel.ERROR);
	private static final boolean PRINT_WARNING = Logger.getNeedPrint(LIB_NAME, LogLevel.WARNING);
	private static final boolean PRINT_INFO = Logger.getNeedPrint(LIB_NAME, LogLevel.INFO);
	private static final boolean PRINT_DEBUG = Logger.getNeedPrint(LIB_NAME, LogLevel.DEBUG);
	private static final boolean PRINT_VERBOSE = Logger.getNeedPrint(LIB_NAME, LogLevel.VERBOSE);
	private static final boolean PRINT_TODO = Logger.getNeedPrint(LIB_NAME, LogLevel.TODO);
	private static final boolean PRINT_PRINT = Logger.getNeedPrint(LIB_NAME, LogLevel.PRINT);
	
	public static void critical(final String data, final Object... objects) {
		if (PRINT_CRITICAL) {
			Logger.critical(LIB_NAME_DRAW, data, objects);
		}
	}
	
	public static void debug(final String data, final Object... objects) {
		if (PRINT_DEBUG) {
			Logger.debug(LIB_NAME_DRAW, data, objects);
		}
	}
	
	public static void error(final String data, final Object... objects) {
		if (PRINT_ERROR) {
			Logger.error(LIB_NAME_DRAW, data, objects);
		}
	}
	
	public static void info(final String data, final Object... objects) {
		if (PRINT_INFO) {
			Logger.info(LIB_NAME_DRAW, data, objects);
		}
	}
	
	public static void print(final String data, final Object... objects) {
		if (PRINT_PRINT) {
			Logger.print(LIB_NAME_DRAW, data, objects);
		}
	}
	
	public static void todo(final String data, final Object... objects) {
		if (PRINT_TODO) {
			Logger.todo(LIB_NAME_DRAW, data, objects);
		}
	}
	
	public static void verbose(final String data, final Object... objects) {
		if (PRINT_VERBOSE) {
			Logger.verbose(LIB_NAME_DRAW, data, objects);
		}
	}
	
	public static void warning(final String data, final Object... objects) {
		if (PRINT_WARNING) {
			Logger.warning(LIB_NAME_DRAW, data, objects);
		}
	}
	
	private Log() {}
	
}
