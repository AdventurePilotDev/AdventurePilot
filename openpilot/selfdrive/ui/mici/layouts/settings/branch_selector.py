import os
from collections.abc import Callable

from openpilot.selfdrive.ui.mici.widgets.button import BigButton
from openpilot.selfdrive.ui.mici.widgets.dialog import BigDialog
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.application import gui_app
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.widgets.scroller import NavScroller

# Top-level branches surfaced directly (in this order), mirroring the "big" UI branch switcher
# (selfdrive/ui/sunnypilot/layouts/settings/software.py). The current branch is prepended at runtime.
TOP_LEVEL_BRANCHES = ["release-mici", "release-tizi", "staging", "dev", "master"]


class BranchSelectorMici(NavScroller):
  """Branch switcher for the mici (comma four) UI.

  The compact mici UI has no Software settings page, so this lives as a pushed sub-page off the
  Developer panel. It reuses the same updater plumbing as the big UI: it lists
  UpdaterAvailableBranches (populated by `updated` from the installed repo's origin), writes the
  selection to UpdaterTargetBranch, and pokes `updated` (SIGUSR1) to act on it.

  Navigation follows the ModelsLayoutMici idiom: a single NavScroller whose item list is swapped
  in place (folders -> branches) rather than opening separate dialogs.
  """

  def __init__(self, back_callback: Callable):
    super().__init__()
    self.set_back_callback(back_callback)
    self.original_back_callback = back_callback

    # Display-only header showing the current target.
    self._current_btn = BigButton(tr("target branch"), self._current_target(), scroll=True)
    self._current_btn.set_enabled(False)

    self.main_items = [self._current_btn, *self._build_top_level_items()]
    self._scroller.add_widgets(self.main_items)

  def _current_target(self) -> str:
    return ui_state.params.get("UpdaterTargetBranch") or ui_state.params.get("GitBranch") or ""

  def _available_branches(self) -> list[str]:
    branches_str = ui_state.params.get("UpdaterAvailableBranches") or ""
    return [b for b in branches_str.split(",") if b]

  def _build_top_level_items(self) -> list[BigButton]:
    branches = self._available_branches()
    current_git_branch = ui_state.params.get("GitBranch") or ""

    # De-duped, order-preserving list of top-level branches that actually exist on the remote.
    top_level = [b for b in dict.fromkeys([current_git_branch, *TOP_LEVEL_BRANCHES]) if b and b in branches]
    prebuilt = sorted(b for b in branches if b.endswith("-prebuilt") and b not in top_level)
    other = sorted(b for b in branches if not b.endswith("-prebuilt") and b not in top_level)

    items: list[BigButton] = []
    for b in top_level:
      items.append(self._branch_button(b))
    if prebuilt:
      items.append(self._folder_button(tr("prebuilt branches"), prebuilt))
    if other:
      items.append(self._folder_button(tr("non-prebuilt branches"), other))
    return items

  def _branch_button(self, branch: str) -> BigButton:
    btn = BigButton(branch, scroll=True)
    btn.set_click_callback(lambda b=branch: self._select(b))
    return btn

  def _folder_button(self, label: str, branches: list[str]) -> BigButton:
    btn = BigButton(label)
    btn.set_click_callback(lambda: self._show_branch_list(branches))
    return btn

  def _show_selection_view(self, items: list[BigButton], back_callback: Callable) -> None:
    self._scroller._items = items
    for item in items:
      item.set_touch_valid_callback(lambda: self._scroller.scroll_panel.is_touch_valid() and self._scroller.enabled)
    self._scroller.scroll_panel.set_offset(0)
    self.set_back_callback(back_callback)

  def _show_main_view(self) -> None:
    self._scroller._items = self.main_items
    self.set_back_callback(self.original_back_callback)
    self._scroller.scroll_panel.set_offset(0)

  def _show_branch_list(self, branches: list[str]) -> None:
    self._show_selection_view([self._branch_button(b) for b in branches], self._show_main_view)

  def _select(self, branch: str) -> None:
    ui_state.params.put("UpdaterTargetBranch", branch)
    # Match the tici branch switcher: set the target and SIGUSR1 (UserRequest.CHECK) to refresh the
    # updater. Downloading + installing is driven by the existing mici update flow — Device ->
    # "update sunnypilot" (which cycles check -> download -> install) or the home "update available"
    # alert — both of which already respect UpdaterTargetBranch, just like tici's separate
    # Download/Install buttons.
    os.system("pkill -SIGUSR1 -f system.updated.updated")
    self._current_btn.set_value(branch)
    self.original_back_callback()
    gui_app.push_widget(BigDialog(tr("target branch set"),
                                  tr("open device settings and tap update to download and install")))
