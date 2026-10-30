#pragma once
#include "midi_def.hh"
#include <cstdint>

// The patch format's panel mapping IDs: which IDs can be knobs, buttons, and jacks.
// These just express maximum ranges and are agnostic to the actual panel/device the patch is written for.
// The MIDI mapping ids are in midi_def.hh

// MappedKnob::panel_knob_id
enum {
	// Knobs: pots, encoders, knob expanders
	LastPossibleKnob = 43,

	// Buttons: built-in, or on button expanders
	FirstButton = LastPossibleKnob + 1, //44
	LastButton = 75,
};

static constexpr auto NumTotalParams = LastButton + 1;

// MappedInputJack/MappedOutputJack::panel_jack_id: jacks, built-in or on expanders
static constexpr uint32_t LastPossibleJack = 0xFF;
static_assert(LastPossibleJack < MidiMonoNoteJack);
