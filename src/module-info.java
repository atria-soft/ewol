/** Basic module interface.
 *
 * @author Edouard DUPIN */

open module org.atriasoft.ephysics {
	exports org.atriasoft.ephysics.body;
	exports org.atriasoft.ephysics.collision;
	exports org.atriasoft.ephysics.collision.broadphase;
	exports org.atriasoft.ephysics.collision.narrowphase;
	exports org.atriasoft.ephysics.collision.narrowphase.EPA;
	exports org.atriasoft.ephysics.collision.narrowphase.GJK;
	exports org.atriasoft.ephysics.collision.shapes;
	exports org.atriasoft.ephysics.configuration;
	exports org.atriasoft.ephysics.constraint;
	exports org.atriasoft.ephysics.engine;
	exports org.atriasoft.ephysics.mathematics;
	exports org.atriasoft.ephysics;
	
	requires transitive org.atriasoft.etk;
	requires javafx.base;
}
