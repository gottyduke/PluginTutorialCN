#include "version.h"

#include <shlobj.h>	

#include "skse64/PluginAPI.h"
#include "skse64_common/skse_version.h"


// 匿名空间
namespace
{
	// 声明插件句柄并初始化为无效值
    PluginHandle                g_thisPlugin    = kPluginHandle_Invalid;

	// 声明接口并初始化为空指针
    SKSEMessagingInterface*     g_messaging     = nullptr;

	// 消息处理函数
    void MessageHandler(SKSEMessagingInterface::Message* a_msg)
    {
        if (a_msg->type == SKSEMessagingInterface::kMessage_DataLoaded) {
            _DMESSAGE("Data Loaded");
        }
    }
}


// 导出域
extern "C"
{
	// 校验
    bool SKSEPlugin_Query(const SKSEInterface* a_skse, PluginInfo* a_info)
    {
    	// 设立日志
        IDebugLog::OpenRelative(CSIDL_MYDOCUMENTS, R"(\My Games\Skyrim Special Edition\SKSE\PluginTemplate.log)");
        IDebugLog::SetPrintLevel(IDebugLog::kLevel_Error);
        IDebugLog::SetLogLevel(IDebugLog::kLevel_DebugMessage);

        _MESSAGE("PluginTemplate %s", PLTP_VERSION);

        // 插件信息
        a_info->infoVersion = PluginInfo::kInfoVersion;
        a_info->name = "PluginTemplate";
        a_info->version = 1;

    	// 获取插件句柄
        g_thisPlugin = a_skse->GetPluginHandle();

    	// 校验模式
        if (a_skse->isEditor) {
            _MESSAGE("loaded in editor, marking as incompatible");
            return false;
        }

    	// 校验版本
        if (a_skse->runtimeVersion != RUNTIME_VERSION_1_5_97) {
            _FATALERROR("unsupported runtime version %08x", a_skse->runtimeVersion);
            return false;
        }

    	// 获取接口实例
        g_messaging = static_cast<SKSEMessagingInterface*>(a_skse->QueryInterface(kInterface_Messaging));

        return true;
    }


	// 加载
    bool SKSEPlugin_Load(const SKSEInterface* a_skse)
    {
        _MESSAGE("PluginTemplate loaded");
        
        _MESSAGE("Hello SKSE64 !");

    	// 注册消息队列
        if (g_messaging->RegisterListener(g_thisPlugin, "SKSE", MessageHandler)) {
            _MESSAGE("Registered messaging interface");
        } else {
            _FATALERROR("Failed to register messaging interface");
            return false;
        }
        
        return true;
    }
}