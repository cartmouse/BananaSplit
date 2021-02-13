/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BananaSplitAudioProcessorEditor::BananaSplitAudioProcessorEditor (BananaSplitAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTreeState (vts)
{
    // Slider
    addAndMakeVisible(dial);
    dialAttachment.reset(new SliderAttachment(valueTreeState, "dial", dial));

    setLookAndFeel(&cartMouseLookAndFeel);

    setSize (300, 300);
}

BananaSplitAudioProcessorEditor::~BananaSplitAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void BananaSplitAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (cartMouseLookAndFeel.backgroundColour);
}

void BananaSplitAudioProcessorEditor::resized()
{
    auto winHeight = getLocalBounds().getHeight();
    auto winWidth = getLocalBounds().getWidth();

    auto dialBox = getLocalBounds();
    dialBox.removeFromTop(winHeight * 0.1);
    dialBox.removeFromBottom(winHeight * 0.1);
    dialBox.removeFromLeft(winWidth * 0.05);
    dialBox.removeFromRight(winWidth * 0.05);

    const auto width = dialBox.getWidth();
    dial.setBounds(dialBox.removeFromLeft(width));
}
