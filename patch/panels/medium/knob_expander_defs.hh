#pragma once
#include "../../mapping_ids.hh"
#include <cstdint>

// The Knob Expanders (MP1's): up to four, with 8 knobs each.
// Their knobs follow the medium panel's 12 knobs: expander N's knobs are IDs 12 + N * 8 ... + 7.
namespace MetaModule::KnobExpander
{

inline constexpr uint32_t NumKnobsPerExpander = 8;
inline constexpr uint32_t MaxExpanders = 4;
inline constexpr uint32_t FirstKnobId = 12;

static_assert(FirstKnobId + NumKnobsPerExpander * MaxExpanders - 1 <= LastPossibleKnob);

} // namespace MetaModule::KnobExpander
