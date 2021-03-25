package org.atriasoft.ewol.resource.font;

public enum FontMode {
	Regular(0),
	Italic(1),
	Bold(2),
	BoldItalic(3);
	
	public static FontMode get(final int newValue) {
		return switch (newValue) {
			case 1 -> Italic;
			case 2 -> Bold;
			case 3 -> BoldItalic;
			default -> Regular;
		};
	}
	
	private final int value;
	
	FontMode(final int newValue) {
		this.value = newValue;
	}
	
	public int getValue() {
		return this.value;
	}
}
