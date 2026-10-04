package test.atriasoft.ewol.widget;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertSame;

import org.atriasoft.ewol.widget.Gird;
import org.atriasoft.ewol.widget.Spacer;
import org.atriasoft.ewol.widget.Widget;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import test.atriasoft.ewol.EwolTestContext;

class GirdTest {

	/** A grid showing its cells. */
	private static final class GirdProbe extends Gird {
		int cellCount() {
			return this.subWidget.size();
		}

		Widget cell(final int index) {
			return this.subWidget.get(index).widget;
		}

		int columnCount() {
			return this.sizeCol.size();
		}
	}

	@BeforeAll
	static void setUpClass() {
		EwolTestContext.init();
	}

	@Test
	void aWidgetAddedOnATakenCellReplacesTheOldOne() {
		final GirdProbe grid = new GirdProbe();
		final Widget first = new Spacer();
		final Widget second = new Spacer();
		grid.subWidgetAdd(0, 0, first);
		grid.subWidgetAdd(0, 0, second);
		assertEquals(1, grid.cellCount(), "the cell is replaced, not added again");
		assertSame(second, grid.cell(0));
	}

	@Test
	void aWidgetAddedBeforeATakenCellReplacesOnlyItsOwnCell() {
		final GirdProbe grid = new GirdProbe();
		final Widget left = new Spacer();
		final Widget right = new Spacer();
		final Widget newRight = new Spacer();
		grid.subWidgetAdd(0, 0, left);
		grid.subWidgetAdd(1, 0, right);
		grid.subWidgetAdd(1, 0, newRight);
		assertEquals(2, grid.cellCount());
		assertSame(left, grid.cell(0));
		assertSame(newRight, grid.cell(1));
	}

	@Test
	void fewerColumnsRemoveTheCellsOfTheRemovedColumns() {
		final GirdProbe grid = new GirdProbe();
		grid.setColNumber(3);
		final Widget kept = new Spacer();
		grid.subWidgetAdd(0, 0, kept);
		grid.subWidgetAdd(2, 0, new Spacer());
		grid.subWidgetAdd(1, 1, new Spacer());
		grid.setColNumber(1);
		assertEquals(1, grid.columnCount());
		assertEquals(1, grid.cellCount());
		assertSame(kept, grid.cell(0));
	}
}
