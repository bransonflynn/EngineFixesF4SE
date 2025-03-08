#include "Internal/Warnings/RogueMasterWarning.hpp"
#include "Internal/Config.hpp"

// # Logs and reports any master files that load after the mods that require them.
// # Newer versions of MO2 typically prevent this from happening, but it can still occur sometimes.
// # (default: enabled | Supports both versions)
// bRogueMasterWarning=true

namespace Internal::Warnings
{
	void RogueMasterWarning::Install() noexcept
	{
		logger::info(FMT_STRING("Warning installing: RogueMasterWarning."sv));

		if (!Config::bRogueMasterWarning.GetValue()) {
			logger::info(FMT_STRING("Fix aborted: RogueMasterWarning. Reason: Fix was disabled in config file."sv));
			return;
		}

		CheckRogueMasters();

		logger::info(FMT_STRING("Warning installed: RogueMasterWarning."sv));
	}

	void RogueMasterWarning::CheckRogueMasters()
	{
		logger::info(FMT_STRING("RogueMasterWarning -> CheckRogueMasters started..."sv));

		bool foundRogueMasters = false;
		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (!dataHandler) {
			logger::info(FMT_STRING("RogueMasterWarning -> dataHandler was null"sv));
			return;
		}

		std::unordered_map<std::string_view, uint32_t> loadOrder;
		std::vector<std::string_view> rogueMasters;
		auto idx = 0;

		// populate loadorder
		for (auto& mods = dataHandler->files; const auto mod : mods) {
			loadOrder.emplace(mod->GetFilename(), idx++);
		}

		// check master order
		for (auto& mods = dataHandler->files; const auto mod : mods) {
			if (mod->masterCount > 0) {
				for (const std::string_view masterName : mod->masters) {
					auto master = dataHandler->LookupModByName(masterName);
					if (master) {
						if (loadOrder.at(master->GetFilename()) > loadOrder.at(mod->GetFilename())) {
							foundRogueMasters = true;
							rogueMasters.emplace_back(masterName);
							logger::warn(FMT_STRING("RogueMasterWarning -> Master file: {} is loaded after {}."),
								master->GetFilename(), mod->GetFilename());
						}
					}
					else {
						logger::info(FMT_STRING("RogueMasterWarning -> a master was null"sv));
					}
				}
			}
		}

		if (foundRogueMasters) {
			std::ranges::sort(rogueMasters);
			const auto [first, last] = std::ranges::unique(rogueMasters);
			rogueMasters.erase(first, last);
			RE::ConsoleLog::GetSingleton()->AddString("EngineFixesF4SE -> Rogue Masters were detected. Check EngineFixesF4SE.log\n");
			logger::info(FMT_STRING("RogueMasterWarning -> Found rogue masters:"sv));
			for (const auto& master : rogueMasters) {
				logger::info(FMT_STRING("\t{}"), master);
			}
		}
		else {
			logger::info(FMT_STRING("RogueMasterWarning -> No rogue masters were detected"sv));
		}

		logger::info(FMT_STRING("RogueMasterWarning -> CheckRogueMasters finished."sv));
	}
}
