#ifndef RAAVRPNCLIENTDEFS
#define RAAVRPNCLIENTDEFS

#ifdef _WIN32
#pragma warning( disable : 4251 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4565 )
#pragma warning( disable : 4150 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4995 )

#ifdef _STATIC_RAAVRPNCLIENT
#define RAAVRPNCLIENT_DLL_DEF
#else
#ifdef RAAVRPNCLIENT_EXPORTS
#define RAAVRPNCLIENT_DLL_DEF __declspec(dllexport)
#else
#define RAAVRPNCLIENT_DLL_DEF __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"raaVRPNClientD")
#else
#pragma comment(lib,"raaVRPNClientR")
#endif
#endif 
#endif
#else
#define RAAVRPNCLIENT_DLL_DEF
#endif

#endif