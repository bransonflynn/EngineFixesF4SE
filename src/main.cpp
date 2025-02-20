#include "Internal/Callbacks/Messaging.hpp"
#include "Internal/Config/Config.hpp"
#include "Internal/Fixes/Installation.hpp"
#include "Internal/Patches/Installation.hpp"

F4SE_EXPORT constinit auto F4SEPlugin_Version = []() noexcept {
	auto data = F4SE::PluginVersionData();

	data.AuthorName(Plugin::AUTHOR);
	data.PluginName(Plugin::NAME);
	data.PluginVersion(Plugin::VERSION);

	data.UsesAddressLibrary(true);
	data.IsLayoutDependent(true);
	data.UsesSigScanning(false);
	data.HasNoStructUse(false);

	data.CompatibleVersions({ F4SE::RUNTIME_LATEST_NG });
	return data;
}();

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
	logger::info("{} v{}"sv, Plugin::NAME, Plugin::VERSION);

	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Plugin::NAME.data();
	a_info->version = Plugin::VERSION.major();

	if (a_f4se->IsEditor()) {
		logger::critical("Loading in editor is not supported"sv);
		return false;
	}

	const auto version = a_f4se->RuntimeVersion();
	if (version < REL::Relocate(F4SE::RUNTIME_LATEST_OG, F4SE::RUNTIME_LATEST_NG)) {
		logger::critical("Unsupported runtime v{}"sv, version.string());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);

	// more patches/fixes are installed in Internal/Callbacks/Messaging
	logger::info("---"sv);
	logger::info("Registering for F4SE events..."sv);
	F4SE::GetMessagingInterface()->RegisterListener(Internal::Messaging::Callback);
	logger::info("Registered for F4SE events."sv);
	logger::info("---"sv);

	Internal::Config::Load();
	logger::info("---"sv);

	Internal::Fixes::Install();
	logger::info("---"sv);

	Internal::Patches::Install();
	logger::info("---"sv);

	logger::info("Loaded"sv);
	logger::info("---"sv);

	return true;
}
