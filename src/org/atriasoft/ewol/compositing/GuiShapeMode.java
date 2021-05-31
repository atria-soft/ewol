package org.atriasoft.ewol.compositing;

public enum GuiShapeMode {
	NONE(-1),
	NORMAL(0),
	OVER(1),
	SELECT(2);
	private float value; 
	GuiShapeMode(final float value) {
		this.value = value;
	}
	public float getValue() {
		return this.value;
	}
}
