## Common Pitfalls

Problems encountered during development — read these to avoid repeating the same mistakes.

### Event system

- **`getWidgetAtPos()` controls who receives events.** If an overlay widget returns `this` for the entire window, it intercepts ALL mouse events and the widget tree below receives nothing. For overlay layers that should be transparent to events (e.g. managed popovers), return `null` from `getWidgetAtPos()`.

- **`InputManager` sends `KeyStatus.leave` when the mouse exits the *target widget* bounds, not the parent.** If a `Container` wraps a `Label`, leaving the `Label` triggers a `leave` event even if the mouse is still inside the `Container`. Always check if the mouse is still inside the parent before closing on `leave`:
  ```java
  if (event.status() == KeyStatus.leave) {
      final Vector2f relPos = relativePosition(event.pos());
      if (isInsideTrigger(relPos)) {
          return; // Still inside — don't close
      }
      // Actually left
  }
  ```

- **`systemEventInput()` bubbles up from child to parent.** Override `systemEventInput()` (not `onEventInput()`) to observe events passing through a Container without consuming them. Call `super.systemEventInput(event)` to let the child handle normally.

- **`inputId == 0` means cursor movement** (no button pressed). Use this to detect hover enter/leave.

### Layout and positioning

- **Widget children use absolute coordinates.** Calling `setOrigin()` on a parent Box without calling `onChangeSize()` moves the box background but NOT its children. Always call `parent.onChangeSize()` after `setOrigin()` to propagate position changes to children.

- **`onChangeSize()` is heavy** — it triggers full layout recalculation (`calculateMinMaxSize()` etc.). For per-frame position updates (e.g. follow-mouse), create a fast path that only recalculates the position, arrow, and hit-box without full relayout. See `Popover.relocateToAnchor()`.

- **Popup stack triggers `onChangeSize()` on ALL widgets** (main widget + all popups). Don't use `Windows.popUpWidgetPush()` for lightweight overlays. Use a dedicated layer (like `PopoverManager`) instead.

### Widget reuse

- **Don't reuse Widget instances across show/dismiss cycles.** Internal state (especially `TextBase` parsing in Labels) gets corrupted. Use a `Supplier<Widget>` factory pattern to create fresh widgets each time:
  ```java
  .popoverContent(() -> new Label("Fresh content"))
  ```

### Rendering

- **`systemDrawWidget()` sets `OpenGL.setViewPort()` OUTSIDE push/pop** — the viewport persists after the method returns. When drawing overlay widgets, use `displayProp.withLimit()` to clip to the widget zone (see `SelectPopup` pattern).

- **Layer order in `Windows.systemDraw()`** (bottom to top): Main widget → Popup stack → Popover layer → Toast notifications. New overlay layers must be drawn in the correct order.

- **Split static and dynamic rendering.** For widgets with expensive backgrounds (gradients, track shapes), split into a cached background `CompositingGC` (regenerated only on resize) and a lightweight cursor/fill overlay (regenerated every frame). See `ColorGradient` (gradient + cursor) and `Slider` (track + cursor/fill) for the pattern.

- **Cache resource data.** `Uri.getAllDataString()` has no internal cache — it reads from classpath every call. Cache the result in a field (see `Tick.cachedSvgData`).

### Resources (gale `create()`/`keep()`/`release()`)

- **Widgets have no destruction step.** A widget taken out of the tree can be put back (a window set again, a popup pushed again), so it dies only when the garbage collector collects it. Never release a resource because a widget left its parent.
- **Give each resource you create or keep to an owner** (`OwnedResources`): it is released exactly once, when the owner replaces it (`releaseOwned`), when the owner knows it is dead (`releaseAll`), otherwise when the owner is collected (a `Cleaner`). A reachable owner, thus one still drawn, is never collected.
- **Compositings own their resources** (vertex array, program, texture, font) through `own(...)`. `Compositing.release()` frees them at once when the layer is drawn no more (a replaced layer, a HUD rebuilt); it draws nothing afterwards. Widgets use `Widget.own(...)` (color files...).
- **Factories with a cache keep the shared resource** (`keepExisting(name, Type.class)`, which throws `IllegalStateException` on a name of another type): one `release()` per `create()`. Do not call `new ResourceColorFile(...)`: it registers a duplicate, use `ResourceColorFile.create(...)`.

### Container

- **`Container(Widget)` constructor does NOT call `setParent()`.** Use `setSubWidget()` to properly set the parent. The constructor just stores the reference without registering the parent relationship.
