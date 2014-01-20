MPRISLyrics
===========

## About
MPRISLyrics is a lyrics downloader/viewer using MPRIS D-Bus interface. It aims for full compatibility with any MPRIS-enabled players and easy extensibility using Tcl-based plugins.

## Building
A simple
```
make
```
should do the trick. The binaries will be placed in the <code>bin</code> directory. In case it doesn't work for you (or you're trying to build the program on a different platform) you might want to generate a new build script using premake4. Run
```
premake4 --help
```
to see the possible script formats.
