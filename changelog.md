# 1.1.0
by [Erymanthus](https://geode-sdk.org/developers/94), [#1](https://github.com/LatterRarity70/Level-Comments-Widget/pull/1)

* change the hook to setupHasCompleted for better consistency with adding nodes
* add the infolayer node to PlayLayer's UILayer (UILayer swallows all touches anyway so this is a win-win)
* added a node ID to the infolayer added by this mod to reduce typeinfo_cast calls made by calling getchildbytype
* change scheduler target to PlayLayer itself for better code clarity
* refactor a few things to remove redundant temporary variable assignments inside conditional statements
* added node IDs and better touch prio dependencies to resolve touch prio issues

# 1.0.0
- hi
