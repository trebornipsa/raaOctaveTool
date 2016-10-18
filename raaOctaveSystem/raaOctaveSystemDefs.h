#ifndef RAAOCTAVESYSTEMDEFS
#define RAAOCTAVESYSTEMDEFS

#ifdef _WIN32
#pragma warning( disable : 4251 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4565 )
#pragma warning( disable : 4150 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4995 )

#ifdef _STATIC_RAAOCTAVESYSTEM
#define RAAOCTAVESYSTEM_DLL_DEF
#else
#ifdef RAAOCTAVESYSTEM_EXPORTS
#define RAAOCTAVESYSTEM_DLL_DEF __declspec(dllexport)
#else
#define RAAOCTAVESYSTEM_DLL_DEF __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"raaOctaveSystemD")
#else
#pragma comment(lib,"raaOctaveSystemR")
#endif
#endif 
#endif
#else
#define RAAOCTAVESYSTEM_DLL_DEF
#endif

#endif