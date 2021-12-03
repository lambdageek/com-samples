#include "mem-leak-lib.h"

static GUID IID_ITest = {0, 0, 0, {0,0,0,0,0,0,0,1}};
static GUID IID_IUnknown = {0, 0, 0, {0xc0,0,0,0,0,0,0,0x46}};
static GUID IID_IDispatch = {0x00020400, 0, 0, {0xc0,0,0,0,0,0,0,0x46}};

HRESULT STDCALL
memleak_create_com_object (ComObject **out)
{
	*out = NULL;
	return E_NOTIMPL;
}
