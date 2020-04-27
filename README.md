DLStorm
=======

Deadline Code Storm.
In order for this package to work you must define either:

#define _DLCS_CONSOLE
or
#define _DLCS_SDL

_DLCS_CONSOLE will create a console only application
_DLCS_SDL will create a SDL application

Utilities and C++ classes with the intent to make game programming with C/C++ easier.
I will start documenting these functions and add examples soon.

Consists of various C/C++ classes and files

File		Description

dlstorm.cpp	C, various functions that make coding easier
dlstorm_lib.h	Header, creates macros and system level definitions for portability
c_net.cpp	Network class wrapper
c_log.cpp	Log class to log information into a file
c_data.cpp	Data storage class (creates .ini files with general data)
c_lua.cpp	Lua class wrapper
c_entity.cpp	Game entity class
c_waypoint.cpp	Waypoint class for use with entities
c_fmod.cpp	FMOD wrapper class
c_gaf.cpp	Game Archive File (GAF) class
c_game.cpp	In development Game class/heirarchical class structure of all the other classes
c_gfx.cpp	Graphics class wrapper (SDL/OpenGL)
c_gl3dmodel.cpp	3D Model class (OpenGL)
c_glfont.cpp	Font class (OpenGL)
c_glmouse.cpp	Mouse class (SDL/OpenGL)
c_gltexture.cpp	Texture class (OpenGL)
c_gui.cpp	GUI class (OpenGL)
c_console.cpp	Console class
c_hash.cpp	In development
c_map.cpp	In development
c_sqlite.cpp	SQLite class wrapper
c_timer.cpp	Timer class
c_gnu.cpp	Portability stuff
s_linux.cpp	Linux console keyboard stuff

 
Twitter: @Sethcoder
Email: defectiveseth@gmail.com

