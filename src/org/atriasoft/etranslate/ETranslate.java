package org.atriasoft.etranslate;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;

import org.atriasoft.ejson.Ejson;
import org.atriasoft.ejson.model.JsonNode;
import org.atriasoft.ejson.model.JsonObject;
import org.atriasoft.etk.Uri;
import org.atriasoft.ewol.internal.Log;

/**
 * This is a simple interface to converte application display string in a
 *        generic current system language
 * @note: The current name of language reprenent the file name, then if you want
 *        to get the machine language in an other than generic passed, juste add
 *        it. Generic langage char: (all translation might be done in UTF-8 this
 *        simplify interface) English : "EN" French : "FR" German : "DE" Spanish
 *        : "SP" Japanese : "JA" Italian : "IT" Korean : "KO" Russian : "RU"
 *        Portuguese, Brazilian : "PT" Chinese : "ZH"
 */
public class ETranslate {
	private static boolean globalIsInit = false;
	private static String globalLanguage = "";
	private static String globalLanguageDefault = "EN";
	private static Map<String, Uri> globalListPath = new HashMap<>();
	private static String globalMajor = "etranslate";
	private static Map<String, String> globalTranslate = new HashMap<>();
	private static boolean globalTranslateLoadad = false;
	
	/**
	 * Initialize etranslate
	 * @param argc Number of argument list
	 * @param argv List of arguments
	 */
	static {}
	
	/**
	 * Set the path folder of the translation files
	 * @param lib Library name that the path depend
	 * @param uri ETK generic uri (DATA:... or /xxx)
	 * @param major This path is the major path (The last loaded, the one which
	 *            overload all)
	 */
	public static void addPath(final String lib, final Uri uri) {
		ETranslate.addPath(lib, uri, false);
	}
	
	public static void addPath(final String lib, final Uri uri, final boolean major) {
		ETranslate.globalListPath.put(lib, uri);
		if (major) {
			ETranslate.globalMajor = lib;
			Log.info("Change major translation : '" + ETranslate.globalMajor + "'");
		}
		ETranslate.globalTranslateLoadad = false;
		ETranslate.globalTranslate.clear();
	}
	
	/**
	 * Automatic detection of the system language
	 */
	public static void autoDetectLanguage() {
		if (!ETranslate.globalIsInit) {
			Log.error("E-translate system has not been init");
		}
		Log.verbose("Auto-detect language of system");
		final String nonameLocalName = "EN";
		final String userLocalName = "EN";
		final String globalLocalName = "EN";
		/*
		 * try { nonameLocalName = setlocale(LC_ALL, ""); userLocalName =
		 * setlocale(LC_MESSAGES, ""); globalLocalName = setlocale(LC_CTYPE, "");
		 * Log.error("    The default locale is '" + globalLocalName + "'");
		 * Log.error("    The user's locale is '" + userLocalName + "'");
		 * Log.error("    A nameless locale is '" + nonameLocalName + "'"); } catch (int
		 * e) {
		 * // TODO Do it better RuntimeError e) {
		 * Log.error("Can not get Locals ==> set English ..."); }
		 */
		Log.error("Can not get Locals ==> set English ...");
		
		String lang = nonameLocalName;
		if (lang.equals("*") || lang.isEmpty()) {
			lang = userLocalName;
		}
		if (lang.equals("*") || lang.isEmpty()) {
			lang = globalLocalName;
		}
		if (lang.equals("C") || lang.isEmpty() || lang.length() < 2) {
			lang = "EN";
		}
		lang = lang.substring(0, 2);
		lang = lang.toUpperCase();
		Log.info("Select Language : '" + lang + "'");
		ETranslate.setLanguage(lang);
	}
	
	/**
	 * Translate a specific text (if not find, it will be retured the same
	 *        text).
	 * @param instance Text to translate.
	 * @return The tranlated text.
	 */
	public static String get(final String instance) {
		ETranslate.loadTranslation();
		Log.verbose("Request translate: '" + instance + "'");
		// find all iterance of 'T{' ... '}'
		final String out = Pattern.compile("T\\{.*\\}").matcher(instance).replaceAll(mr -> {
			final String data = mr.group();
			Log.info("translate : '" + data + "'");
			final String itTranslate = ETranslate.globalTranslate.get(data);
			if (itTranslate == null) {
				Log.debug("Can not find tranlation : '" + instance + "'");
				return data;
			}
			return itTranslate;
		});
		return out;
	}
	
	/**
	 * Get the current language loaded
	 * @return The 2/3 char defining the language
	 */
	public static String getLanguage() {
		return ETranslate.globalLanguage;
	}
	
	/**
	 * Get the current language selected
	 * @return The 2/3 char defining the language
	 */
	public static String getLanguageDefault() {
		return ETranslate.globalLanguageDefault;
	}
	
	/**
	 * Get the current paths of the library
	 * @param lib Library name that the path depend
	 * @return Uri value.
	 */
	public static Uri getPaths(final String lib) {
		return ETranslate.globalListPath.get(lib);
	}
	
	private static void loadTranslation() {
		if (ETranslate.globalTranslateLoadad) {
			return;
		}
		Log.debug("Load Translation MAJOR='" + ETranslate.globalMajor + "' LANG='" + ETranslate.globalLanguage + "' default=" + ETranslate.globalLanguageDefault);
		Log.debug("list path=" + ETranslate.globalListPath.keySet());
		// start parse language for Major:
		final Uri itMajor = ETranslate.globalListPath.get(ETranslate.globalMajor);
		if (itMajor != null) {
			Uri uri = itMajor.withPath(itMajor.getPath() + "/" + ETranslate.globalLanguage + ".json");
			try {
				final JsonObject root = (JsonObject) Ejson.parse(uri);
				for (final Map.Entry<String, JsonNode> element : root.getNodes().entrySet()) {
					final String val = element.getValue().toJsonString().getValue();
					//Log.info("Add global translate: '" + element.getKey() + "' => '" + val + "'");
					ETranslate.globalTranslate.put(element.getKey(), val);
				}
			} catch (final Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			uri = itMajor.withPath(itMajor.getPath() + "/" + ETranslate.globalLanguageDefault + ".json");
			try {
				final JsonObject root = (JsonObject) Ejson.parse(uri);
				for (final Map.Entry<String, JsonNode> element : root.getNodes().entrySet()) {
					final String val = element.getValue().toJsonString().getValue();
					//Log.info("Add global translate: '" + element.getKey() + "' => '" + val + "'");
					ETranslate.globalTranslate.put(element.getKey(), val);
				}
			} catch (final Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		// start parse language:
		for (final Map.Entry<String, Uri> it : ETranslate.globalListPath.entrySet()) {
			if (it.getKey().contentEquals(ETranslate.globalMajor)) {
				continue;
			}
			final Uri uri = it.getValue().withPath(it.getValue().getPath() + "/" + ETranslate.globalLanguage + ".json");
			/*
			 * TODO ... if (Uri.exist(uri) == false) { continue; }
			 */
			JsonObject doc;
			try {
				doc = (JsonObject) Ejson.parse(uri);
			} catch (final Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				continue;
			}
			for (final Map.Entry<String, JsonNode> element : doc.getNodes().entrySet()) {
				final String val = element.getValue().toJsonString().getValue();
				//Log.info("Add global translate: '" + element.getKey() + "' => '" + val + "'");
				ETranslate.globalTranslate.put(element.getKey(), val);
			}
		}
		// start parse default language:
		for (final Map.Entry<String, Uri> it : ETranslate.globalListPath.entrySet()) {
			if (it.getKey().contentEquals(ETranslate.globalMajor)) {
				continue;
			}
			final Uri uri = it.getValue().withPath(it.getValue().getPath() + "/" + ETranslate.globalLanguageDefault + ".json");
			/*
			 * TODO ... if (Uri.exist(uri) == false) { continue; }
			 */
			JsonObject doc;
			try {
				doc = (JsonObject) Ejson.parse(uri);
			} catch (final Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				continue;
			}
			for (final Map.Entry<String, JsonNode> element : doc.getNodes().entrySet()) {
				final String val = element.getValue().toJsonString().getValue();
				//Log.info("Add global translate: '" + element.getKey() + "' => '" + val + "'");
				ETranslate.globalTranslate.put(element.getKey(), val);
			}
		}
		ETranslate.globalTranslateLoadad = true;
	}
	
	/**
	 * Set the language to load data. when no data availlable, we get the
	 *        default language.
	 * @param lang Language to load : ("EN" for english, "FR" for french, "DE"
	 *            for German, "SP" for spanish ...)
	 */
	public static void setLanguage(final String lang) {
		if (ETranslate.globalLanguage.equals(lang)) {
			return;
		}
		ETranslate.globalLanguage = lang;
		ETranslate.globalTranslateLoadad = false;
		ETranslate.globalTranslate.clear();
		if (lang.equals("EN")) {
			Log.info("Change language translation: '" + lang + "'=English");
		} else if (lang.equals("FR")) {
			Log.info("Change language translation: '" + lang + "'=French");
		} else if (lang.equals("DE")) {
			Log.info("Change language translation: '" + lang + "'=German");
		} else if (lang.equals("SP")) {
			Log.info("Change language translation: '" + lang + "'=Spanish");
		} else if (lang.equals("JA")) {
			Log.info("Change language translation: '" + lang + "'=Japanese");
		} else if (lang.equals("IT")) {
			Log.info("Change language translation: '" + lang + "'=Italian");
		} else if (lang.equals("KO")) {
			Log.info("Change language translation: '" + lang + "'=Korean");
		} else if (lang.equals("RU")) {
			Log.info("Change language translation: '" + lang + "'=Russian");
		} else if (lang.equals("PT")) {
			Log.info("Change language translation: '" + lang + "'=Portuguese, Brazilian");
		} else if (lang.equals("ZH")) {
			Log.info("Change language translation: '" + lang + "'=Chinese");
		} else {
			Log.info("Change language translation: '" + lang + "'=Unknow");
		}
	}
	
	/**
	 * Set the default language to load data (the default language might
	 *        contain all internal data for the basic application)
	 * @param lang Language to load : ("EN" for english, "FR" for french, "DE"
	 *            for German, "SP" for spanish ...)
	 */
	public static void setLanguageDefault(final String lang) {
		if (ETranslate.globalLanguageDefault.equals(lang)) {
			return;
		}
		Log.info("Change default language translation : '" + lang + "'");
		ETranslate.globalLanguageDefault = lang;
		ETranslate.globalTranslateLoadad = false;
		ETranslate.globalTranslate.clear();
	}
	
	private ETranslate() {}
}
