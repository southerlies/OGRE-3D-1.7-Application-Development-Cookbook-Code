

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Aug 25 14:47:17 2010
 */
/* Compiler settings for DynamicGrammar.idl:
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


#ifndef __DynamicGrammar_h_h__
#define __DynamicGrammar_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDynamicGrammar_FWD_DEFINED__
#define __IDynamicGrammar_FWD_DEFINED__
typedef interface IDynamicGrammar IDynamicGrammar;
#endif 	/* __IDynamicGrammar_FWD_DEFINED__ */


#ifndef __CDynamicGrammarDoc_FWD_DEFINED__
#define __CDynamicGrammarDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CDynamicGrammarDoc CDynamicGrammarDoc;
#else
typedef struct CDynamicGrammarDoc CDynamicGrammarDoc;
#endif /* __cplusplus */

#endif 	/* __CDynamicGrammarDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DynamicGrammar_LIBRARY_DEFINED__
#define __DynamicGrammar_LIBRARY_DEFINED__

/* library DynamicGrammar */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_DynamicGrammar;

#ifndef __IDynamicGrammar_DISPINTERFACE_DEFINED__
#define __IDynamicGrammar_DISPINTERFACE_DEFINED__

/* dispinterface IDynamicGrammar */
/* [uuid] */ 


EXTERN_C const IID DIID_IDynamicGrammar;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D98A9905-73B5-41D3-BB63-B922BC49B213")
    IDynamicGrammar : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDynamicGrammarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDynamicGrammar * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDynamicGrammar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDynamicGrammar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDynamicGrammar * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDynamicGrammar * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDynamicGrammar * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDynamicGrammar * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDynamicGrammarVtbl;

    interface IDynamicGrammar
    {
        CONST_VTBL struct IDynamicGrammarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDynamicGrammar_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDynamicGrammar_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDynamicGrammar_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDynamicGrammar_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDynamicGrammar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDynamicGrammar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDynamicGrammar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDynamicGrammar_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CDynamicGrammarDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("12FA5758-39B2-4FF6-B41B-9258F7BA27E7")
CDynamicGrammarDoc;
#endif
#endif /* __DynamicGrammar_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


