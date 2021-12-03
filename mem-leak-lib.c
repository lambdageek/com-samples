#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem-leak-lib.h"

static GUID IID_ITest = {0, 0, 0, {0,0,0,0,0,0,0,1}};
static GUID IID_IUnknown = {0, 0, 0, {0xc0,0,0,0,0,0,0,0x46}};
static GUID IID_IDispatch = {0x00020400, 0, 0, {0xc0,0,0,0,0,0,0,0x46}};

static GUID IID_IOne = {0x78338C12, 0x1AB5, 0x435D, {0xBC, 0xDA, 0xA1, 0x74, 0x17, 0x49, 0x6F, 0x4A}};
static GUID IID_OneClsId = {0x58228253, 0xA9B1, 0x4F6C, {0xA4, 0xD2, 0x0F, 0x99, 0x7C, 0xEE, 0x74, 0xFD}};

static void*
marshal_alloc (size_t size)
{
#ifdef WIN32
	return CoTaskMemAlloc (size);
#else
	return malloc (size);
#endif
}

static void
marshal_free (void *ptr)
{
#ifdef WIN32
	CoTaskMemFree (ptr);
#else
	free (ptr);
#endif
}

#define LOG(msg) fprintf(stderr, "%s:%d: %s %s\n", __FILE__, __LINE__, __func__, msg);

static HRESULT STDCALL
comobject_QueryInterface (IUnknown *pUnk, gpointer riid, gpointer* ppv);

static HRESULT STDCALL
comobject_AddRef (IUnknown *pUnk);

static HRESULT STDCALL
comobject_Release (IUnknown *pUnk);

static HRESULT STDCALL
comobject_CommandOne (void);

static IOneVtbl comobject_vtable = {
	&comobject_QueryInterface,
	&comobject_AddRef,
	&comobject_Release,
	&comobject_CommandOne,
};

HRESULT STDCALL
memleak_create_com_object (ComObject **out)
{
	LOG("enter");
	ComObject *p = marshal_alloc (sizeof (ComObject));
	p->unk.vtbl = &comobject_vtable.unk;
	p->refcount = 1;
	*out = p;
	LOG("return");
	return S_OK;
}

static HRESULT STDCALL
comobject_QueryInterface (IUnknown *pUnk, gpointer riid, gpointer* ppv)
{
	ComObject *obj = (ComObject*)pUnk;
	LOG ("enter");
	*ppv = NULL;
	if (!memcmp(riid, &IID_IUnknown, sizeof(GUID))) {
		*ppv = pUnk;
		pUnk->vtbl->AddRef (pUnk);
		LOG ("return IUnknown");
		return S_OK;
	}
	else if (!memcmp(riid, &IID_IOne, sizeof(GUID))) {
		*ppv = pUnk;
		pUnk->vtbl->AddRef (pUnk);
		LOG ("return IOne");
		return S_OK;
	}
#if 0
	else if (!memcmp(riid, &IID_IDispatch, sizeof(GUID))) {
		*ppv = pUnk;
		pUnk->vtbl->AddRef (pUnk);
		LOG ("return IDispatch");
		return S_OK;
	}
#endif
	LOG ("return E_NOINTERFACE");
	return E_NOINTERFACE;
}

static HRESULT STDCALL
comobject_AddRef (IUnknown *pUnk)
{
	ComObject *obj = (ComObject*)pUnk;
	LOG ("enter");
	obj->refcount++;
	LOG ("return");
	return S_OK;
}

static HRESULT STDCALL
comobject_Release (IUnknown *pUnk)
{
	ComObject *obj = (ComObject*)pUnk;
	LOG ("enter");
	obj->refcount--;
	if (obj->refcount == 0) {
		LOG ("free");
		marshal_free (obj);
	}
	LOG ("return");
	return S_OK;
}

static HRESULT STDCALL
comobject_CommandOne (void)
{
	LOG ("enter");
	LOG ("return");
	return S_OK;
}
