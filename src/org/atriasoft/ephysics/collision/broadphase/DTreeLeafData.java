package org.atriasoft.ephysics.collision.broadphase;

class DTreeLeafData extends DTree {
	Object dataPointer = null;
	
	public DTreeLeafData(final Object dataPointer) {
		super();
		this.dataPointer = dataPointer;
	}
	
	@Override
	boolean isLeaf() {
		return true;
	}
}