#pragma once
#include "mapping_ids.hh"
#include "midi_def.hh"
#include "util/math.hh"
#include "util/static_string.hh"
#include <optional>
#include <string>
#include <vector>

constexpr unsigned MaxKnobSets = 8;

struct Jack {
	uint16_t module_id;
	uint16_t jack_id;
	bool operator==(const Jack &other) const {
		return this->module_id == other.module_id && this->jack_id == other.jack_id;
	}
};

struct StaticParam {
	uint16_t module_id;
	uint16_t param_id;
	float value;
};

using AliasNameString = StaticString<31>;

struct MappedKnob {
	uint16_t panel_knob_id;
	uint16_t module_id;
	uint16_t param_id;

	enum CurveType : uint8_t { Normal, Toggle };
	uint8_t curve_type;

	uint8_t midi_chan; //0: ignore, 1-16: only MIDI channel 1-16

	float min;
	float max;
	AliasNameString alias_name;

	// Returns the value of the mapped knob, given the panel knob value
	// Return value goes from min to max as panel_val goes from 0 to 1
	// If max < min then mapping will be reversed direction
	float get_mapped_val(float panel_val) const {
		return (max - min) * panel_val + min;
	}

	float unmap_val(float mapped_val) const {
		if (min == max)
			return 0;
		return (mapped_val - min) / (max - min);
	}

	bool is_panel_knob() const {
		return panel_knob_id < MaxPanelKnobs;
	}

	bool is_midi_cc() const {
		return (panel_knob_id >= MidiCC0 && panel_knob_id <= MidiCC127);
	}

	bool is_midi_notegate() const {
		return (panel_knob_id >= MidiGateNote0 && panel_knob_id <= MidiGateNote127);
	}

	bool is_midi() const {
		return is_midi_notegate() || is_midi_cc();
	}

	bool is_button() const {
		return panel_knob_id >= FirstButton && panel_knob_id <= LastButton;
	}

	uint16_t cc_num() const {
		return panel_knob_id - MidiCC0;
	}

	uint16_t notegate_num() const {
		return panel_knob_id - MidiGateNote0;
	}

	bool operator==(const MappedKnob &other) const {
		return maps_to_same_as(other);
	}

	bool maps_to_same_as(const MappedKnob &other) const {
		return (module_id == other.module_id) && (param_id == other.param_id);
	}
};

struct MappedKnobSet {
	std::vector<MappedKnob> set;
	AliasNameString name{};
};

struct InternalCable {
	Jack out{};
	std::vector<Jack> ins;
	std::optional<uint16_t> color{};
};

struct MappedInputJack {
	uint32_t panel_jack_id{};
	std::vector<Jack> ins;
	AliasNameString alias_name{};
};

struct MappedOutputJack {
	uint32_t panel_jack_id{};
	Jack out{};
	AliasNameString alias_name{};
};

struct ModuleInitState {
	uint32_t module_id{};
	std::string state_data;
};

struct MappedLight {
	uint32_t panel_light_id{};
	uint16_t module_id{};
	uint16_t light_id{};
};

static_assert(sizeof(Jack) == 4, "Jack should be 4B");
static_assert(sizeof(StaticParam) == 8, "StaticParam should be 8B");
static_assert(sizeof(AliasNameString) == 32, "AliasNameString should be 32B");
static_assert(sizeof(MappedKnob) == 48, "MappedKnob should be 48B");
static_assert(sizeof(MappedOutputJack) == 40, "MappedOutputJack should be 40B");
