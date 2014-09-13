

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Aug 19 21:26:53 2010
 */
/* Compiler settings for OgreInRibbon.idl:
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


#ifndef __OgreInRibbon_h_h__
#define __OgreInRibbon_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOgreInRibbon_FWD_DEFINED__
#define __IOgreInRibbon_FWD_DEFINED__
typedef interface IOgreInRibbon IOgreInRibbon;
#endif 	/* __IOgreInRibbon_FWD_DEFINED__ */


#ifndef __COgreInRibbonDoc_FWD_DEFINED__
#define __COgreInRibbonDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class COgreInRibbonDoc COgreInRibbonDoc;
#else
typedef struct COgreInRibbonDoc COgreInRibbonDoc;
#endif /* __cplusplus */

#endif 	/* __COgreInRibbonDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __OgreInRibbon_LIBRARY_DEFINED__
#define __OgreInRibbon_LIBRARY_DEFINED__

/* library OgreInRibbon */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_OgreInRibbon;

#ifndef __IOgreInRibbon_DISPINTERFACE_DEFINED__
#define __IOgreInRibbon_DISPINTERFACE_DEFINED__

/* dispinterface IOgreInRibbon */
/* [uuid] */ 


EXTERN_C const IID DIID_IOgreInRibbon;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("23165206-081A-45B6-B638-ABCC106D38B4")
    IOgreInRibbon : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IOgreInRibbonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOgreInRibbon * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOgreInRibbon * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOgreInRibbon * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOgreInRibbon * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOgreInRibbon * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOgreInRibbon * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOgreInRibbon * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IOgreInRibbonVtbl;

    interface IOgreInRibbon
    {
        CONST_VTBL struct IOgreInRibbonVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOgreInRibbon_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOgreInRibbon_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOgreInRibbon_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOgreInRibbon_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOgreInRibbon_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOgreInRibbon_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOgreInRibbon_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IOgreInRibbon_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_COgreInRibbonDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("001D7781-B45B-44EF-996B-E43F4EAF1B94")
COgreInRibbonDoc;
#endif
#endif /* __OgreInRibbon_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


