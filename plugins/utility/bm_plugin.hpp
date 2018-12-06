#pragma once
#include <vector>
#include <memory>

class bm_plugin_t
{
public:
	bm_plugin_t();

	struct death_list_t
	{
		void* object;
		std::string name;
		unsigned net_id;
		float death_tick;
		float death_time;
		bool was_flamed = false;
	};

	std::vector<death_list_t> death_list;
	std::vector<std::string> insult_list; // ich mach das aus simplicity jetzt zu nem vector, array ist besser

	static auto disrespect(const std::string& target) -> void;
	static auto update_death_list() -> void;
};

extern std::unique_ptr<bm_plugin_t> bm_plugin;