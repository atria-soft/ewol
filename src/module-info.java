/** Basic module interface.
 *
 * @author Edouard DUPIN */

open module org.atriasoft.ewol {
	exports org.atriasoft.ewol;
	
	requires transitive org.atriasoft.gale;
	requires transitive org.atriasoft.etk;
	requires transitive org.atriasoft.exml;
	requires transitive io.scenarium.logger;
}
