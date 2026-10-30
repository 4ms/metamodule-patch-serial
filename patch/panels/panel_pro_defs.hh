#pragma once
#include "../mapping_ids.hh"
#include "medium/audio_expander_defs.hh"
#include "medium/button_expander_defs.hh"
#include <array>
#include <cstdint>
#include <string_view>

// The pro panel (MetaModule Pro, MP2): 16 encoders, and the medium panel's jacks: 6 audio ins,
// 2 gate ins, 8 audio outs.
// ENC1..12 are the medium panel's knob IDs (A-F, u-z), so medium patches play with their knobs on
// ENC1..12. ENC13..16 are the next four IDs (the medium's first knob expander IDs).
// It takes the medium's expanders' jack and button IDs: BUT1-8 are the first Button Expander's.
struct ProPanelDef {
	static constexpr uint32_t PanelID = 2;

	// Plays patches made for the medium panel's hub
	static constexpr char typeID[20] = "HubMedium";

	static constexpr uint32_t NumAudioIn = 6;
	static constexpr uint32_t NumAudioOut = 8;
	static constexpr uint32_t NumDACOut = 0;
	static constexpr uint32_t NumPot = 0;
	static constexpr uint32_t NumEncoders = 16;
	static constexpr uint32_t NumCVIn = 0;
	static constexpr uint32_t NumGateIn = 2;
	static constexpr uint32_t NumGateOut = 0;
	static constexpr uint32_t NumMetaCV = 0;
	static constexpr uint32_t NumRgbButton = 0;
	static constexpr uint32_t NumMetaRgbButton = 1;

	static constexpr uint32_t NumKnobs = NumPot + NumEncoders;
	static constexpr int NumUserFacingInJacks = NumAudioIn + NumCVIn + NumGateIn;
	static constexpr int NumUserFacingOutJacks = NumAudioOut + NumDACOut + NumGateOut;
	static constexpr char NumJacks = NumUserFacingInJacks + NumUserFacingOutJacks + NumMetaCV;

	static constexpr std::array<std::string_view, NumKnobs> KnobNames{
		"A", "B", "C", "D", "E", "F", "G", "H", "s", "t", "u", "v", "w", "x", "y", "z"};

	static constexpr std::array<std::string_view, NumUserFacingInJacks> InJackNames{
		"In1", "In2", "In3", "In4", "In5", "In6", "GateIn1", "GateIn2"};

	static constexpr std::array<std::string_view, NumUserFacingInJacks> InJackAbbrev{
		"1", "2", "3", "4", "5", "6", "G1", "G2"};

	static constexpr std::array<std::string_view, NumUserFacingOutJacks> OutJackNames{
		"Out1", "Out2", "Out3", "Out4", "Out5", "Out6", "Out7", "Out8"};

	static constexpr std::array<std::string_view, NumUserFacingOutJacks> OutJackAbbrevs{
		"1", "2", "3", "4", "5", "6", "7", "8"};

	static constexpr bool is_encoder(uint32_t panel_knob_id) {
		return panel_knob_id < NumEncoders;
	}

	static constexpr std::string_view get_map_param_name(uint32_t id) {
		if (id < KnobNames.size())
			return KnobNames[id];
		return "?";
	}

	static constexpr std::string_view get_map_injack_name(uint32_t id) {
		if (id < NumUserFacingInJacks)
			return InJackNames[id];
		return "?";
	}

	static constexpr std::string_view get_map_injack_abbrev(uint32_t id) {
		if (id < NumUserFacingInJacks)
			return InJackAbbrev[id];
		return "?";
	}

	static constexpr std::string_view get_map_outjack_name(uint32_t id) {
		if (id < NumUserFacingOutJacks)
			return OutJackNames[id];
		return "?";
	}

	static constexpr std::string_view get_map_outjack_abbrev(uint32_t id) {
		if (id < NumUserFacingOutJacks)
			return OutJackAbbrevs[id];
		return "?";
	}

	static constexpr bool is_main_panel_input(unsigned panel_jack_idx) {
		return panel_jack_idx < NumUserFacingInJacks;
	}

	static constexpr bool is_main_panel_output(unsigned panel_jack_idx) {
		return panel_jack_idx < NumUserFacingOutJacks;
	}
};

static_assert(ProPanelDef::NumKnobs - 1 <= LastPossibleKnob);

// The jacks, and the Audio Expander's jacks after them
static_assert(ProPanelDef::NumUserFacingInJacks == MetaModule::AudioExpander::FirstInJackId);
static_assert(ProPanelDef::NumUserFacingOutJacks == MetaModule::AudioExpander::FirstOutJackId);
