package org.atriasoft.ewol.resource.font;

public enum FontMode {
	Regular(0),
	Italic(1),
	Bold(2),
	BoldItalic(3);
	
	public static FontMode get(final int newValue) {
		switch (newValue) {
			case 1:
				return Italic;
			case 2:
				return Bold;
			case 3:
				return BoldItalic;
		}
		return Regular;
	}
	
	private final int value;
	
	FontMode(final int newValue) {
		this.value = newValue;
	}
	
	public int getValue() {
		return this.value;
	}
}
