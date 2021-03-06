solution "MPRISLyrics"
  configurations { "Debug", "Release" }
  location      "build"
  targetdir     "bin"

  project "MPRISLyrics"
    targetname    "mprislyrics"
    kind          "ConsoleApp"
    language      "C++"

    files         { "**.hpp",
                    "**.cpp" }
    includedirs   { "include" }

    DEPS =        "dbus-c++-1 tcl ncurses"
    libdirs       { os.findlib("dbus-c++-1"),
                    os.findlib("tcl"),
                    os.findlib("ncurses") }
    linkoptions   { "`pkg-config --libs " .. DEPS .. "`",
                    "-ltcl" }
    buildoptions  { "`pkg-config --cflags " .. DEPS .. "`",
                    "-std=c++11" }

    configuration "Debug"
      defines { "DEBUG" }
      flags   { "Symbols" }

    configuration "Release"
      defines { "NDEBUG" }
      flags   { "Optimize" }
