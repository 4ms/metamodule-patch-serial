#pragma once
#include "../../mapping_ids.hh"
#include <cstdint>

// The Button Expanders (MP1's): up to four, with 8 buttons each.
// Expander N's buttons are IDs FirstButton + N * 8 ... + 7.
namespace MetaModule::Expander::Button
{

static constexpr unsigned NumTotalButtons = sizeof(uint32_t) * 8; //1 bit per button, and state is stored in uint32_t
static constexpr unsigned NumButtonsPerExpander = 8;			  //as defined by hardware
static constexpr unsigned MaxExpanders = NumTotalButtons / NumButtonsPerExpander;

static_assert(FirstButton + NumTotalButtons - 1 <= LastButton);

} // namespace MetaModule::Expander::Button
