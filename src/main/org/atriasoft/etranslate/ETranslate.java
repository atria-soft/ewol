package org.atriasoft.etranslate;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Pattern;

import java.util.Iterator;

import com.fasterxml.jackson.databind.JsonNode;

import org.atriasoft.ewol.internal.JsonHelper;
import org.atriasoft.etk.Uri;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

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
	private static final Logger LOGGER = LoggerFactory.getLogger(ETranslate.class);
	private static String globalLanguage = "";
	private static String globalLanguageDefault = "EN";
	private static Map<String, Uri> globalListPath = new HashMap<>();
	private static String globalMajor = "etranslate";
	private static Map<String, String> globalTranslate = new HashMap<>();
	private static boolean globalTranslateLoadad = false;
	
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
			LOGGER.info("Change major translation : '{}'", ETranslate.globalMajor);
		}
		ETranslate.globalTranslateLoadad = false;
		ETranslate.globalTranslate.clear();
	}
	
	/**
	 * Automatic detection of the system language
	 */
	public static void autoDetectLanguage() {
		LOGGER.trace("Auto-detect language of system");
		final String nonameLocalName = "EN";
		final String userLocalName = "EN";
		final String globalLocalName = "EN";
		/*
		 * try { nonameLocalName = setlocale(LC_ALL, ""); userLocalName =
		 * setlocale(LC_MESSAGES, ""); globalLocalName = setlocale(LC_CTYPE, "");
		 * LOGGER.error("    The default locale is '" + globalLocalName + "'");
		 * LOGGER.error("    The user's locale is '" + userLocalName + "'");
		 * LOGGER.error("    A nameless locale is '" + nonameLocalName + "'"); } catch (int
		 * e) {
		 * // TODO Do it better RuntimeError e) {
		 * LOGGER.error("Can not get Locals ==> set English ..."); }
		 */
		LOGGER.debug("Detection of the system locale is not implemented: use English");
		
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
		LOGGER.info("Select Language : '{}'", lang);
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
		LOGGER.trace("Request translate: '{}'", instance);
		// find all iterance of 'T{' ... '}'
		final String out = Pattern.compile("_T\\{(.*)\\}").matcher(instance).replaceAll(mr -> {
			final String data = mr.group(1);
			LOGGER.info("translate : '{}'", data);
			final String itTranslate = ETranslate.globalTranslate.get(data);
			if (itTranslate == null) {
				LOGGER.debug("Can not find tranlation : '{}'", instance);
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
	
	private static void loadTranslationFile(final Uri uri) {
		try {
			final JsonNode root = JsonHelper.parse(uri);
			final Iterator<Map.Entry<String, JsonNode>> fields = root.fields();
			while (fields.hasNext()) {
				final Map.Entry<String, JsonNode> element = fields.next();
				ETranslate.globalTranslate.put(element.getKey(), element.getValue().asText());
			}
		} catch (final Exception e) {
			e.printStackTrace();
		}
	}

	private static void loadTranslation() {
		if (ETranslate.globalTranslateLoadad) {
			return;
		}
		LOGGER.debug("Load Translation MAJOR='{}' LANG='{}' default={}", ETranslate.globalMajor, ETranslate.globalLanguage, ETranslate.globalLanguageDefault);
		LOGGER.debug("list path={}", ETranslate.globalListPath.keySet());
		// start parse language for Major:
		final Uri itMajor = ETranslate.globalListPath.get(ETranslate.globalMajor);
		if (itMajor != null) {
			loadTranslationFile(itMajor.withPath(itMajor.getPath() + "/" + ETranslate.globalLanguage + ".json"));
			loadTranslationFile(itMajor.withPath(itMajor.getPath() + "/" + ETranslate.globalLanguageDefault + ".json"));
		}
		// start parse language:
		for (final Map.Entry<String, Uri> it : ETranslate.globalListPath.entrySet()) {
			if (it.getKey().contentEquals(ETranslate.globalMajor)) {
				continue;
			}
			loadTranslationFile(it.getValue().withPath(it.getValue().getPath() + "/" + ETranslate.globalLanguage + ".json"));
		}
		// start parse default language:
		for (final Map.Entry<String, Uri> it : ETranslate.globalListPath.entrySet()) {
			if (it.getKey().contentEquals(ETranslate.globalMajor)) {
				continue;
			}
			loadTranslationFile(it.getValue().withPath(it.getValue().getPath() + "/" + ETranslate.globalLanguageDefault + ".json"));
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
			LOGGER.info("Change language translation: '{}'=English", lang);
		} else if (lang.equals("FR")) {
			LOGGER.info("Change language translation: '{}'=French", lang);
		} else if (lang.equals("DE")) {
			LOGGER.info("Change language translation: '{}'=German", lang);
		} else if (lang.equals("SP")) {
			LOGGER.info("Change language translation: '{}'=Spanish", lang);
		} else if (lang.equals("JA")) {
			LOGGER.info("Change language translation: '{}'=Japanese", lang);
		} else if (lang.equals("IT")) {
			LOGGER.info("Change language translation: '{}'=Italian", lang);
		} else if (lang.equals("KO")) {
			LOGGER.info("Change language translation: '{}'=Korean", lang);
		} else if (lang.equals("RU")) {
			LOGGER.info("Change language translation: '{}'=Russian", lang);
		} else if (lang.equals("PT")) {
			LOGGER.info("Change language translation: '{}'=Portuguese, Brazilian", lang);
		} else if (lang.equals("ZH")) {
			LOGGER.info("Change language translation: '{}'=Chinese", lang);
		} else {
			LOGGER.info("Change language translation: '{}'=Unknown", lang);
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
		LOGGER.info("Change default language translation : '{}'", lang);
		ETranslate.globalLanguageDefault = lang;
		ETranslate.globalTranslateLoadad = false;
		ETranslate.globalTranslate.clear();
	}
	
	private ETranslate() {}
}
