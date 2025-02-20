#include "Internal/Fixes/PerkEntryPoints/ApplySpellsFix.hpp"
#include "Internal/Config/Config.hpp"

// fixes entrypoint kApplyCombatHitSpell so multiple spells can be applied at once
namespace Internal::Fixes::PerkEntryPoints::ApplySpellsFix
{

	void ApplyCombatHitSpell(REX::Enum<RE::BGSPerkEntry::EntryPoint, std::uint8_t> entryPoint,
		RE::Actor* perkOwner,
		...);

	void Install() noexcept
	{
		logger::info("Fix installing: ApplySpellsFix."sv);
		
		if (!Config::bApplySpellsFix.GetValue()) {
			logger::info("Fix aborted: ApplySpellsFix. Reason: Fix was disabled in ini file."sv);
			return;
		}

		// F4SE::Trampoline& trampoline = F4SE::GetTrampoline();

		if (Config::bIsNG == true) {
			// NG Patch
			logger::info("Fix aborted: ApplySpellsFix. Reason: Game version was NG."sv);
			return;
		}
		else {
			// OG Patch
			// REL::Relocation<uintptr_t> ptr_BGSEntryPoint_HandleEntryPoint_OG{ REL::ID(714336) };
			// trampoline.write_branch<5>(ptr_BGSEntryPoint_HandleEntryPoint_OG.address(), &ApplyCombatHitSpell);
		}

		logger::info("Fix installed: ApplySpellsFix."sv);
	}

	// TODO
	// void ApplyCombatHitSpell(REX::Enum<RE::BGSPerkEntry::EntryPoint, std::uint8_t> entryPoint, RE::Actor* perkOwner, ...)
	// {
	// 	std::va_list variadicArguments;
	// 	va_start(variadicArguments, perkOwner);

	// 	// auto* weapon = va_arg(variadicArguments, RE::TESObjectWEAP*);
	// 	// auto* target = va_arg(variadicArguments, RE::Actor*);
	// 	// auto** result = va_arg(variadicArguments, RE::SpellItem**);

	// 	va_end(variadicArguments);

	// 	// auto combatHitSpellItems = ApplySpellsFix::HandleEntryPoint(entryPoint, perkOwner, std::vector<RE::TESForm*>{ perkOwner, weapon, target });

	// 	// for (auto* combatHitSpellItem : combatHitSpellItems) {
	// 	// 	if (combatHitSpellItem) {
	// 	// 		// static_cast<RE::SpellItem*>(combatHitSpellItem)->Cast(perkOwner, target);
	// 	// 	}
	// 	// }
	// }

	// TODO
	// std::vector<void*> HandleEntryPoint(
	// 	REX::Enum<RE::BGSPerkEntry::EntryPoint, std::uint8_t> entryPoint,
	// 	RE::Actor* perkOwner,
	// 	std::vector<RE::TESForm*> conditionFilterArguments)
	// {
	// 	kSetFatigueToAPMult since it is the highest enum value
	// 	if (entryPoint.underlying() <= std::to_underlying(RE::BGSEntryPoint::ENTRY_POINT::kSetFatigueToAPMult)) {
	// 		if (perkOwner && perkOwner->HasPerkEntries(entryPoint.underlying())) {
	// 			//unk04 is likely since it's the only uint32_t in perkEntries.hpp OR std::uint8_t numArgs;
	// 			if (conditionFilterArguments.size() == RE::BGSEntryPoint::ENTRY_POINT(entryPoint)) {
	// 				std::vector<void*> entryPointFunctionArguments = { std::numeric_limits<std::uint8_t>::max(), nullptr };
	// 			}
	// 		}
	// 	}
	// 	return {};
	// }
}

// Skyrim::BGSEntryPoint::GetEntryPoint(entryPoint)->entryPointFunctionType.get()

// std::vector<void*> ApplySpellsFix::HandleEntryPoint(
// 	Utility::Enumeration<Skyrim::BGSEntryPoint::EntryPoint, std::uint32_t> entryPoint,
// 	Skyrim::Actor*                                                         perkOwner,
// 	std::vector<Skyrim::TESForm*>                                          conditionFilterArguments)
// {
// 	if (entryPoint.underlying() < Utility::Conversion::ToUnderlying(Skyrim::BGSEntryPoint::EntryPoint::kTotal))
// 	{
// 		if (perkOwner && perkOwner->HasPerkEntries(entryPoint.get()))
// 		{
// 			if (conditionFilterArguments.size() == Skyrim::BGSEntryPoint::GetEntryPoint(entryPoint)->conditionFilterArgumentCount)
// 			{
// 				std::vector<void*> entryPointFunctionArguments = { std::numeric_limits<std::uint8_t>::max(), nullptr };

// 				Skyrim::HandleEntryPointVisitor handleEntryPointVisitor(
// 					Skyrim::BGSEntryPoint::GetEntryPoint(entryPoint)->entryPointFunctionType.get(),
// 					conditionFilterArguments.empty() ? nullptr : conditionFilterArguments.data(),
// 					entryPointFunctionArguments.empty() ? nullptr : entryPointFunctionArguments.data(),
// 					perkOwner,
// 					static_cast<std::uint8_t>(conditionFilterArguments.size()),
// 					static_cast<std::uint8_t>(entryPointFunctionArguments.size()));

// 				perkOwner->ForEachPerkEntry(entryPoint.get(), handleEntryPointVisitor);

// 				std::erase(entryPointFunctionArguments, nullptr);

// 				return entryPointFunctionArguments;
// 			}
// 		}
// 	}

// 	return {};
// }
