package org.atriasoft.ewol.resource.font;

public enum FontMode {
	REGULAR(0),
	ITALIC(1),
	BOLD(2),
	BOLD_ITALIC(3);
	
	public static FontMode get(final int newValue) {
		return switch (newValue) {
			case 1 -> ITALIC;
			case 2 -> BOLD;
			case 3 -> BOLD_ITALIC;
			default -> REGULAR;
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
