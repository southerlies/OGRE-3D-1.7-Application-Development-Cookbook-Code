

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Aug 26 08:38:58 2010
 */
/* Compiler settings for KeyboardInput.idl:
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


#ifndef __KeyboardInput_h_h__
#define __KeyboardInput_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IKeyboardInput_FWD_DEFINED__
#define __IKeyboardInput_FWD_DEFINED__
typedef interface IKeyboardInput IKeyboardInput;
#endif 	/* __IKeyboardInput_FWD_DEFINED__ */


#ifndef __CKeyboardInputDoc_FWD_DEFINED__
#define __CKeyboardInputDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CKeyboardInputDoc CKeyboardInputDoc;
#else
typedef struct CKeyboardInputDoc CKeyboardInputDoc;
#endif /* __cplusplus */

#endif 	/* __CKeyboardInputDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __KeyboardInput_LIBRARY_DEFINED__
#define __KeyboardInput_LIBRARY_DEFINED__

/* library KeyboardInput */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_KeyboardInput;

#ifndef __IKeyboardInput_DISPINTERFACE_DEFINED__
#define __IKeyboardInput_DISPINTERFACE_DEFINED__

/* dispinterface IKeyboardInput */
/* [uuid] */ 


EXTERN_C const IID DIID_IKeyboardInput;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DB130AF4-AE74-4E06-8933-9DF7861C4910")
    IKeyboardInput : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IKeyboardInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IKeyboardInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IKeyboardInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IKeyboardInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IKeyboardInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IKeyboardInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IKeyboardInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IKeyboardInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IKeyboardInputVtbl;

    interface IKeyboardInput
    {
        CONST_VTBL struct IKeyboardInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IKeyboardInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IKeyboardInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IKeyboardInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IKeyboardInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IKeyboardInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IKeyboardInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IKeyboardInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IKeyboardInput_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CKeyboardInputDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("73B476C4-E62C-4BDA-B7EA-67647CC45592")
CKeyboardInputDoc;
#endif
#endif /* __KeyboardInput_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


