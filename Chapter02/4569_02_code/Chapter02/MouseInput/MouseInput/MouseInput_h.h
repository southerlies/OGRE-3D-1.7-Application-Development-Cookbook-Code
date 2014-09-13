

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Aug 26 08:55:18 2010
 */
/* Compiler settings for MouseInput.idl:
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


#ifndef __MouseInput_h_h__
#define __MouseInput_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMouseInput_FWD_DEFINED__
#define __IMouseInput_FWD_DEFINED__
typedef interface IMouseInput IMouseInput;
#endif 	/* __IMouseInput_FWD_DEFINED__ */


#ifndef __CMouseInputDoc_FWD_DEFINED__
#define __CMouseInputDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CMouseInputDoc CMouseInputDoc;
#else
typedef struct CMouseInputDoc CMouseInputDoc;
#endif /* __cplusplus */

#endif 	/* __CMouseInputDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __MouseInput_LIBRARY_DEFINED__
#define __MouseInput_LIBRARY_DEFINED__

/* library MouseInput */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_MouseInput;

#ifndef __IMouseInput_DISPINTERFACE_DEFINED__
#define __IMouseInput_DISPINTERFACE_DEFINED__

/* dispinterface IMouseInput */
/* [uuid] */ 


EXTERN_C const IID DIID_IMouseInput;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("25D93ED6-3EA0-4047-BCEF-13E04D6C4943")
    IMouseInput : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMouseInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMouseInput * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMouseInput * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMouseInput * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMouseInput * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMouseInput * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMouseInput * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMouseInput * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IMouseInputVtbl;

    interface IMouseInput
    {
        CONST_VTBL struct IMouseInputVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMouseInput_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMouseInput_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMouseInput_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMouseInput_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMouseInput_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMouseInput_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMouseInput_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMouseInput_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CMouseInputDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("D8B75C1D-EAFC-4374-BE60-8180268994BA")
CMouseInputDoc;
#endif
#endif /* __MouseInput_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


