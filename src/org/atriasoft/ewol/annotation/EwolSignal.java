package org.atriasoft.ewol.annotation;

public @interface EwolSignal {
	String description() default "";
	
	String[] name();
}
