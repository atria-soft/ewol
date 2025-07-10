package org.atriasoft.ewol.compositing;

import org.atriasoft.etk.math.Vector2f;
import org.atriasoft.ewol.Padding;

public record ShapeBox(
		Vector2f outOrigin,
		Vector2f outSize,
		Vector2f inOrigin,
		Vector2f inSize) {
	
	public static final ShapeBox ZERO = new ShapeBox(Vector2f.ZERO, Vector2f.ZERO, Vector2f.ZERO, Vector2f.ZERO);
	
	@Override
	public String toString() {
		final StringBuilder out = new StringBuilder();
		out.append("{");
		out.append(this.outOrigin);
		out.append("->");
		out.append(this.outOrigin.add(this.outSize));
		out.append(" / ");
		out.append(this.inOrigin);
		out.append("->");
		out.append(this.inOrigin.add(this.inSize));
		out.append("}");
		return out.toString();
	}
	
	public ShapeBox(final Vector2f outOrigin, final Vector2f outSize, final Vector2f inOrigin, final Vector2f inSize) {
		this.outOrigin = outOrigin;
		this.outSize = outSize;
		this.inOrigin = inOrigin;
		this.inSize = inSize;
	}
	
	public ShapeBox(final Vector2f outOrigin, final Vector2f outSize, final Padding padding) {
		this(outOrigin, outSize, outOrigin.add(padding.left(), padding.bottom()), outSize.less(padding.x(), padding.y()));
	}
	
	public boolean isInside(final Vector2f value) {
		return value.x() > this.outOrigin.x() //
				&& value.y() > this.outOrigin.y() //
				&& value.x() < this.outOrigin.x() + this.outSize.x() //
				&& value.y() < this.outOrigin.y() + this.outSize.y();
	}
	
}
