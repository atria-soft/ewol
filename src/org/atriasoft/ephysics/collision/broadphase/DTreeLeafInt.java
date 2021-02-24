package org.atriasoft.ephysics.collision.broadphase;

class DTreeLeafInt extends DTree {
	int dataInt_0 = 0;
	int dataInt_1 = 0;
	
	public DTreeLeafInt(final int dataInt_0, final int dataInt_1) {
		super();
		this.dataInt_0 = dataInt_0;
		this.dataInt_1 = dataInt_1;
	}
	
	@Override
	boolean isLeaf() {
		return true;
	}
}
