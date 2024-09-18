#pragma once
#include "midi_def.hh"
#include "util/math.hh"
#include "util/static_string.hh"
#include <optional>
#include <string>
#include <vector>

constexpr int MAX_MODULES_IN_PATCH = 32;
constexpr int MAX_LIGHTS_PER_MODULE = 256;
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
	uint16_t curve_type; // reserved for future use
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
		return panel_knob_id < LastPossibleKnob;
	}

	bool is_midi_cc() const {
		return (panel_knob_id >= MidiCC0 && panel_knob_id <= MidiCC127);
	}

	uint16_t cc_num() const {
		return panel_knob_id - MidiCC0;
	}

	bool operator==(const MappedKnob &other) {
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

static_assert(sizeof(Jack) == 4, "Jack should be 4B");
static_assert(sizeof(StaticParam) == 8, "StaticParam should be 8B");
static_assert(sizeof(AliasNameString) == 32, "AliasNameString should be 32B");
static_assert(sizeof(MappedKnob) == 48, "MappedKnob should be 48B");
static_assert(sizeof(MappedOutputJack) == 40, "MappedOutputJack should be 40B");
