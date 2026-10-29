# Panzers: Phase One - widescreen / borderless fix

A tiny `d3d9.dll` proxy for Codename: Panzers - Phase One (Spanish Premium DVD release).
It forwards `Direct3DCreate9` to the system DLL and applies three byte-checked patches to
the running game's memory. The game files on disk are never modified, which matters because
a patched `PANZERS.EXE` on disk makes the game quit shortly after the main menu.

## What it changes (in memory)

1. Removes the 4:3-only filter on the Graphics menu's resolution list, so widescreen modes appear.
2. Replaces the two windowed-window style values with a popup style, giving a borderless window
   (also when toggling with F11).

Each patch checks the original bytes first and is skipped if they don't match, so it does
nothing on other builds. Tested against a `PANZERS.EXE` with SHA-256 starting `ac8b3d3f`.

## Use

1. Build (MSVC x86 command prompt): `build.bat`, producing `d3d9.dll`.
2. Copy `d3d9.dll` next to `PANZERS.EXE` in your own installation. Back up anything you replace.
3. For borderless 2560x1440 at start: in `options.ini` set `FullScreen=0`, `WindowWidth=2560`,
   `WindowHeight=1440`. (Fullscreen with F11 toggling also works.)
4. Undo: delete `d3d9.dll`.

## Known notes

- A stale `options.ini` with an unlisted resolution can crash the Graphics screen; let the game
  regenerate defaults if that happens.
- No game files are included. You need your own copy of the game.
