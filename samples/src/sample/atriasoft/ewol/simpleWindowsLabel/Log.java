package sample.atriasoft.ewol.simpleWindowsLabel;

public class Log {
	private static final String LIBNAME = "LoxelEngine";
	
	public static void critical(final String data) {
		System.out.println("[C] " + Log.LIBNAME + " | " + data);
	}
	
	public static void debug(final String data) {
		System.out.println("[D] " + Log.LIBNAME + " | " + data);
	}
	
	public static void error(final String data) {
		System.out.println("[E] " + Log.LIBNAME + " | " + data);
	}
	
	public static void info(final String data) {
		System.out.println("[I] " + Log.LIBNAME + " | " + data);
	}
	
	public static void print(final String data) {
		System.out.println(data);
	}
	
	public static void todo(final String data) {
		System.out.println("[TODO] " + Log.LIBNAME + " | " + data);
	}
	
	public static void verbose(final String data) {
		System.out.println("[V] " + Log.LIBNAME + " | " + data);
	}
	
	public static void warning(final String data) {
		System.out.println("[W] " + Log.LIBNAME + " | " + data);
	}
	
	private Log() {}
}
