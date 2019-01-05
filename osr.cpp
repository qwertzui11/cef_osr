#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/wrapper/cef_library_loader.h>

#include <Ogre.h>



class RenderHandler : public Ogre::FrameListener, public CefRenderHandler
{
public:
    Ogre::TexturePtr m_renderTexture;
    Ogre::SceneNode *m_renderNode;
    
    RenderHandler(Ogre::TexturePtr texture, Ogre::SceneNode *renderNode)
    : m_renderTexture(texture)
    , m_renderNode(renderNode)
    {;}
    
    // FrameListener interface
public:
    bool frameStarted(const Ogre::FrameEvent &evt) override
    {
        if (Ogre::Root::getSingletonPtr()->getAutoCreatedWindow()->isClosed())
        {
            return false;
        }
        
        m_renderNode->yaw(Ogre::Radian(evt.timeSinceLastFrame)*Ogre::Math::PI*2.*(1./10.)); // one turn in 10sec
        
        CefDoMessageLoopWork();
        
       
        
        return true;
    }
    
    // CefRenderHandler interface
public:
    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override
    {
        rect = CefRect(0, 0, m_renderTexture->getWidth(), m_renderTexture->getHeight());
        printf("\ngetview\n");
        return true;
    }
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height) override
    {
        Ogre::HardwarePixelBufferSharedPtr texBuf = m_renderTexture->getBuffer();
        texBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
        memcpy(texBuf->getCurrentLock().data, buffer, width*height*4);
        texBuf->unlock();
    }
    
    // CefBase interface
public:
    IMPLEMENT_REFCOUNTING(RenderHandler);
    
};


// for manual render handler
class BrowserClient : public CefClient
{
public:
    BrowserClient(RenderHandler *renderHandler)
        : m_renderHandler(renderHandler)
    {;}
    
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override
    {        
        return m_renderHandler;
    }
    
    CefRefPtr<CefRenderHandler> m_renderHandler;
    
    IMPLEMENT_REFCOUNTING(BrowserClient);
};


int main(int argc, char *argv[])
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
    CefScopedLibraryLoader library_loader;
    if (!library_loader.LoadInMain())
        return 1;
#endif
    
    CefMainArgs args(argc, argv);    
    {
        CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
        command_line->InitFromArgv(argc, argv);
        
        int result = CefExecuteProcess(args, nullptr, nullptr);
        // checkout CefApp, derive it and set it as second parameter, for more control on
        // command args and resources.
        if (result >= 0) // child proccess has endend, so exit.
        {
            return result;
        }
        if (result == -1)
        {
            // we are here in the father proccess.
        }
    }
    
    {
        
        
        CefSettings settings;
                   
        // checkout detailed settings options http://magpcss.org/ceforum/apidocs/projects/%28default%29/_cef_settings_t.html
        // nearly all the settings can be set via args too.
        // settings.multi_threaded_message_loop = true; // not supported, except windows
        // CefString(&settings.browser_subprocess_path).FromASCII("sub_proccess path, by default uses and starts this executeable as child");
        // CefString(&settings.cache_path).FromASCII("");
        // CefString(&settings.log_file).FromASCII("");
        // settings.log_severity = LOGSEVERITY_DEFAULT;
        // CefString(&settings.resources_dir_path).FromASCII("");
        // CefString(&settings.locales_dir_path).FromASCII("");
        // settings.no_sandbox = true;
        // settings.windowless_rendering_enabled = true;
        
        bool result = CefInitialize(args, settings, nullptr, nullptr);
        // CefInitialize creates a sub-proccess and executes the same executeable, as calling CefInitialize, if not set different in settings.browser_subprocess_path
        // if you create an extra program just for the childproccess you only have to call CefExecuteProcess(...) in it.
        if (!result)
        {
            // handle error
            return -1;
        }
    }
  
    Ogre::Root* renderSystem(nullptr);
    Ogre::SceneManager* renderScene(nullptr);
    Ogre::TexturePtr renderTexture;
    Ogre::SceneNode *renderNode;
    // renderer
    {
        // initalise Ogre3d
        renderSystem = new Ogre::Root();
        if (!renderSystem->restoreConfig())
        {
            renderSystem->showConfigDialog();
        }
        renderSystem->initialise(true);
        
        renderScene = renderSystem->createSceneManager(Ogre::ST_GENERIC);
        renderScene->setAmbientLight(Ogre::ColourValue(1.0,1.0,1.0));
        
        Ogre::Camera* camera(renderScene->createCamera("camera"));
        camera->setAutoAspectRatio(true);
        camera->setNearClipDistance(0.1);
        camera->setFarClipDistance(100.);
        renderSystem->getAutoCreatedWindow()->addViewport(camera);
        camera->getViewport()->setBackgroundColour(Ogre::ColourValue::White);
        
        // create mesh, texture, material, node and entity
        renderTexture = Ogre::TextureManager::getSingleton().createManual(
                                                                          "texture",
                                                                          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                          Ogre::TEX_TYPE_2D, 1024, 768, 0, Ogre::PF_A8R8G8B8, Ogre::TU_DYNAMIC_WRITE_ONLY);
        
        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->create("material", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        material->getTechnique(0)->getPass(0)->setCullingMode(Ogre::CULL_NONE); // print both sides of the polygones
        material->getTechnique(0)->getPass(0)->createTextureUnitState("texture");
        
        Ogre::MeshPtr mesh = Ogre::MeshManager::getSingletonPtr()->createPlane("mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                               Ogre::Plane(Ogre::Vector3::UNIT_Z, 0), 3, 2);
        
        Ogre::Entity* entity = renderScene->createEntity(mesh);
        entity->setMaterial(material);
        
        renderNode = renderScene->getRootSceneNode()->createChildSceneNode("node", Ogre::Vector3(0., 0., -3.));
        
        renderNode->attachObject(entity);
       
    }
    
    
    RenderHandler* renderHandler;
    {
        renderHandler = new RenderHandler(renderTexture, renderNode);
        
        renderSystem->addFrameListener(renderHandler);
    }
    
    // create browser-window
    CefRefPtr<CefBrowser> browser;
    CefRefPtr<BrowserClient> browserClient;
    {
        CefWindowInfo window_info;
        CefBrowserSettings browserSettings;
        
        // browserSettings.windowless_frame_rate = 60; // 30 is default
        
        // in linux set a gtk widget, in windows a hwnd. If not available set nullptr - may cause some render errors, in context-menu and plugins.
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
        window_info.SetAsWindowless(0);         
#else
        std::size_t windowHandle = 0;
        renderSystem->getAutoCreatedWindow()->getCustomAttribute("WINDOW", &windowHandle);
        window_info.SetAsWindowless(windowHandle); 
#endif
        
        browserClient = new BrowserClient(renderHandler);
        browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "http://deanm.github.io/pre3d/monster.html", browserSettings, nullptr);
        
        
        // inject user-input by calling - non-trivial for non-windows - checkout the cefclient source and the platform specific cpp, like cefclient_osr_widget_gtk.cpp for linux
        // browser->GetHost()->SendKeyEvent(...);
        // browser->GetHost()->SendMouseMoveEvent(...);
        // browser->GetHost()->SendMouseClickEvent(...);
        // browser->GetHost()->SendMouseWheelEvent(...);
    }
    
    // start rendering and calling method RenderHandler::frameStarted
    {
        renderSystem->startRendering();
    }
    
    {
        browser = nullptr;
        browserClient = nullptr;
        CefShutdown();
        
        renderScene->destroyAllMovableObjects();
        delete renderScene;
        delete renderSystem;
        delete renderHandler;
    }
    
    return 0;
}
