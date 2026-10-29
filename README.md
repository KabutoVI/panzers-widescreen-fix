# Panzers: Phase One - widescreen / borderless fix

A tiny `d3d9.dll` proxy for Codename: Panzers - Phase One (Spanish Premium DVD release).
It forwards `Direct3DCreate9` to the system DLL and applies three byte-checked patches to
the running game's memory. The game files on disk are never modified. This matters because
a patched `PANZERS.EXE` on disk made the game quit shortly after the main menu.

## What it changes (in memory)

1. Removes the 4:3-only filter on the Graphics menu's resolution list, so widescreen modes appear.
2. Replaces the two windowed-window style values with a popup style, which gives a borderless
   window, including when switching to windowed with F11.

Each patch checks the original bytes first and is skipped if they don't match, so it does
nothing on other builds. It was tested against a `PANZERS.EXE` with SHA-256
`ac8b3d3ffbd3b39a7e2b52008d444b3ba902b61714bb82aa02534583b76b3bc0`.

## Install

1. Download `panzers-widescreen-fix-v0.1.0.zip` from the Releases page, or build it yourself
   (below).
2. Copy `d3d9.dll` next to `PANZERS.EXE` in your own installation. If a `d3d9.dll` is already
   there (for example from dgVoodoo), keep a copy of it, because this one replaces it.
3. For a borderless window at start: in `options.ini` set `FullScreen=0`, `WindowWidth=2560`
   and `WindowHeight=1440` (use your own screen size). F11 switches from fullscreen to
   the borderless window.

## Uninstall

Delete `d3d9.dll` from the game folder, and restore the one you kept if there was one.

## Build

From an MSVC x86 command prompt, run `build.bat`. It produces `d3d9.dll` (32-bit).

## Known notes

- A stale `options.ini` with a resolution the game doesn't list can crash the Graphics screen.
  Let the game regenerate its defaults if that happens.
- Tested on Windows 11 at 2560x1440 only.
- No game files are included. You need your own copy of the game.

## License

[MIT](LICENSE)
