#pragma once
#include <memory>
#include "sdkapi.hpp"
#include <unordered_map>
#include "../plugins/champion/champion.hpp"

class global_t
{
public:
	explicit global_t(SDK_CONTEXT& context);
	auto context() const -> const SDK_CONTEXT&;
	auto get_champion(uint32_t name) -> champion_t*;

	static auto on_tick() -> void;

private:
	SDK_CONTEXT context_;
	std::vector<std::unique_ptr<champion_t>> supported_champions_;
};

extern std::unique_ptr<global_t> global;