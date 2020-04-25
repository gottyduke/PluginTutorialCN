#include "version.h"

#include <shlobj.h>	

#include "skse64/PluginAPI.h"
#include "skse64_common/skse_version.h"


namespace
{
    PluginHandle                g_thisPlugin    = kPluginHandle_Invalid;
    SKSEMessagingInterface*     g_messaging     = nullptr;
    
    void MessageHandler(SKSEMessagingInterface::Message* a_msg)
    {
        if (a_msg->type == SKSEMessagingInterface::kMessage_DataLoaded) {
            _DMESSAGE("Data Loaded");
        }
    }
}


extern "C"
{
    bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
    {
        IDebugLog::OpenRelative(CSIDL_MYDOCUMENTS, R"(\My Games\Skyrim Special Edition\SKSE\PluginTemplate.log)");
        IDebugLog::SetPrintLevel(IDebugLog::kLevel_Error);
        IDebugLog::SetLogLevel(IDebugLog::kLevel_DebugMessage);

        _MESSAGE("PluginTemplate %s", PLTP_VERSION);

        a_info->infoVersion = PluginInfo::kInfoVersion;
        a_info->name = "PluginTemplate";
        a_info->version = 1;
        
        g_thisPlugin = a_skse->GetPluginHandle();

        if (a_skse->isEditor) {
            _MESSAGE("loaded in editor, marking as incompatible");

            return false;
        }

        if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_97) {
            _FATALERROR("unsupported runtime version %08x", a_skse->runtimeVersion);

            return false;
        }
        
        g_messaging = static_cast<SKSEMessagingInterface*>(a_skse->QueryInterface(kInterface_Messaging));

        return true;
    }

    
    bool SKSEPlugin_Load(const SKSEInterface* a_skse)
    {
        _MESSAGE("PluginTemplate loaded");
        
        _MESSAGE("Hello SKSE64 !");

        if (g_messaging->RegisterListener(g_thisPlugin, "SKSE", MessageHandler)) {
            _MESSAGE("Registered messaging interface");
        } else {
            _MESSAGE("Failed to register messaging interface");

            return false;
        }
        
        return true;
    }
}