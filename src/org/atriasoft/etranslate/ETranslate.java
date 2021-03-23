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
	private static Map<String, Uri> m_listPath = new HashMap<>();
	private static String m_major = "etranslate";
	private static String m_languageDefault = "EN";
	private static String m_language = "";
	private static boolean m_translateLoadad = false;
	private static Map<String, String> m_translate = new HashMap<>();
	private static boolean g_isInit = false;

	/**
	 * Initialize etranslate
	 * @param _argc Number of argument list
	 * @param _argv List of arguments
	 */
	static {

	}

	/**
	 * Set the path folder of the translation files
	 * @param _lib Library name that the path depend
	 * @param _uri ETK generic uri (DATA:... or /xxx)
	 * @param _major This path is the major path (The last loaded, the one which
	 *            overload all)
	 */
	public static void addPath(final String _lib, final Uri _uri) {
		addPath(_lib, _uri, false);
	}

	public static void addPath(final String _lib, final Uri _uri, final boolean _major) {
		m_listPath.put(_lib, _uri);
		if (_major == true) {
			m_major = _lib;
			Log.info("Change major translation : '" + m_major + "'");
		}
		m_translateLoadad = false;
		m_translate.clear();
	}

	/**
	 * Automatic detection of the system language
	 */
	public static void autoDetectLanguage() {
		if (g_isInit == false) {
			Log.error("E-translate system has not been init");
		}
		Log.verbose("Auto-detect language of system");
		String nonameLocalName = "EN";
		String userLocalName = "EN";
		String globalLocalName = "EN";
		/*
		 * try { nonameLocalName = setlocale(LC_ALL, ""); userLocalName =
		 * setlocale(LC_MESSAGES, ""); globalLocalName = setlocale(LC_CTYPE, "");
		 * Log.error("    The default locale is '" + globalLocalName + "'");
		 * Log.error("    The user's locale is '" + userLocalName + "'");
		 * Log.error("    A nameless locale is '" + nonameLocalName + "'"); } catch (int
		 * e) { // TODO: Do it better RuntimeError e) {
		 * Log.error("Can not get Locals ==> set English ..."); }
		 */
		Log.error("Can not get Locals ==> set English ...");

		String lang = nonameLocalName;
		if (lang == "*" || lang == "") {
			lang = userLocalName;
		}
		if (lang == "*" || lang == "") {
			lang = globalLocalName;
		}
		if (lang == "C" || lang == "" || lang.length() < 2) {
			lang = "EN";
		}
		lang = lang.substring(0, 2);
		lang = lang.toUpperCase();
		Log.info("Select Language : '" + lang + "'");
		setLanguage(lang);
	}

	/**
	 * Translate a specific text (if not find, it will be retured the same
	 *        text).
	 * @param _instance Text to translate.
	 * @return The tranlated text.
	 */
	public static String get(final String _instance) {
		loadTranslation();
		Log.verbose("Request translate: '" + _instance + "'");
		// find all iterance of '_T{' ... '}'
		String out = Pattern.compile("_T\\{.*\\}").matcher(_instance).replaceAll(mr -> {
			String data = mr.group();
			Log.info("translate : '" + data + "'");
			String itTranslate = m_translate.get(data);
			if (itTranslate == null) {
				Log.debug("Can not find tranlation : '" + _instance + "'");
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
		return m_language;
	}

	/**
	 * Get the current language selected
	 * @return The 2/3 char defining the language
	 */
	public static String getLanguageDefault() {
		return m_languageDefault;
	}

	/**
	 * Get the current paths of the library
	 * @param _lib Library name that the path depend
	 * @return Uri value.
	 */
	public static Uri getPaths(final String _lib) {
		return m_listPath.get(_lib);
	}

	private static void loadTranslation() {
		if (m_translateLoadad == true) {
			return;
		}
		Log.debug("Load Translation MAJOR='" + m_major + "' LANG='" + m_language + "' default=" + m_languageDefault);
		Log.debug("list path=" + m_listPath.keySet());
		// start parse language for Major:
		Uri itMajor = m_listPath.get(m_major);
		if (itMajor != null) {
			Uri uri = itMajor.withPath(itMajor.getPath() + "/" + m_language + ".json");
			try {
				JsonObject root = (JsonObject) Ejson.parse(uri);
				for (Map.Entry<String, JsonNode> element : root.getNodes().entrySet()) {
					String val = element.getValue().toJsonString().getValue();
					m_translate.put(element.getKey(), val);
				}
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			uri = itMajor.withPath(itMajor.getPath() + "/" + m_languageDefault + ".json");
			try {
				JsonObject root = (JsonObject) Ejson.parse(uri);
				for (Map.Entry<String, JsonNode> element : root.getNodes().entrySet()) {
					String val = element.getValue().toJsonString().getValue();
					m_translate.put(element.getKey(), val);
				}
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		// start parse language:
		for (Map.Entry<String, Uri> it : m_listPath.entrySet()) {
			if (it.getKey().contentEquals(m_major)) {
				continue;
			}
			Uri uri = it.getValue().withPath(it.getValue().getPath() + "/" + m_language + ".json");
			/*
			 * TODO ... if (Uri.exist(uri) == false) { continue; }
			 */
			JsonObject doc;
			try {
				doc = (JsonObject) Ejson.parse(uri);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				continue;
			}
			for (Map.Entry<String, JsonNode> element : doc.getNodes().entrySet()) {
				String val = element.getValue().toJsonString().getValue();
				m_translate.put(element.getKey(), val);
			}
		}
		// start parse default language:
		for (Map.Entry<String, Uri> it : m_listPath.entrySet()) {
			if (it.getKey().contentEquals(m_major)) {
				continue;
			}
			Uri uri = it.getValue().withPath(it.getValue().getPath() + "/" + m_languageDefault + ".json");
			/*
			 * TODO ... if (Uri.exist(uri) == false) { continue; }
			 */
			JsonObject doc;
			try {
				doc = (JsonObject) Ejson.parse(uri);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				continue;
			}
			for (Map.Entry<String, JsonNode> element : doc.getNodes().entrySet()) {
				String val = element.getValue().toJsonString().getValue();
				m_translate.put(element.getKey(), val);
			}
		}
		m_translateLoadad = true;
	}

	/**
	 * Set the language to load data. when no data availlable, we get the
	 *        default language.
	 * @param _lang Language to load : ("EN" for english, "FR" for french, "DE"
	 *            for German, "SP" for spanish ...)
	 */
	public static void setLanguage(final String _lang) {
		if (m_language == _lang) {
			return;
		}
		m_language = _lang;
		m_translateLoadad = false;
		m_translate.clear();
		if (_lang == "EN") {
			Log.info("Change language translation: '" + _lang + "'=English");
		} else if (_lang == "FR") {
			Log.info("Change language translation: '" + _lang + "'=French");
		} else if (_lang == "DE") {
			Log.info("Change language translation: '" + _lang + "'=German");
		} else if (_lang == "SP") {
			Log.info("Change language translation: '" + _lang + "'=Spanish");
		} else if (_lang == "JA") {
			Log.info("Change language translation: '" + _lang + "'=Japanese");
		} else if (_lang == "IT") {
			Log.info("Change language translation: '" + _lang + "'=Italian");
		} else if (_lang == "KO") {
			Log.info("Change language translation: '" + _lang + "'=Korean");
		} else if (_lang == "RU") {
			Log.info("Change language translation: '" + _lang + "'=Russian");
		} else if (_lang == "PT") {
			Log.info("Change language translation: '" + _lang + "'=Portuguese, Brazilian");
		} else if (_lang == "ZH") {
			Log.info("Change language translation: '" + _lang + "'=Chinese");
		} else {
			Log.info("Change language translation: '" + _lang + "'=Unknow");
		}
	}

	/**
	 * Set the default language to load data (the default language might
	 *        contain all internal data for the basic application)
	 * @param _lang Language to load : ("EN" for english, "FR" for french, "DE"
	 *            for German, "SP" for spanish ...)
	 */
	public static void setLanguageDefault(final String _lang) {
		if (m_languageDefault == _lang) {
			return;
		}
		Log.info("Change default language translation : '" + _lang + "'");
		m_languageDefault = _lang;
		m_translateLoadad = false;
		m_translate.clear();
	}

	private ETranslate() {
	}
}
