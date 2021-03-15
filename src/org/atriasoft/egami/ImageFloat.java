package org.atriasoft.egami;

import org.atriasoft.etk.math.Vector2i;

public class ImageFloat {
	private int width;
	private int height;
	private float[] buffer;
	
	public ImageFloat(final int width, final int height) {
		this.width = width;
		this.height = height;
		this.buffer = new float[width * height * 4];
	}
	
	public ImageFloat(final int width, final int height, final float[] buffer) {
		this.buffer = buffer;
		this.width = width;
		this.height = height;
	}
	
	public float getA(final int x, final int y) {
		return this.buffer[(y * this.width + x) * 4 + 3];
	}
	
	public float getB(final int x, final int y) {
		return this.buffer[(y * this.width + x) * 4 + 2];
	}
	
	public float[] getBuffer() {
		return this.buffer;
	}
	
	public float getG(final int x, final int y) {
		return this.buffer[(y * this.width + x) * 4 + 1];
	}
	
	public Vector2i getGPUSize() {
		/*
		if (false) {
			// Some GPU does not support not pow2 dimention....
			/*
			#if    defined(__TARGET_OS__Android) \
			    || defined(__TARGET_OS__IOs)
				return ivec2(nextP2(m_data->getSize().x()),
				             nextP2(m_data->getSize().y()));
		}
		*/
		return getSize();
	}
	
	public int getHeight() {
		return this.height;
	}
	
	public float getR(final int x, final int y) {
		return this.buffer[(y * this.width + x) * 4];
	}
	
	public Vector2i getSize() {
		return new Vector2i(this.width, this.height);
	}
	
	public int getWidth() {
		return this.width;
	}
	
	public void resize(final int width, final int height) {
		if (width == this.width && height == this.height) {
			// same size  == > nothing to do ...
			return;
		}
		final int oldWidth = this.width;
		final int oldHeight = this.height;
		final float[] oldBuffer = this.buffer;
		this.width = width;
		this.height = height;
		this.buffer = new float[this.width * this.height * 4];
		if (this.width <= oldWidth) {
			if (this.height < oldHeight) {
				// Just remove lines ....
				for (int yyy = 0; yyy < this.height; ++yyy) {
					for (int xxx = 0; xxx < this.width; ++xxx) {
						this.buffer[(yyy * this.width + xxx) * 4] = oldBuffer[(yyy * oldWidth + xxx) * 4];
						this.buffer[(yyy * this.width + xxx) * 4 + 1] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 1];
						this.buffer[(yyy * this.width + xxx) * 4 + 2] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 2];
						this.buffer[(yyy * this.width + xxx) * 4 + 3] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 3];
					}
				}
			} else {
				// just add lines
				for (int yyy = 0; yyy < oldHeight; ++yyy) {
					for (int xxx = 0; xxx < this.width; ++xxx) {
						this.buffer[(yyy * this.width + xxx) * 4] = oldBuffer[(yyy * oldWidth + xxx) * 4];
						this.buffer[(yyy * this.width + xxx) * 4 + 1] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 1];
						this.buffer[(yyy * this.width + xxx) * 4 + 2] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 2];
						this.buffer[(yyy * this.width + xxx) * 4 + 3] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 3];
					}
				}
			}
		} else if (this.height <= oldHeight) {
			for (int yyy = 0; yyy < this.height; ++yyy) {
				for (int xxx = 0; xxx < oldWidth; ++xxx) {
					this.buffer[(yyy * this.width + xxx) * 4] = oldBuffer[(yyy * oldWidth + xxx) * 4];
					this.buffer[(yyy * this.width + xxx) * 4 + 1] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 1];
					this.buffer[(yyy * this.width + xxx) * 4 + 2] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 2];
					this.buffer[(yyy * this.width + xxx) * 4 + 3] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 3];
				}
			}
		} else {
			for (int yyy = 0; yyy < oldHeight; ++yyy) {
				for (int xxx = 0; xxx < oldWidth; ++xxx) {
					this.buffer[(yyy * this.width + xxx) * 4] = oldBuffer[(yyy * oldWidth + xxx) * 4];
					this.buffer[(yyy * this.width + xxx) * 4 + 1] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 1];
					this.buffer[(yyy * this.width + xxx) * 4 + 2] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 2];
					this.buffer[(yyy * this.width + xxx) * 4 + 3] = oldBuffer[(yyy * oldWidth + xxx) * 4 + 3];
				}
			}
		}
	}
	
	public void setA(final int x, final int y, final byte value) {
		this.buffer[(y * this.width + x) * 4 + 3] = value / 256.0f;
	}
	
	public void setA(final int x, final int y, final float value) {
		this.buffer[(y * this.width + x) * 4 + 3] = value;
	}
	
	public void setB(final int x, final int y, final byte value) {
		this.buffer[(y * this.width + x) * 4 + 2] = value / 256.0f;
	}
	
	public void setB(final int x, final int y, final float value) {
		this.buffer[(y * this.width + x) * 4 + 2] = value;
	}
	
	public void setG(final int x, final int y, final byte value) {
		this.buffer[(y * this.width + x) * 4 + 1] = value / 256.0f;
	}
	
	public void setG(final int x, final int y, final float value) {
		this.buffer[(y * this.width + x) * 4 + 1] = value;
	}
	
	public void setR(final int x, final int y, final byte value) {
		this.buffer[(y * this.width + x) * 4] = value / 256.0f;
	}
	
	public void setR(final int x, final int y, final float value) {
		this.buffer[(y * this.width + x) * 4] = value;
	}
	
	public void setSize(final int width, final int height) {
		this.width = width;
		this.height = height;
		this.buffer = new float[width * height * 4];
	}
}
