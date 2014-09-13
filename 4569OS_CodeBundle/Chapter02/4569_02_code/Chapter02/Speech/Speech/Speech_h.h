

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Aug 21 17:27:51 2010
 */
/* Compiler settings for Speech.idl:
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


#ifndef __Speech_h_h__
#define __Speech_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISpeech_FWD_DEFINED__
#define __ISpeech_FWD_DEFINED__
typedef interface ISpeech ISpeech;
#endif 	/* __ISpeech_FWD_DEFINED__ */


#ifndef __CSpeechDoc_FWD_DEFINED__
#define __CSpeechDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CSpeechDoc CSpeechDoc;
#else
typedef struct CSpeechDoc CSpeechDoc;
#endif /* __cplusplus */

#endif 	/* __CSpeechDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Speech_LIBRARY_DEFINED__
#define __Speech_LIBRARY_DEFINED__

/* library Speech */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Speech;

#ifndef __ISpeech_DISPINTERFACE_DEFINED__
#define __ISpeech_DISPINTERFACE_DEFINED__

/* dispinterface ISpeech */
/* [uuid] */ 


EXTERN_C const IID DIID_ISpeech;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("EE3E3D41-A623-40D1-8BC9-C5EB88E720A6")
    ISpeech : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ISpeechVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISpeech * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISpeech * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISpeech * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISpeech * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISpeech * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISpeech * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISpeech * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ISpeechVtbl;

    interface ISpeech
    {
        CONST_VTBL struct ISpeechVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpeech_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISpeech_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISpeech_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISpeech_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISpeech_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISpeech_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISpeech_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ISpeech_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CSpeechDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("C8ED2678-15C2-429B-A526-31AC7647C508")
CSpeechDoc;
#endif
#endif /* __Speech_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


