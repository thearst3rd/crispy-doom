# Crispy Doom U
![Screenshot of game in Wii U Gamepad](wiiu/CrispyWiiUScreenshot.png)

Crispy Doom U is a Wii U port of [Crispy Doom](https://www.chocolate-doom.org/wiki/index.php/Crispy_Doom). It is possible thanks to the amazing efforts of the Crispy/Chocolate Doom authors, devkitPro, and the team that ported SDL2 to the Wii U. Without these people and more, this port would have been a lot harder to make, or not happened at all!

## Download
Download Crispy Doom U on the [Releases](https://github.com/thearst3rd/crispy-doom-u/releases) page.

To install, extract the release's contents into `sd:/wiiu/apps/crispy-doom-u` (this is hard coded for now) and put your limit removing WADs in `sd:/wiiu/apps/crispy-doom-u/wads`. If you do not have any Doom WADs, check out the free [shareware version of Doom](https://www.doomworld.com/classicdoom/info/shareware.php), or check out the amazing [Freedoom project](https://freedoom.github.io/)!

## Features
* Up to date with Crispy Doom 5.12.0 (up to commit [35dc15e](https://github.com/fabiangreffrath/crispy-doom/commit/35dc15ec4175d8d46997276953604e24e053a2fa))
* Very high feature parity with the PC version, full Crispy menu intact (other than things not relevant to a Wii U release)
* Smooth analog control support - both moving and turning make use of the analog values of the analog sticks
* Simple launcher that lets you pick an IWAD and optionally PWADs
* Music support using OPL3
	* Causes a bit of a stutter when loading a new music track. I'm looking into making this faster, but it's not too bad for most levels.
	* Note: There is a known bug involving the Freedoom WADs (and probably others) - details below
* Wii U Pro Controller and Wii Classic Controller support
* Controller rebinding
	* Currently this can only be achieved by editing `crispy-doom.cfg`. In the future this will be made easier by doing it through the launcher.
* Optional toggle to fix the [blockmap bug](https://youtu.be/-IYz6-KnvWU) (on page 4 of the Crispy menu)

### What's not perfect (yet)
As this is a pre-release, not everything is perfect yet. Here are a list of some things to be aware of.
* High resolution rendering is laggy
	* When high resolution rendering is enabled (especially when combined with widescreen), the game becomes notably laggy. This is a bit of a surprise to me since I figured the Wii U would be totally capable of running this game at higher resolutions right out of the box. More optimizations are needed, I hope that I will be able to get this to work at full speed without major changes to the renderer. I also might just be doing something very wrong/inefficient so we'll see :)
* No networking support
	* `SDL2_net` is not supported on the Wii U, so for now, this is single player only.
* Launcher is lacking features
	* I plan on improving the launcher in the future to let you add additional parameters if you'd like (such as `-fast`, `-nomonsters`, `-warp xy` etc), record/play back demos, and rebind controls.
* No cheats
	* Since there is no keyboard, you cannot enter any cheat codes. I plan to rectify this by adding a menu to control the cheats from there.

### Known Bugs

* When playing with a [Freedoom](https://freedoom.github.io/) WAD (and probably others), the music will _sometimes_ not work. You might need to switch to a different stage then switch back to kick-start music into working.
	* I have a feeling this is because Freedoom uses `midi` files rather than `mus` files that that doesn't play perfectly with my music speedup hack. Looking into it.

## Default Controls

Action | Button
--- | ---
Move | Left stick
Aim | Right stick
Use | A
Fire | ZR
Jump (if enabled) | B
Toggle automap | Minus
Toggle automap overlay mode | Y
Toggle automap rotation mode | X
Previous weapon | L
Next weapon | R
Select shotgun | D-pad up
Select chaingun | D-pad right
Select rocket launcher | D-pad down
Select plasma rifle | D-pad left

*Note: on the Wii Classic Controller, ZL and L are swapped, as well as ZR and R.*

## Compiling
If you just want to play the game, you do not need to compile - instead, checkout the download section above. If you are a developer and want to compile the game yourself, feel free to check out these instructions.

The Wii U version of Crispy Doom compiles using [wut](https://github.com/devkitPro/wut). The instructions for building are as follows:

Get setup with [devkitPro](https://devkitpro.org/wiki/Getting_Started), and make sure to enable Wii U development (wiiu-dev).

Download necessary dependancies:

```bash
(dkp-)pacman -S wiiu-sdl2{,_mixer} ppc-libvorbisidec
```

Build the executable:

```bash
make -f Makefile.wiiu
```

NOTE: This repository is fully capable of building the original PC version as well. If you plan on doing so, I recommend `clean`ing the repository in between building the two different versions because the build system can get confused.

### The rest of the readme is the original Crispy Doom readme. Thanks for playing!

# Crispy Doom
[![Crispy Doom Icon](https://github.com/fabiangreffrath/crispy-doom/blob/master/data/doom.png)](https://github.com/fabiangreffrath/crispy-doom)

[![Top Language](https://img.shields.io/github/languages/top/fabiangreffrath/crispy-doom.svg)](https://github.com/fabiangreffrath/crispy-doom)
[![Code Size](https://img.shields.io/github/languages/code-size/fabiangreffrath/crispy-doom.svg)](https://github.com/fabiangreffrath/crispy-doom)
[![License](https://img.shields.io/github/license/fabiangreffrath/crispy-doom.svg?logo=gnu)](https://github.com/fabiangreffrath/crispy-doom/blob/master/COPYING.md)
[![Release](https://img.shields.io/github/release/fabiangreffrath/crispy-doom.svg)](https://github.com/fabiangreffrath/crispy-doom/releases)
[![Release Date](https://img.shields.io/github/release-date/fabiangreffrath/crispy-doom.svg)](https://github.com/fabiangreffrath/crispy-doom/releases)
[![Downloads](https://img.shields.io/github/downloads/fabiangreffrath/crispy-doom/latest/total.svg)](https://github.com/fabiangreffrath/crispy-doom/releases)
[![Commits](https://img.shields.io/github/commits-since/fabiangreffrath/crispy-doom/latest.svg)](https://github.com/fabiangreffrath/crispy-doom/commits/master)
[![Last Commit](https://img.shields.io/github/last-commit/fabiangreffrath/crispy-doom.svg)](https://github.com/fabiangreffrath/crispy-doom/commits/master)
[![Build Status](https://github.com/fabiangreffrath/crispy-doom/actions/workflows/main.yml/badge.svg)](https://github.com/fabiangreffrath/crispy-doom/actions/workflows/main.yml)

Crispy Doom is a limit-removing enhanced-resolution Doom source port based on [Chocolate Doom](https://www.chocolate-doom.org/wiki/index.php/Chocolate_Doom).

Its name means that its internal 640x400 resolution looks "crisp" and is also a [slight reference](http://www.mathsisfun.com/recipie.html) to its origin.

## Synopsis

Crispy Doom is a friendly fork of [Chocolate Doom](https://www.chocolate-doom.org/wiki/index.php/Chocolate_Doom) that provides a higher display resolution, removes the [static limits](https://doomwiki.org/wiki/Static_limits) of the Doom engine and offers further optional visual, tactical and physical enhancements while remaining entirely config file, savegame, netplay and demo compatible with the original.

## Objectives and features

Crispy Doom is a source port that aims to provide a faithful Doom gaming experience while also featuring some user-requested improvements and enhancements. It is forked off of Chocolate Doom to take advantage of its free and open-source code base, portability, accuracy and compatibility with Vanilla Doom.

Its core features are:

 * Enhanced 640x400 display resolution, with the original 320x200 resolution still available in the "High Resolution Rendering: Off" mode.
 * Widescreen rendering for using all the available horizontal space of screens with aspect ratios up to 24:9.
 * Uncapped rendering framerate with interpolation and optional vertical synchronization (VSync) with the screen refresh rate.
 * Intermediate gamma correction levels (0.5, 1.5, 2.5 and 3.5).
 * Removal of all static engine limits, or at least raising of the less crucial ones.
 * Full support for the "Doom Classic" WADs shipped with the "Doom 3: BFG Edition", especially the "No Rest For The Living" episode shipped in the NERVE.WAD file.
 * Support for all versions of John Romero's Episode 5: Sigil for Ultimate Doom.

Furthermore, the following optional user-visible and audible features are available:

 * Jumping.
 * Free vertical looking, including mouse look and vertical aiming.
 * Aiming support by a crosshair that may get directly rendered into the game world.
 * A new minimal Crispy HUD, displaying only the status bar numbers.
 * Clean Screenshot feature, enabling to take screenshots without HUD elements and even without status bar numbers and weapon sprites at higher screen sizes.
 * Colorized status bar numbers, HUD texts and blood sprites for certain monsters.
 * Translucency for certain sprites and status bar elements in the Crispy HUD.
 * Randomly mirrored death animations and corpse sprites.
 * Command line options to allow for playing with flipped player weapon sprites and/or entirely flipped level geometry.
 * Players may walk over or under monsters and hanging corpses.
 * Centered Weapons when firing, weapon recoil thrust and pitch.
 * Reports whenever a secret is revealed.
 * Level statistics and extended coloring in the Automap.
 * Playing sounds in full length, and misc. other sound fixes.
 * Demo recording and/or playback timers and progress bar.
 * Demo continue and take-over features, handing controls over to the player when demo playback is finished or interrupted.

Most of these features are disabled by default and need to get enabled either in the in-game "Crispness" menu, in the crispy-doom-setup tool or as command line parameters. They are implemented in a way that preserves demo-compatibility with Vanilla Doom and network game compatibility with Chocolate Doom. Furthermore, Crispy Doom's savegames and config files are compatible, though not identical (see the [Compatibility section in the Wiki](https://github.com/fabiangreffrath/crispy-doom/wiki/Compatibility)), to Vanilla Doom's.

Crispy Doom strives for maximum compatibility with all "limit-removing Vanilla" maps -- but not Boom or ZDoom maps. More specifically, Crispy Doom supports some select advanced features such as [ANIMATED](https://doomwiki.org/wiki/ANIMATED) and [SWITCHES](https://doomwiki.org/wiki/SWITCHES) lumps, MBF sky transfers, SMMU swirling flats and [MUSINFO](https://doomwiki.org/wiki/MUSINFO) -- but neither generalized linedef and sector types nor DECORATE and MAPINFO.

Many additional less user-visible features have been implemented, e.g. fixed engine limitations and crashes, fixed rendering bugs, fixed harmless game logic bugs, full support for DEHACKED files and lumps in BEX format, additional and improved cheat codes, an improved Automap, and many more! Due to the extra DEHACKED states added from [MBF](https://doomwiki.org/wiki/MBF), Crispy Doom supports [enhancer](https://www.doomworld.com/forum/topic/84859-black-ops-smooth-weapons-dehacked-mod) [mods](https://www.doomworld.com/forum/topic/85991-smoothed-smooth-monsters-for-doom-retro-and-crispy-doom) that can make the gameplay even more pleasing to the eyes. For a detailed list of features and changes please refer to the release notes below.

### New controls (with default bindings)

 * Move Forward (alt.) <kbd>W</kbd>
 * Move Backward (alt.) <kbd>S</kbd>
 * Strafe Left (alt.) <kbd>A</kbd>
 * Strafe Right (alt.) <kbd>D</kbd>
 * Jump (bindable to joystick and mouse buttons as well) <kbd>/</kbd> (as in Hexen and Strife)
 * Quick Reverse (bindable to mouse buttons as well)
 * Mouse Look (bindable to mouse buttons or permanent)
 * Look up (bindable to joystick axes as well) <kbd>PgDn</kbd> (as in Heretic)
 * Look down (bindable to joystick axes as well) <kbd>Del</kbd> (as in Heretic)
 * Center view <kbd>End</kbd> (as in Heretic)
 * Toggle always run <kbd>&#8682;</kbd>
 * Toggle vertical mouse movement (new in 5.4)
 * Delete savegame <kbd>Del</kbd>
 * Go to next level
 * Reload current level
 * Save a clean screenshot
 * Toggle Automap overlay mode <kbd>O</kbd>
 * Toggle Automap rotate mode <kbd>R</kbd>
 * Resurrect from savegame (single player mode only) "Run" + "Use"

### New command line parameters

 * `-dm3` specifies the Deathmatch 3.0 rules (weapons stay, items respawn) for netgames (since 4.1).
 * `-episode 1` launches Hell on Earth and `-episode 2` launches No Rest for the Living episode if the Doom 2 IWAD shipped with the Doom 3: BFG Edition is used.
 * `-warp 1a` warps to the secret level E1M10: Sewers of XBox Doom IWAD (since 2.3).
 * `-mergedump <file>` merges the PWAD file(s) given on the command line with the IWAD file and writes the resulting data into the `<file>` given as argument. May be considered as a replacement for the `DEUSF.EXE` tool (since 2.3).
 * `-lumpdump` dumps raw content of a lump into a file (since 5.7).
 * `-blockmap` forces a (re-)building of the BLOCKMAP lumps for loaded maps (since 2.3).
 * `-playdemo demoname -warp N` plays back fast-forward up to the requested map (since 3.0).
 * `-loadgame N -record demoname` and `-loadgame N -playdemo demoname` allow to record and play demos starting from a savegame instead of the level start (since 4.0).
 * `-playdemo demoname1 -record demoname2` plays back fast-forward until the end of demoname1 and continues recording as demoname2 (new in 5.5).
 * `-fliplevels` loads mirrored versions of the maps (this was the default on April 1st up to version 5.0).
 * `-flipweapons` flips the player's weapons (new in 5.3).
 * `-levelstat` prints a levelstat.txt file with statistics for each completed level (new in 5.9.0).
 * `-pistolstart` reset health, armor and inventory at start of each level in Doom (new in 5.9.2)
 * `-wandstart` reset health, armor and inventory at start of each level in Heretic (new in 5.9.2)
 * `-doubleammo` doubles ammo pickup rate in Doom and Strife (new in 5.11).
 * `-moreammo` increases ammo pickup rate by 50% in Heretic (new in 5.11).
 * `-moremana` increases mana pickup rate by 50% in Hexen (new in 5.11).
 * `-fast` enables fast monsters in Heretic and Hexen (new in 5.11).
 * `-autohealth` enables automatic use of Quartz flasks and Mystic urns in Heretic and Hexen (new in 5.11).
 * `-keysloc` enables display of keys on the automap in Heretic (new in 5.11).

### New cheat codes

 * `TNTWEAP` followed by a weapon number gives or removes this weapon (8 = Chainsaw, 9 = SSG). `TNTWEAP0` takes away all weapons and ammo except for the pistol and 50 bullets. Try to load Doom 1 with `DOOM2.WAD` as a PWAD and type `TNTWEAP9` to play the SSG in Doom 1.
 * `TNTEM`, `KILLEM` or `FHHALL` kill all monsters on the current map (and disables all cube spitters).
 * `SPECHITS` triggers all [Linedef actions](https://doomwiki.org/wiki/Linedef_type) on a map at once, no matter if they are enabled by pushing, walking over or shooting or whether they require a key or not. It also triggers all boss monster and Commander Keen actions if possible.
 * `NOTARGET` or `FHSHH` toggle deaf and blind monsters that do not act until attacked.
 * `TNTHOM` toggles the flashing [HOM](https://doomwiki.org/wiki/Hall_of_mirrors_effect) indicator (disabled by default).
 * `SHOWFPS` or `IDRATE` toggle printing the FPS in the upper right corner.
 * `NOMOMENTUM` toggles a debug aid for pixel-perfect positioning on a map (not recommended to use in-game).
 * `GOOBERS` triggers an easter egg, i.e. an "homage to an old friend". ;-)
 * `IDBEHOLD0` disables all currently active power-ups (since 2.2).
 * `IDCLEV00` restarts the current level (since 2.0).
 * `IDMUS00` restarts the current music (new in 5.1).
 * `VERSION` shows the engine version, build date and SDL version (new in 5.1).
 * `SKILL` shows the current skill level (new in 5.5.2).

## Download

Source code and Windows binaries (compatible with both x86 and x64 editions) for the latest release can be found on the [Release](https://github.com/fabiangreffrath/crispy-doom/releases/latest) page.

The most recent list of changes can be found in the [Changelog](https://github.com/fabiangreffrath/crispy-doom/blob/master/CHANGELOG.md).

A complete history of changes and releases can be found in the [Wiki](https://github.com/fabiangreffrath/crispy-doom/wiki/Changelog) or on the [Releases](https://github.com/fabiangreffrath/crispy-doom/releases) page.


To install on Ubuntu ("Eoan Ermine" 19.10 and later)/Debian ("Buster" 10 and later) based systems:
```bash
sudo apt-get install crispy-doom
```


Daily builds of Crispy Doom can be found here:
http://latest.chocolate-doom.org/


Crispy Doom can play nearly all variants of Doom. If you don't own any, you may download the [Shareware version of Doom](http://cdn.debian.net/debian/pool/non-free/d/doom-wad-shareware/doom-wad-shareware_1.9.fixed.orig.tar.gz), extract it and copy the DOOM1.WAD file into your Crispy Doom directory. Alternatively, you may want to play Crispy Doom with [Freedoom](https://www.chocolate-doom.org/wiki/index.php/Freedoom) and a MegaWAD.

### Sources
[![Open Hub](https://www.openhub.net/p/crispy-doom/widgets/project_thin_badge?style=flat&format=gif)](https://www.openhub.net/p/crispy-doom)

The Crispy Doom source code is available at GitHub: https://github.com/fabiangreffrath/crispy-doom.
It can be [downloaded in either ZIP or TAR.GZ format](https://github.com/fabiangreffrath/crispy-doom/releases) 
or cloned via

```
 git clone https://github.com/fabiangreffrath/crispy-doom.git
```

Brief instructions to set up a build system on Windows can be found [in the Crispy Doom Wiki](https://github.com/fabiangreffrath/crispy-doom/wiki/Building-on-Windows). A much more detailed guide is provided [in the Chocolate Doom Wiki](https://www.chocolate-doom.org/wiki/index.php/Building_Chocolate_Doom_on_Windows), but applies to Crispy Doom as well for most parts.

Compilation on Debian systems (Debian 10 "buster" or later) should be as simple as

```
 sudo apt install build-essential automake git
 sudo apt build-dep crispy-doom
```

to install the prerequisites and then

```
 cd crispy-doom
 autoreconf -fiv
 ./configure
 make
```

After successful compilation the resulting binaries can be found in the `src/` directory.

## More documentation

 * **[Changelog](https://github.com/fabiangreffrath/crispy-doom/wiki/Changelog)**
 * **[Compatibility](https://github.com/fabiangreffrath/crispy-doom/wiki/Compatibility)**
 * **[Crispness](https://github.com/fabiangreffrath/crispy-doom/wiki/Crispness)**
 * **[FAQ](https://github.com/fabiangreffrath/crispy-doom/wiki/FAQ)**

## Contact

The canonical homepage for Crispy Doom is https://github.com/fabiangreffrath/crispy-doom

Crispy Doom is maintained by [Fabian Greffrath](mailto:fabian@greffXremovethisXrath.com). 

Please report any bugs, glitches or crashes that you encounter to the GitHub [Issue Tracker](https://github.com/fabiangreffrath/crispy-doom/issues).

## Acknowledgement

Although I have played the thought of hacking on Chocolate Doom's renderer for quite some time already, it was Brad Harding's [Doom Retro](https://www.chocolate-doom.org/wiki/index.php/Doom_Retro) that provided the incentive to finally do it. However, his fork aims at a different direction and I did not take a single line of code from it. Lee Killough's [MBF](https://doomwiki.org/wiki/WinMBF) was studied and used to debug the code, especially in the form of Team Eternity's [WinMBF](https://doomwiki.org/wiki/WinMBF) source port, which made it easier to compile and run on my machine. And of course there is fraggle's [Chocolate Doom](https://www.chocolate-doom.org/wiki/index.php/Chocolate_Doom) with its exceptionally clean and legible source code. Please let me take this opportunity to appreciate all these authors for their work!

Also, thanks to plums of the [Doomworld forums](https://www.doomworld.com/vb/) for beta testing, "release manager" Zodomaniac and "art director" JNechaevsky for the continuous flow of support and inspiration during the post-3.x development cycle and (last but not the least) [Cacodemon9000](http://www.moddb.com/members/cacodemon9000) for his [Infested Outpost](http://www.moddb.com/games/doom-ii/addons/infested-outpost) map that helped to track down quite a few bugs!

Furthermore, thanks to VGA for his aid with adding support for his two mods: [PerK & NightFright's Black Ops smooth weapons add-on converted to DEHACKED](https://www.doomworld.com/forum/topic/84859-black-ops-smooth-weapons-dehacked-mod) and [Gifty's Smooth Doom smooth monster animations converted to DEHACKED](https://www.doomworld.com/forum/topic/85991-smoothed-smooth-monsters-for-doom-retro-and-crispy-doom) that can make the gameplay even more pleasing to the eyes.

## Legalese

Doom is © 1993-1996 Id Software, Inc.; 
Boom 2.02 is © 1999 id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman;
PrBoom+ is © 1999 id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman,
© 1999-2000 Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze,
© 2005-2006 Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko;
Chocolate Doom is © 1993-1996 Id Software, Inc., © 2005 Simon Howard; 
Chocolate Hexen is © 1993-1996 Id Software, Inc., © 1993-2008 Raven Software, © 2008 Simon Howard;
Strawberry Doom is © 1993-1996 Id Software, Inc., © 2005 Simon Howard, © 2008-2010 GhostlyDeath; 
Crispy Doom is additionally © 2014-2019 Fabian Greffrath;
all of the above are released under the [GPL-2+](https://www.gnu.org/licenses/gpl-2.0.html).

SDL 2.0, SDL_mixer 2.0 and SDL_net 2.0 are © 1997-2016 Sam Lantinga and are released under the [zlib license](http://www.gzip.org/zlib/zlib_license.html).

Secret Rabbit Code (libsamplerate) is © 2002-2011 Erik de Castro Lopo and is released under the [GPL-2+](http://www.gnu.org/licenses/gpl-2.0.html).
Libpng is © 1998-2014 Glenn Randers-Pehrson, © 1996-1997 Andreas Dilger, © 1995-1996 Guy Eric Schalnat, Group 42, Inc. and is released under the [libpng license](http://www.libpng.org/pub/png/src/libpng-LICENSE.txt).
Zlib is © 1995-2013 Jean-loup Gailly and Mark Adler and is released under the [zlib license](http://www.zlib.net/zlib_license.html).

The Crispy Doom icon (as shown at the top of this page) has been contributed by [Philip](https://github.com/kiwaph).
