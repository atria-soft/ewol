package org.atriasoft.ewol.compositing.tools;

import org.atriasoft.etk.Color;
import org.atriasoft.ewol.resource.font.FontMode;

@SuppressWarnings("preview")
public record TextDecoration(Color colorFG, Color colorBG, FontMode mode) {
	public TextDecoration(final Color colorFG, final Color colorBG, final FontMode mode) {
		this.colorFG = colorFG;
		this.colorBG = colorBG;
		this.mode = mode;
	}

	public TextDecoration() {
		this(Color.BLACK, Color.NONE, FontMode.Regular);
	}

	public TextDecoration withFG(final Color color) {
		return new TextDecoration(color, this.colorBG, this.mode);
	}

	public TextDecoration withBG(final Color color) {
		return new TextDecoration(this.colorFG, color, this.mode);
	}

	public TextDecoration withMode(final FontMode mode) {
		return new TextDecoration(this.colorFG, this.colorBG, mode);
	}
}
