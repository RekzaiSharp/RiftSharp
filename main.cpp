#include "libary/sdkapi.hpp"
#include "libary/global.hpp"


PSDK_CONTEXT SDK_CONTEXT_GLOBAL;

BOOL WINAPI DllMain(HINSTANCE hinst_dll, DWORD fdw_reason, LPVOID lpv_reserved)
{
	UNREFERENCED_PARAMETER(hinst_dll);

	if (fdw_reason != DLL_PROCESS_ATTACH)
		return TRUE;

	SDK_EXTRACT_CONTEXT(lpv_reserved);
	if (!SDK_CONTEXT_GLOBAL)
		return FALSE;

	// not even here vvv 
	if (!SDKSTATUS_SUCCESS(SdkNotifyLoadedModule("RiftSharp", SDK_VERSION)))
		return FALSE;

	global = std::make_unique<global_t>(*SDK_CONTEXT_GLOBAL);
	return TRUE;
}
