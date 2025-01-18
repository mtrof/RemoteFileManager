

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 06:14:07 2038
 */
/* Compiler settings for interface.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0628 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __interface_h__
#define __interface_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __FileSystemInterface_INTERFACE_DEFINED__
#define __FileSystemInterface_INTERFACE_DEFINED__

/* interface FileSystemInterface */
/* [version][uuid] */ 

int Authenticate( 
    /* [in] */ handle_t hBinding,
    /* [string][in] */ unsigned char *Login,
    /* [string][in] */ unsigned char *Password);

unsigned int Get_File_Size( 
    /* [in] */ handle_t hBinding,
    /* [string][in] */ unsigned char *Path);

int Download_File( 
    /* [in] */ handle_t hBinding,
    /* [string][in] */ unsigned char *Path,
    /* [size_is][out][in] */ unsigned char *Arr,
    /* [in] */ long Size);

int Upload_File( 
    /* [in] */ handle_t hBinding,
    /* [string][in] */ unsigned char *Path,
    /* [size_is][out][in] */ unsigned char *Arr,
    /* [in] */ long Size);

int Delete_File( 
    /* [in] */ handle_t hBinding,
    /* [string][in] */ const unsigned char *Path);



extern RPC_IF_HANDLE FileSystemInterface_v1_0_c_ifspec;
extern RPC_IF_HANDLE FileSystemInterface_v1_0_s_ifspec;
#endif /* __FileSystemInterface_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


