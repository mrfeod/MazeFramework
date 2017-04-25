#pragma once

#if defined(MAZEFRAMEWORK_LIBRARY)
#  define MAZE_EXPORT __declspec(dllexport)
#else
#  define MAZE_EXPORT __declspec(dllimport)
#endif
