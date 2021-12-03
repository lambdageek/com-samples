
#ifndef _MEM_LEAK_LIB_H
#define _MEM_LEAK_LIB_H

#include <stdint.h>

typedef void* gpointer;

#ifndef NULL
#define NULL ((void*)0)
#endif

#define API_EXPORT __attribute__((visibility("default")))

#define STDCALL /* empty */

#define S_OK 0
#define E_FAIL 0x80004005
#define E_NOTIMPL 0x80004001
#define E_NOINTERFACE 0x80004002

#define HRESULT int

typedef struct
{
	uint32_t a;
	uint16_t b;
	uint16_t c;
	uint8_t d[8];
} GUID;

typedef const GUID *REFIID;

typedef struct IUnknown IUnknown;
typedef struct IDispatch IDispatch;

typedef struct 
{
	int (STDCALL *QueryInterface)(IUnknown* pUnk, gpointer riid, gpointer* ppv);
	int (STDCALL *AddRef)(IUnknown* pUnk);
	int (STDCALL *Release)(IUnknown* pUnk);
} IUnknownVtbl;

typedef struct
{
	IUnknownVtbl iunk;
	HRESULT (STDCALL *GetTypeInfoCount)(IDispatch *iface, unsigned int *count);
	HRESULT (STDCALL *GetTypeInfo)(IDispatch *iface, unsigned int index, unsigned int lcid, gpointer *out);
	HRESULT (STDCALL *GetIDsOfNames)(IDispatch *iface, REFIID iid, gpointer names, unsigned int count, unsigned int lcid, gpointer ids);
	HRESULT (STDCALL *Invoke)(IDispatch *iface, unsigned int dispid, REFIID iid, unsigned int lcid, unsigned short flags, gpointer params, gpointer result, gpointer excepinfo, gpointer err_arg);
} IDispatchVtbl;

typedef struct IOneVtbl {
	IUnknownVtbl unk;
	HRESULT (STDCALL *CommandOne) (void);
} IOneVtbl;

struct IDispatch
{
	const IDispatchVtbl *lpVtbl;
};

struct IUnknown {
	const IUnknownVtbl *vtbl;
};

typedef struct IOne {
	const IOneVtbl *vtbl;
} IOne;

typedef struct OneObject {
	IUnknown unk;
	int refcount;
} OneObject;

API_EXPORT HRESULT STDCALL memleak_create_one_object (OneObject **out);


#endif
