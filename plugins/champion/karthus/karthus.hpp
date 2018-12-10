#pragma once
#include "../champion.hpp"

class karthus_t : public champion_t
{
public:
	karthus_t();
	virtual auto on_tick() -> void;
	virtual auto menu() -> void;

	//spells
	auto spell_laywaste() -> void;

	//Menu
	bool menu_visible;
};