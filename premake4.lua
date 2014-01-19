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

    DEPS =        "dbus-1 tcl"
    libdirs       { os.findlib("dbus-1"),
                    os.findlib("tcl") }
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
