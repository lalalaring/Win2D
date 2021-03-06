// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may
// not use these files except in compliance with the License. You may obtain
// a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

#include "pch.h"

#include <wrl/module.h>

extern "C" 
{
    BOOL WINAPI ProxyDllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved);
    STDAPI ProxyDllCanUnloadNow(void);
    STDAPI ProxyDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
    STDAPI ProxyDllRegisterServer(void);
    STDAPI ProxyDllUnregisterServer(void);
}

STDAPI_(BOOL)
DllMain(
    _In_     HINSTANCE inst,
    _In_     DWORD reason,
    _In_opt_ void* pvreserved
    )
{
    if (!ProxyDllMain(inst, reason, pvreserved))
        return FALSE;

    return TRUE;
}

STDAPI 
DllGetActivationFactory(
    _In_         HSTRING activatibleClassId, 
    _COM_Outptr_ IActivationFactory** factory)
{
    auto &module = ::Microsoft::WRL::Module< ::Microsoft::WRL::InProc >::GetModule();
    return module.GetActivationFactory(activatibleClassId, factory);
}

STDAPI
DllGetClassObject(
    _In_ REFCLSID clsid,
    _In_ REFIID iid,
    _Outptr_ void** obj
    )
{
    HRESULT hr;
    auto &module = ::Microsoft::WRL::Module< ::Microsoft::WRL::InProc >::GetModule();
    hr =  module.GetClassObject(clsid, iid, obj);
    if (FAILED(hr))
    {
        hr = ProxyDllGetClassObject(clsid, iid, obj);
    }

    return hr;
}

STDAPI DllCanUnloadNow()
{
    auto &module = ::Microsoft::WRL::Module< ::Microsoft::WRL::InProc >::GetModule();
    HRESULT hr = ProxyDllCanUnloadNow();
    if (hr == S_OK)
    {
        return module.Terminate() ? S_OK : S_FALSE;
    }
    return hr;
}
