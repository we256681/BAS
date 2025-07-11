#pragma once

#ifndef O0_EXPORT
// For exporting symbols from Windows' DLLs
#ifdef _WIN32
    #ifdef ENGINE_LIBRARY
        #define O0_EXPORT __declspec(dllexport)
    #else
        #define O0_EXPORT __declspec(dllimport)
    #endif
#else
    #define O0_EXPORT
#endif

#endif // O0_EXPORT
