#ifndef RAAOCTAVEKERNELDEFS
#define RAAOCTAVEKERNELDEFS

#ifdef _WIN32
#pragma warning( disable : 4251 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4565 )
#pragma warning( disable : 4150 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4995 )

#ifdef _STATIC_RAAOCTAVEKERNEL
#define RAAOCTAVEKERNEL_DLL_DEF
#else
#ifdef RAAOCTAVEKERNEL_EXPORTS
#define RAAOCTAVEKERNEL_DLL_DEF __declspec(dllexport)
#else
#define RAAOCTAVEKERNEL_DLL_DEF __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"raaOctaveKernelD")
#else
#pragma comment(lib,"raaOctaveKernelR")
#endif
#endif 
#endif
#else
#define RAAOCTAVEKERNEL_DLL_DEF
#endif

#endif