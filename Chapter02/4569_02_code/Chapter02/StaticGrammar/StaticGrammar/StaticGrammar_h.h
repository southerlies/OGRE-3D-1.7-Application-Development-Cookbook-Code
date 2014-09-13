

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sat Aug 28 19:05:48 2010
 */
/* Compiler settings for StaticGrammar.idl:
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


#ifndef __StaticGrammar_h_h__
#define __StaticGrammar_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IStaticGrammar_FWD_DEFINED__
#define __IStaticGrammar_FWD_DEFINED__
typedef interface IStaticGrammar IStaticGrammar;
#endif 	/* __IStaticGrammar_FWD_DEFINED__ */


#ifndef __CStaticGrammarDoc_FWD_DEFINED__
#define __CStaticGrammarDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CStaticGrammarDoc CStaticGrammarDoc;
#else
typedef struct CStaticGrammarDoc CStaticGrammarDoc;
#endif /* __cplusplus */

#endif 	/* __CStaticGrammarDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __StaticGrammar_LIBRARY_DEFINED__
#define __StaticGrammar_LIBRARY_DEFINED__

/* library StaticGrammar */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_StaticGrammar;

#ifndef __IStaticGrammar_DISPINTERFACE_DEFINED__
#define __IStaticGrammar_DISPINTERFACE_DEFINED__

/* dispinterface IStaticGrammar */
/* [uuid] */ 


EXTERN_C const IID DIID_IStaticGrammar;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7B303943-8E53-4E9C-AAC1-09D5C7F9214F")
    IStaticGrammar : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IStaticGrammarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStaticGrammar * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStaticGrammar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStaticGrammar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStaticGrammar * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStaticGrammar * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStaticGrammar * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStaticGrammar * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IStaticGrammarVtbl;

    interface IStaticGrammar
    {
        CONST_VTBL struct IStaticGrammarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStaticGrammar_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IStaticGrammar_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IStaticGrammar_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IStaticGrammar_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IStaticGrammar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IStaticGrammar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IStaticGrammar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IStaticGrammar_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CStaticGrammarDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("09878752-4BFE-4446-9CBA-10C320525BCA")
CStaticGrammarDoc;
#endif
#endif /* __StaticGrammar_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


