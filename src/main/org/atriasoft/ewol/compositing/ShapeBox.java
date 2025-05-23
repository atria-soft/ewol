package org.atriasoft.ewol.compositing;

import org.atriasoft.etk.math.Vector3f;
import org.atriasoft.ewol.Padding;

public record ShapeBox(
		Vector3f outOrigin,
		Vector3f outSize,
		Vector3f inOrigin,
		Vector3f inSize) {
	
	public static final ShapeBox ZERO = new ShapeBox(Vector3f.ZERO, Vector3f.ZERO, Vector3f.ZERO, Vector3f.ZERO);
	
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
	
	public ShapeBox(final Vector3f outOrigin, final Vector3f outSize, final Vector3f inOrigin, final Vector3f inSize) {
		this.outOrigin = outOrigin;
		this.outSize = outSize;
		this.inOrigin = inOrigin;
		this.inSize = inSize;
	}
	
	public ShapeBox(final Vector3f outOrigin, final Vector3f outSize, final Padding padding) {
		this(outOrigin, outSize, outOrigin.add(padding.left(), padding.bottom(), padding.back()), outSize.less(padding.x(), padding.y(), padding.z()));
	}
	
	public boolean isInside(final Vector3f value) {
		return value.x() > this.outOrigin.x() //
				&& value.y() > this.outOrigin.y() //
				&& value.x() < this.outOrigin.x() + this.outSize.x() //
				&& value.y() < this.outOrigin.y() + this.outSize.y();
	}
	
}
