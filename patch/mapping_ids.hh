#pragma once

static constexpr auto MaxPanelKnobs = 12;
static constexpr auto MaxExpButtons = 32;
static constexpr auto MaxExpPots = 32;

enum {
	LastPossibleKnob = MaxPanelKnobs + MaxExpPots - 1, //43

	FirstButton = LastPossibleKnob + 1,			  //44
	LastButton = LastPossibleKnob + MaxExpButtons //75
};
