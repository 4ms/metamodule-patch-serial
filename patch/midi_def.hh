#pragma once
#include "util/math.hh"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>

// Bit layout:
// 0xCNnn
//    ^---- N & 0b0111: is the Event Type (Note, Gate, Vel, Aft, Retrig, CC, GateNote, Clk)
//    ^---- N & 0b1000: 0: MIDI Omni (all channels). 1: MIDI channel is specified by bits 12-15 ("C")
//   ^----- C: MIDI Channel (0x0-0xF == 1-16). Top bit of "N" must be set if channel is used
//
// Where Nnn can be:
// 0x1nn    Note-based events
//     ^--- Polychannel: 0-7 (which poly note the event refers to)
//    ^---- Event (0:Note, 1:Gate, 2: Vel, 3: Aft, 4: Retrig)
//   ^----- 1: MIDI Note-based event
//
// 0x2nn    CC
//    ^^--- 00-7F: CC number, or 0x80 == PitchWheel
//   ^----- 2: MIDI CC
//
// 0x3nn    Gate on Note events
//    ^^--- 00-7F: note number
//   ^----- 3: MIDI GateNote (gate held high while note is pressed)
//
// 0x4nn    Clock
//    ^^--- division amount (relative to 24ppqn)
//   ^----- 4: MIDI Clock
//
//          Transport
// 0x501: Start
// 0x501: Stop
// 0x502: Continue
//
// 0x6xx: unused
// 0x7xx: unused
//
// Note: Bit 12 (0x800) must be set in order for MIDI channel to be considered valid
// Note: Channel does not have any meaning for MIDI Clock or Transport events
//

enum MidiMappings {
	MidiMonoNoteJack = 0x100,
	MidiNote2Jack,
	MidiNote3Jack,
	MidiNote4Jack,
	MidiNote5Jack,
	MidiNote6Jack,
	MidiNote7Jack,
	MidiNote8Jack,

	MidiMonoGateJack = 0x110,
	MidiGate2Jack,
	MidiGate3Jack,
	MidiGate4Jack,
	MidiGate5Jack,
	MidiGate6Jack,
	MidiGate7Jack,
	MidiGate8Jack,

	MidiMonoVelJack = 0x120,
	MidiVel2Jack,
	MidiVel3Jack,
	MidiVel4Jack,
	MidiVel5Jack,
	MidiVel6Jack,
	MidiVel7Jack,
	MidiVel8Jack,

	MidiMonoAftertouchJack = 0x130,
	MidiAftertouch2Jack,
	MidiAftertouch3Jack,
	MidiAftertouch4Jack,
	MidiAftertouch5Jack,
	MidiAftertouch6Jack,
	MidiAftertouch7Jack,
	MidiAftertouch8Jack,

	MidiMonoRetrigJack = 0x140,
	MidiRetrig2Jack,
	MidiRetrig3Jack,
	MidiRetrig4Jack,
	MidiRetrig5Jack,
	MidiRetrig6Jack,
	MidiRetrig7Jack,
	MidiRetrig8Jack,

	MidiCC0 = 0x200,
	MidiCC1, // MidiModWheelJack,
	//...
	MidiCC127 = 0x27F,
	MidiPitchWheelJack = 0x280,

	MidiGateNote0 = 0x300, //Note 0 (C-2) -> Gate
	//...
	MidiGateNote127 = 0x37F, //Note 127 (G8) -> Gate

	MidiClockJack = 0x400,			 // 24 PPQN clock (not divided)
	MidiClockDiv1Jack = 0x400 + 1,	 // 24 PPQN (alias for MidiClockJack)
	MidiClockDiv2Jack = 0x400 + 2,	 // 12 PPQN
	MidiClockDiv3Jack = 0x400 + 3,	 // 32nd note = 3 pulses
	MidiClockDiv6Jack = 0x400 + 6,	 // 16th note = 6 pulses
	MidiClockDiv12Jack = 0x400 + 12, // 8th note = 12 pulses
	MidiClockDiv24Jack = 0x400 + 24, // Quarter note = 24 pulses
	MidiClockDiv48Jack = 0x400 + 48, // Half notes = 2 quarter notes
	MidiClockDiv96Jack = 0x400 + 96, // Whole note = 4 quarter notes

	MidiStartJack = 0x500,
	MidiStopJack,
	MidiContinueJack,

	LastMidiJack
};

namespace TimingEvents
{
enum : uint8_t { Clock = 0, Start = 1, Stop = 2, Cont = 3, NumTimingEvents };
} // namespace TimingEvents

enum { MidiModWheelJack = MidiCC1 };

static constexpr unsigned MaxMidiPolyphony = 8;

static constexpr size_t NumMidiNotes = 128;
static constexpr size_t NumMidiCCs = 128;
static constexpr size_t NumMidiCCsPW = NumMidiCCs + 1; //plus pitch wheel (aka bend)
static constexpr size_t NumMidiClockJacks = MidiClockDiv96Jack - MidiClockJack + 1;

namespace MetaModule::Midi
{

static constexpr unsigned PitchBendCC = 128;

// Converts MIDI 7-bit to volts
template<unsigned MaxVolts>
constexpr float u7_to_volts(uint8_t val) {
	return (float)val / (127.f / (float)MaxVolts);
}

template<unsigned NumSemitones>
constexpr float s14_to_semitones(int16_t val) {
	return (float)val / (8192.f / ((float)NumSemitones / 12.f));
}
static_assert(s14_to_semitones<2>(-8192) == -0.16666667f);
static_assert(s14_to_semitones<2>(8192) == 0.16666667f);

// Note 60 = C5 means use the freq. set by the panel controls
constexpr float note_to_volts(uint8_t note) {
	return (note - 60) / 12.f;
}
static_assert(note_to_volts(60) == 0);
static_assert(note_to_volts(72) == 1);

// Returns 1-8 for the poly chan of a MidiMapping
constexpr std::optional<uint8_t> polychan(unsigned mapping) {
	if (mapping >= MidiMonoNoteJack && mapping < MidiCC0) {
		return std::min<uint8_t>(mapping & 0x0F, 7) + 1;
	}
	return std::nullopt;
}

static_assert(polychan(MidiMonoNoteJack).value() == 1);
static_assert(polychan(MidiNote2Jack).value() == 2);
static_assert(polychan(MidiNote8Jack).value() == 8);
static_assert(polychan(MidiRetrig2Jack).value() == 2);
static_assert(polychan(MidiRetrig8Jack).value() == 8);

constexpr uint32_t strip_midi_channel(uint32_t panel_jack_id) {
	return panel_jack_id & 0x07FF; //clear channel bits and omni bit
}

// Returns 0 for Omni, or 1-16 for MIDI channel 1-16
constexpr uint32_t midi_channel(uint32_t panel_jack_id) {
	if ((panel_jack_id & 0x0800) && strip_midi_channel(panel_jack_id) < 0x400)
		return ((panel_jack_id >> 12) & 0xF) + 1; //1-16
	else
		return 0;
}

// midi_chan: 1-16 for a MIDI Channel. 0 for Omni
constexpr MidiMappings set_midi_channel(uint32_t panel_jack_id, uint32_t midi_chan) {
	if (midi_chan >= 1 && midi_chan <= 16)
		return MidiMappings(strip_midi_channel(panel_jack_id) | 0x0800 | ((midi_chan - 1) << 12));
	else
		return MidiMappings(strip_midi_channel(panel_jack_id));
}

constexpr std::optional<uint32_t> midi_note_pitch(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiMonoNoteJack, MidiNote8Jack);
}

constexpr std::optional<uint32_t> midi_note_gate(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiMonoGateJack, MidiGate8Jack);
}

constexpr std::optional<uint32_t> midi_note_vel(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiMonoVelJack, MidiVel8Jack);
}

constexpr std::optional<uint32_t> midi_note_aft(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiMonoAftertouchJack, MidiAftertouch8Jack);
}

constexpr std::optional<uint32_t> midi_note_retrig(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiMonoRetrigJack, MidiRetrig8Jack);
}

constexpr std::optional<uint32_t> midi_gate(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiGateNote0, MidiGateNote127);
}

constexpr std::optional<uint32_t> midi_cc(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiCC0, MidiPitchWheelJack);
}

constexpr std::optional<uint32_t> midi_clk(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return panel_jack_id == MidiClockJack ? std::optional<uint32_t>{0} : std::nullopt;
}

constexpr std::optional<uint32_t> midi_divclk(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiClockDiv1Jack, MidiClockDiv96Jack);
}

constexpr std::optional<uint32_t> midi_transport(uint32_t panel_jack_id) {
	panel_jack_id = strip_midi_channel(panel_jack_id);
	return MathTools::between<uint32_t>(panel_jack_id, MidiStartJack, MidiContinueJack);
}

}; // namespace MetaModule::Midi
