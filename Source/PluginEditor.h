/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CartMouseLookAndFeel.h"

//==============================================================================
/**
*/
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class BananaSplitAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    BananaSplitAudioProcessorEditor (BananaSplitAudioProcessor&, AudioProcessorValueTreeState&);
    ~BananaSplitAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    BananaSplitAudioProcessor& processor;

    CartMouseLookAndFeel cartMouseLookAndFeel;

    AudioProcessorValueTreeState& valueTreeState;

    Slider dial{ Slider::RotaryHorizontalVerticalDrag, Slider::TextBoxBelow };
    std::unique_ptr<SliderAttachment> dialAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BananaSplitAudioProcessorEditor)
};
