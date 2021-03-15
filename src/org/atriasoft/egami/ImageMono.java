package org.atriasoft.egami;

import org.atriasoft.etk.math.Vector2i;

public class ImageMono {
	private int width;
	private int height;
	private byte[] buffer;
	
	public ImageMono(final int width, final int height) {
		this.width = width;
		this.height = height;
		this.buffer = new byte[width * height];
	}
	
	public ImageMono(final int width, final int height, final byte[] buffer) {
		this.buffer = buffer;
		this.width = width;
		this.height = height;
	}
	
	public byte get(final int x, final int y) {
		return this.buffer[(y * this.width + x)];
	}
	
	public byte[] getBuffer() {
		return this.buffer;
	}
	
	public Vector2i getGPUSize() {
		/*
		if (false) {
			// Some GPU does not support not pow2 dimention....
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
		final byte[] oldBuffer = this.buffer;
		this.width = width;
		this.height = height;
		this.buffer = new byte[this.width * this.height * 4];
		if (this.width <= oldWidth) {
			if (this.height < oldHeight) {
				// Just remove lines ....
				for (int yyy = 0; yyy < this.height; ++yyy) {
					for (int xxx = 0; xxx < this.width; ++xxx) {
						this.buffer[yyy * this.width + xxx] = oldBuffer[yyy * oldWidth + xxx];
					}
				}
			} else {
				// just add lines
				for (int yyy = 0; yyy < oldHeight; ++yyy) {
					for (int xxx = 0; xxx < this.width; ++xxx) {
						this.buffer[yyy * this.width + xxx] = oldBuffer[yyy * oldWidth + xxx];
					}
				}
			}
		} else if (this.height <= oldHeight) {
			for (int yyy = 0; yyy < this.height; ++yyy) {
				for (int xxx = 0; xxx < oldWidth; ++xxx) {
					this.buffer[yyy * this.width + xxx] = oldBuffer[yyy * oldWidth + xxx];
				}
			}
		} else {
			for (int yyy = 0; yyy < oldHeight; ++yyy) {
				for (int xxx = 0; xxx < oldWidth; ++xxx) {
					this.buffer[yyy * this.width + xxx] = oldBuffer[yyy * oldWidth + xxx];
				}
			}
		}
	}
	
	public void set(final int x, final int y, final byte value) {
		this.buffer[(y * this.width + x)] = value;
	}
	
	public void set(final int x, final int y, final float value) {
		this.buffer[(y * this.width + x)] = (byte) (value * 256.0f);
	}
	
	public void setSize(final int width, final int height) {
		this.width = width;
		this.height = height;
		this.buffer = new byte[width * height];
	}
	
}
