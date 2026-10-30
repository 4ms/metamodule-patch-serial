#pragma once
#include "../../mapping_ids.hh"
#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>

// The Audio Expander (MP1's): 6 ins and 8 outs
namespace MetaModule::AudioExpander
{

// The expander's jacks' IDs follow the medium panel's jacks: ins are 8..13 (after In1-6, GateIn1-2),
// and outs are 8..15 (after Out1-8)
static constexpr uint32_t FirstInJackId = 8;
static constexpr uint32_t FirstOutJackId = 8;

/// Ins

static constexpr uint32_t NumInJacks = 6;

static constexpr std::array<std::string_view, NumInJacks> InJackNames{
	"ExpIn7", "ExpIn8", "ExpIn9", "ExpIn10", "ExpIn11", "ExpIn12"};

static constexpr std::array<std::string_view, NumInJacks> InJackAbbrevs{"7", "8", "9", "10", "11", "12"};

static constexpr std::string_view get_map_injack_name(uint32_t id) {
	if (id < InJackNames.size())
		return InJackNames[id];
	return "?";
}

// 0..5 => 8..13
static constexpr uint32_t get_map_injack_num(uint32_t id) {
	id = std::clamp<uint32_t>(id, 0u, NumInJacks - 1);
	return id + FirstInJackId;
}

//// Outs

static constexpr uint32_t NumOutJacks = 8;
static constexpr uint32_t NumUserFacingOutJacks = NumOutJacks;

static constexpr std::array<std::string_view, NumUserFacingOutJacks> OutJackNames{
	"ExpOut9", "ExpOut10", "ExpOut11", "ExpOut12", "ExpOut13", "ExpOut14", "ExpOut15", "ExpOut16"};

static constexpr std::array<std::string_view, NumUserFacingOutJacks> OutJackAbbrevs{
	"9", "10", "11", "12", "13", "14", "15", "16"};

static constexpr std::string_view get_map_outjack_name(uint32_t id) {
	if (id < OutJackNames.size())
		return OutJackNames[id];
	return "?";
}

static constexpr uint32_t get_map_outjack_num(uint32_t id) {
	id = std::clamp<uint32_t>(id, 0u, NumUserFacingOutJacks);
	return id + FirstOutJackId;
}

//////////

static constexpr uint32_t exp_to_panel_input(uint32_t exp_injack_idx) {
	return exp_injack_idx + FirstInJackId;
}

static constexpr unsigned panel_to_exp_input(unsigned panel_injack_idx) {
	return panel_injack_idx - FirstInJackId;
}

static constexpr uint32_t exp_to_panel_output(uint32_t exp_outjack_idx) {
	return exp_outjack_idx + FirstOutJackId;
}

static constexpr uint32_t panel_to_exp_output(uint32_t panel_outjack_idx) {
	return panel_outjack_idx - FirstOutJackId;
}

static constexpr bool is_expander_input(unsigned panel_injack_idx) {
	return panel_injack_idx < FirstInJackId + NumInJacks &&
		   panel_injack_idx >= FirstInJackId;
}

static constexpr bool is_expander_output(unsigned panel_outjack_idx) {
	return panel_outjack_idx < FirstOutJackId + NumOutJacks &&
		   panel_outjack_idx >= FirstOutJackId;
}

static constexpr auto NumJacks = NumInJacks + NumOutJacks;

static_assert(FirstInJackId + NumInJacks - 1 <= LastPossibleJack);
static_assert(FirstOutJackId + NumOutJacks - 1 <= LastPossibleJack);

} // namespace MetaModule::AudioExpander
