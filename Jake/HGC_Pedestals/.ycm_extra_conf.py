import os


# Base flags for C++17 support and general settings
flags = [
    "-x", "c++",
    "-std=c++17",
    "-stdlib=libc++",
    "-m64",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wno-long-long",
    "-Wno-variadic-macros",
    "-fexceptions",
    "-DNDEBUG",
    "-D__CLING__",  # Define __CLING__ for ROOT macros if needed
    "-pthread",
    "-I/opt/homebrew/Cellar/root/6.32.04/include/root",
    "-L/opt/homebrew/Cellar/root/6.32.04/lib/root",
    "-lCore",
    "-lImt",
    "-lRIO",
    "-lNet",
    "-lHist",
    "-lGraf",
    "-lGraf3d",
    "-lGpad",
    "-lROOTVecOps",
    "-lTree",
    "-lTreePlayer",
    "-lRint",
    "-lPostscript",
    "-lMatrix",
    "-lPhysics",
    "-lMathCore",
    "-lThread",
    "-lMultiProc",
    "-lROOTDataFrame",
    "-Wl,-rpath,/opt/homebrew/Cellar/root/6.32.04/lib/root",
    "-stdlib=libc++",
    "-lpthread",
    "-lm",
    "-ldl"
]

def setting(filename, **kwargs):
    import ycm_core
    return {
        "flags": flags,
        "do_cache": True
    }

