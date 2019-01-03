
// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "include/cef_app.h"
#include "include/wrapper/cef_library_loader.h"
//#include "include/cef_sandbox_mac.h"

int main(int argc, char* argv[])
{
    /*CefScopedSandboxContext sandbox_context;
    if(!sandbox_context.Initialize(argc,argv))
        return 1;
    */
    CefScopedLibraryLoader library_loader;
    if (!library_loader.LoadInHelper())
        return 1;
    
    CefMainArgs main_args(argc, argv);
    //CefRefPtr<ClientApp> app(new ClientApp);
    return CefExecuteProcess(main_args,nullptr,nullptr);
}
