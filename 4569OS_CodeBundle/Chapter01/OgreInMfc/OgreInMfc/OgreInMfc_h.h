

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Aug 19 22:58:48 2010
 */
/* Compiler settings for OgreInMfc.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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
#endif // __RPCNDR_H_VERSION__


#ifndef __OgreInMfc_h_h__
#define __OgreInMfc_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOgreInMfc_FWD_DEFINED__
#define __IOgreInMfc_FWD_DEFINED__
typedef interface IOgreInMfc IOgreInMfc;
#endif 	/* __IOgreInMfc_FWD_DEFINED__ */


#ifndef __COgreInMfcDoc_FWD_DEFINED__
#define __COgreInMfcDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class COgreInMfcDoc COgreInMfcDoc;
#else
typedef struct COgreInMfcDoc COgreInMfcDoc;
#endif /* __cplusplus */

#endif 	/* __COgreInMfcDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __OgreInMfc_LIBRARY_DEFINED__
#define __OgreInMfc_LIBRARY_DEFINED__

/* library OgreInMfc */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_OgreInMfc;

#ifndef __IOgreInMfc_DISPINTERFACE_DEFINED__
#define __IOgreInMfc_DISPINTERFACE_DEFINED__

/* dispinterface IOgreInMfc */
/* [uuid] */ 


EXTERN_C const IID DIID_IOgreInMfc;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("AD471551-0C86-4F44-A945-EA2D4B6780D8")
    IOgreInMfc : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IOgreInMfcVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOgreInMfc * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOgreInMfc * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOgreInMfc * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOgreInMfc * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOgreInMfc * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOgreInMfc * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOgreInMfc * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IOgreInMfcVtbl;

    interface IOgreInMfc
    {
        CONST_VTBL struct IOgreInMfcVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOgreInMfc_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOgreInMfc_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOgreInMfc_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOgreInMfc_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOgreInMfc_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOgreInMfc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOgreInMfc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IOgreInMfc_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_COgreInMfcDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("AC647C57-6BDF-4FF3-BE67-F21DF24A3F2D")
COgreInMfcDoc;
#endif
#endif /* __OgreInMfc_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


