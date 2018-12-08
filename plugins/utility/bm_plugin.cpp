#include "bm_plugin.hpp"
#include "../../libary/global.hpp"
#include <string>
#include <array>
#include "../../common/menu.hpp"

bm_plugin_t::bm_plugin_t()
{
	auto insults = std::vector<std::string>
	{
		"%target% XDDDDDD",
		"%target% ????",
		"my god this %target% is such a god.. at being bad",
		"you remind me of intro bots %target%",
		"%target% wtf???",
		"how much did the boost cost %target%",
		"add me after the game %target%. I'll teach u how to play",
		"%target% you're honestly trash",
		"%target% you lost that fight harder than germany lost the war",
		"%target% damn you carry... too many chromosomes",
		"l<ys %target%",
		"%target% u must sweat so much in your wheelchair right now XD",
		"%target% did your mom strap on your helmet to tight?",
		"%target% did u fall out your wheelchair or what happened?",
		"Bad player (Y) Dies constantly (Y) 12 years old (Y) Donates to twitch streamers (Y) Must be %target% (Y)"
	};

	for (auto& insult : insults)
		insult_list.emplace_back(insult);
}

auto bm_plugin_t::disrespect(const std::string& target) -> void
{
	auto mode = std::rand() % 14;

	auto needle = std::string() + "%target%";
	auto insult = bm_plugin->insult_list[mode];
	auto pos = insult.find(needle);
	auto insult_s = std::string("/all ") + insult.substr(0, pos) + target + insult.substr(pos + needle.length());
	std::transform(insult_s.begin(), insult_s.end(), insult_s.begin(), ::tolower);
	global->context()._SdkSendChatLocalPlayer(insult_s.c_str());
}

auto bm_plugin_t::update_death_list() -> void
{		
	global->context()._SdkEnumHeroes([] (void* object, unsigned net_id, void*) -> bool
	{
		auto team_id(0);
		global->context()._SdkGetObjectTeamID(object, &team_id);
		if (team_id == object_manager->local_player.team_id)
			return true;

		for (auto i = 0; i < int(bm_plugin->death_list.size()); ++i)
		{
			auto& entry = bm_plugin->death_list.at(i);
			if (object != entry.object)
				continue;

			auto delay = 4000; 
			if (!entry.was_flamed && GetTickCount() > entry.death_tick + delay)
			{
				if (menu->bm_disrespect)
					bm_plugin->disrespect(entry.name);

				entry.was_flamed = true;
			}

			if (GetTickCount() > entry.death_tick + entry.death_time * 1000)
			{
				bm_plugin->death_list.erase(bm_plugin->death_list.begin() + i--);
				global->context()._SdkConsoleWrite(std::string("Deathlist: entry removed.").c_str());
			}
				

			return true;
		}

		auto is_dead(false);
		global->context()._SdkIsObjectDead(object, &is_dead);
		if (!is_dead)
			return true;

		death_list_t dl_object;
		dl_object.object = object;
		dl_object.net_id = net_id;
		dl_object.death_tick = GetTickCount();
		global->context()._SdkGetAIDeathDuration(object, &dl_object.death_time);
		auto name = "";
		global->context()._SdkGetAIName(object, &name);
		Vector pos;
		global->context()._SdkGetObjectPosition(object,(PSDKVECTOR)&pos);
		dl_object.name = name;
		if (object_manager->local_player.object_position.DistanceTo(pos) < 2000)
		{
			if (menu->bm_mastery_emote)
				global->context()._SdkShowEmoteLocalPlayer(EMOTE_TYPE_TOGGLE);

			global->context()._SdkConsoleWrite(std::string("Deathlist: " + dl_object.name + " added.").c_str());
			bm_plugin->death_list.push_back(std::move(dl_object));
		}

		return true;
	}, nullptr);
}

std::unique_ptr<bm_plugin_t> bm_plugin = std::make_unique<bm_plugin_t>();
