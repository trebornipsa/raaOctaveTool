#pragma once
#ifndef RAANETWORKDEFS
#define RAANETWORKDEFS

#ifdef _WIN32
#pragma warning( disable : 4251 )
#pragma warning( disable : 4786 )
#pragma warning( disable : 4503 )
#pragma warning( disable : 4565 )
#pragma warning( disable : 4150 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4995 )

#ifdef _STATIC_RAANETWORK
#define RAANETWORK_DLL_DEF
#else
#ifdef RAANETWORK_EXPORTS
#define RAANETWORK_DLL_DEF __declspec(dllexport)
#else
#define RAANETWORK_DLL_DEF __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"raaNetworkD")
#else
#pragma comment(lib,"raaNetworkR")
#endif
#endif 
#endif
#else
#define RAANETWORK_DLL_DEF
#endif

#endif