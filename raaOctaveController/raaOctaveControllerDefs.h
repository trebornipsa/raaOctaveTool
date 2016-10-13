#ifndef RAAOCTAVECONTROLLERDEFS
#define RAAOCTAVECONTROLLERDEFS

#ifdef _WIN32
#pragma warning( disable : 4251 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4565 )
#pragma warning( disable : 4150 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4995 )

#ifdef _STATIC_RAAOCTAVECONTROLLER
#define RAAOCTAVECONTROLLER_DLL_DEF
#else
#ifdef RAAOCTAVECONTROLLER_EXPORTS
#define RAAOCTAVECONTROLLER_DLL_DEF __declspec(dllexport)
#else
#define RAAOCTAVECONTROLLER_DLL_DEF __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"raaOctaveControllerD")
#else
#pragma comment(lib,"raaOctaveControllerR")
#endif
#endif 
#endif
#else
#define RAAOCTAVECONTROLLER_DLL_DEF
#endif

#endif