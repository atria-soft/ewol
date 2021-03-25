package org.atriasoft.ewol.annotation;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Meta-annotation (annotations used on other annotations)
 * used for marking all annotations that are
 * part of Exml package. Can be used for recognizing all
 * Exml annotations generically, and in future also for
 * passing other generic annotation configuration.
 */
@Target(ElementType.ANNOTATION_TYPE)
@Retention(RetentionPolicy.RUNTIME)
public @interface EwolAnnotation {
	// for now, a pure tag annotation, no parameters
	
}
