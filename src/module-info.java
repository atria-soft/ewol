/** Basic module interface.
 *
 * @author Edouard DUPIN */

open module org.atriasoft.ewol {
	exports org.atriasoft.ewol;
	exports org.atriasoft.egami;
	
	requires transitive org.atriasoft.gale;
	requires transitive org.atriasoft.etk;
	requires transitive org.atriasoft.exml;
	requires transitive org.atriasoft.ejson;
	requires transitive io.scenarium.logger;
	requires freetype.jni;
}
