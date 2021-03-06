#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem-leak-lib.h"

static GUID IID_ITest = {0, 0, 0, {0,0,0,0,0,0,0,1}};
static GUID IID_IUnknown = {0, 0, 0, {0xc0,0,0,0,0,0,0,0x46}};
static GUID IID_IDispatch = {0x00020400, 0, 0, {0xc0,0,0,0,0,0,0,0x46}};

static GUID IID_IOne = {0x78338C12, 0x1AB5, 0x435D, {0xBC, 0xDA, 0xA1, 0x74, 0x17, 0x49, 0x6F, 0x4A}};
static GUID IID_OneClsId = {0x58228253, 0xA9B1, 0x4F6C, {0xA4, 0xD2, 0x0F, 0x99, 0x7C, 0xEE, 0x74, 0xFD}};

static GUID IID_IBoxXYZ = {0xE6E1F38F, 0x9700, 0x473A, {0x98, 0x0C, 0xAA, 0x41, 0xB6, 0xFD, 0xE3, 0xE8}};

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
oneobject_QueryInterface (IUnknown *pUnk, gpointer riid, gpointer* ppv);

static HRESULT STDCALL
oneobject_AddRef (IUnknown *pUnk);

static HRESULT STDCALL
oneobject_Release (IUnknown *pUnk);

static HRESULT STDCALL
oneobject_CommandOne (IOne *pThis);

static HRESULT STDCALL
oneobject_CommandTwo (IOne *pThis, IBoxXYZ *pBox);

static HRESULT STDCALL
oneobject_Add (IOne *pThis, IBoxXYZ *pBox);

static IOneVtbl oneobject_vtable = {
	&oneobject_QueryInterface,
	&oneobject_AddRef,
	&oneobject_Release,
	&oneobject_CommandOne,
	&oneobject_CommandTwo,
	&oneobject_Add,
};

HRESULT STDCALL
memleak_create_one_object (OneObject **out)
{
	LOG("enter");
	OneObject *p = marshal_alloc (sizeof (OneObject));
	p->unk.vtbl = &oneobject_vtable.unk;
	p->refcount = 1;
	p->container = mini_container_new (5);
	*out = p;
	LOG("return");
	return S_OK;
}

static HRESULT STDCALL
oneobject_QueryInterface (IUnknown *pUnk, gpointer riid, gpointer* ppv)
{
	OneObject *obj = (OneObject*)pUnk;
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
oneobject_AddRef (IUnknown *pUnk)
{
	OneObject *obj = (OneObject*)pUnk;
	LOG ("enter");
	obj->refcount++;
	LOG ("return");
	return S_OK;
}

static void
item_destroy_func (void *item, void *user_data)
{
	IBoxXYZ *pBox = (IBoxXYZ*)item;
	pBox->vtbl->unk.Release ((IUnknown*)pBox);
	fprintf (stderr, "  Released Box %p\n", pBox);
}

static HRESULT STDCALL
oneobject_Release (IUnknown *pUnk)
{
	OneObject *obj = (OneObject*)pUnk;
	LOG ("enter");
	obj->refcount--;
	if (obj->refcount == 0) {
		LOG ("free");
		mini_container_destroy (obj->container, item_destroy_func);
		marshal_free (obj);
	}
	LOG ("return");
	return S_OK;
}

static HRESULT STDCALL
oneobject_CommandOne (IOne *pThis)
{
	LOG ("enter");
	fprintf (stderr, "  pThis = %p\n", (gpointer) pThis);
	LOG ("return");
	return S_OK;
}

static HRESULT STDCALL
oneobject_CommandTwo (IOne *pThis, IBoxXYZ *pBox)
{
	LOG ("enter");
	fprintf (stderr, "  pThis = %p, pBox = %p\n", (gpointer)pThis, (gpointer)pBox);
	LOG ("calling ThankYou");
	int res = -1;
	pBox->vtbl->ThankYou (pBox, &res);
	fprintf (stderr, "  => returned %d\n", res);
	res = -1;
	LOG ("calling ThankYou again");
	pBox->vtbl->ThankYou (pBox, &res);
	fprintf (stderr, "  => returned %d\n", res);
	
	LOG ("return");
	return S_OK;
}

static HRESULT STDCALL
oneobject_Add (IOne *pThis, IBoxXYZ *pBox)
{
	OneObject *obj = (OneObject*)pThis;
	pBox->vtbl->unk.AddRef ((IUnknown*)pBox);
	mini_container_add (obj->container, pBox);
	fprintf (stderr, "  Added Box %p\n", pBox);
	return S_OK;
}
