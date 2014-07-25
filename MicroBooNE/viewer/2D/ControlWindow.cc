#include "ControlWindow.h"
#include "InfoWindow.h"

#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGButtonGroup.h"
#include "TGListBox.h"
#include "TGLabel.h"
#include "TGDoubleSlider.h"

#include <iostream>
using namespace std;

ControlWindow::ControlWindow(const TGWindow *p, int w, int h)
    :TGVerticalFrame(p, w, h)
{
    fInfoWindow = new InfoWindow(this, w, 200); 
    AddFrame(fInfoWindow, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    
    // Navigation Frame
    TGHorizontalFrame *navigationFrame = new TGHorizontalFrame(this, w, 100, kFixedWidth);
    AddFrame(navigationFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    {
        fPrevButton = new TGTextButton(navigationFrame, "< Prev");
        navigationFrame->AddFrame(fPrevButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

        fEventEntry = new TGNumberEntryField(navigationFrame, -1, 0, TGNumberFormat::kNESInteger);
        fEventEntry->SetDefaultSize(80, 20);
        navigationFrame->AddFrame(fEventEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

        fNextButton = new TGTextButton(navigationFrame, "Next >");
        navigationFrame->AddFrame(fNextButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));
    }
    

    // // Display Option Selection
    // fDisplayFrame = new TGHorizontalFrame(this, w, 100);
    // AddFrame(fDisplayFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    // displayButtonGroup = new TGButtonGroup(fDisplayFrame, "Display Properties", kHorizontalFrame); 
    // fDisplayFrame->AddFrame(displayButtonGroup, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    // rawButton = new TGRadioButton(displayButtonGroup,  "Raw    "); 
    // calibButton = new TGRadioButton(displayButtonGroup, "Calib    "); 
    // hitsButton = new TGRadioButton(displayButtonGroup, "Hits    "); 
    // rawButton->SetState(kButtonDown);

    // // zoom control button frame  
    // fZoomControlFrame = new TGGroupFrame(this, "Display Options", kVerticalFrame);
    // fZoomControlFrame->SetTitlePos(TGGroupFrame::kLeft);
    // AddFrame(fZoomControlFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    TGHorizontalFrame *zoomButtonFrame = new TGHorizontalFrame(this, w, 100, kFixedWidth);
    AddFrame(zoomButtonFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    {
        fAutoZoomButton = new TGTextButton(zoomButtonFrame, "Auto Zoom");
        zoomButtonFrame->AddFrame(fAutoZoomButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

        fUnZoomButton = new TGTextButton(zoomButtonFrame, "UnZoom");
        zoomButtonFrame->AddFrame(fUnZoomButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    }


    // // XaxisFrame Frame
    // fXaxisFrame = new TGHorizontalFrame(fZoomControlFrame, w, 100);
    // fZoomControlFrame->AddFrame(fXaxisFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    // xrangeButton = new TGTextButton(fXaxisFrame, "X Range");
    // fXaxisFrame->AddFrame(xrangeButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

    // tdcMinEntry = new TGNumberEntryField(fXaxisFrame, -1, 0, TGNumberFormat::kNESInteger);
    // tdcMinEntry->SetDefaultSize(40, 20);
    // tdcMinEntry->SetIntNumber(0);
    // fXaxisFrame->AddFrame(tdcMinEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

    // tdcMaxEntry = new TGNumberEntryField(fXaxisFrame, -1, 0, TGNumberFormat::kNESInteger);
    // tdcMaxEntry->SetDefaultSize(50, 20);
    // tdcMaxEntry->SetIntNumber(3200);
    // fXaxisFrame->AddFrame(tdcMaxEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));


    // // ZaxisFrame Frame
    // fZaxisFrame = new TGHorizontalFrame(fZoomControlFrame, w, 100);
    // fZoomControlFrame->AddFrame(fZaxisFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    // zrangeButton = new TGTextButton(fZaxisFrame, "Color Scale");
    // fZaxisFrame->AddFrame(zrangeButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

    // ztColorEntry = new TGNumberEntryField(fZaxisFrame, -1, 0, TGNumberFormat::kNESInteger);
    // ztColorEntry->SetDefaultSize(40, 20);
    // ztColorEntry->SetIntNumber(0);
    // fZaxisFrame->AddFrame(ztColorEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

    // utColorEntry = new TGNumberEntryField(fZaxisFrame, -1, 0, TGNumberFormat::kNESInteger);
    // utColorEntry->SetDefaultSize(40, 20);
    // utColorEntry->SetIntNumber(0);
    // fZaxisFrame->AddFrame(utColorEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

    // vtColorEntry = new TGNumberEntryField(fZaxisFrame, -1, 0, TGNumberFormat::kNESInteger);
    // vtColorEntry->SetDefaultSize(40, 20);
    // vtColorEntry->SetIntNumber(0);
    // fZaxisFrame->AddFrame(vtColorEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));
    
    // // Induction Signal Group
    // inductionSigButtonGroup = new TGButtonGroup(fZoomControlFrame, "Show Induction Signal", kHorizontalFrame); 
    // fZoomControlFrame->AddFrame(inductionSigButtonGroup, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    // inductionSigButton_pos = new TGRadioButton(inductionSigButtonGroup, "Positive    ");
    // inductionSigButton_neg = new TGRadioButton(inductionSigButtonGroup, "Negative    ");
    // inductionSigButton_both = new TGRadioButton(inductionSigButtonGroup, "Both    ");
    // inductionSigButton_pos->SetState(kButtonDown);



    // MC group frame
    TGGroupFrame* mcGroupFrame = new TGGroupFrame(this, "Monte Carlo Truth", kVerticalFrame);
    AddFrame(mcGroupFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    {
        int mcHeightTotal = 300; // height
        int parentHeight = 40;
        int siblingHeight = mcHeightTotal - 120;
        int daughterHeight = mcHeightTotal - 40;

        TGHorizontalFrame *tracksFrame = new TGHorizontalFrame(mcGroupFrame, w, mcHeightTotal);
        mcGroupFrame->AddFrame(tracksFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
        {
            TGVerticalFrame* parentSiblingTracksFrame = new TGVerticalFrame(tracksFrame, w, mcHeightTotal);
            tracksFrame->AddFrame(parentSiblingTracksFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
            {
                TGLabel *labelParentTracks = new TGLabel(parentSiblingTracksFrame, "Parents (MeV)");
                parentSiblingTracksFrame->AddFrame(labelParentTracks, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
                
                fParentTracksListBox = new TGListBox(parentSiblingTracksFrame);
                parentSiblingTracksFrame->AddFrame(fParentTracksListBox, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
                fParentTracksListBox->Resize(w/2, parentHeight);

                TGLabel *labelSiblingTracks = new TGLabel(parentSiblingTracksFrame, "Siblings (MeV)");
                parentSiblingTracksFrame->AddFrame(labelSiblingTracks, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
                
                fSiblingTracksListBox = new TGListBox(parentSiblingTracksFrame);
                parentSiblingTracksFrame->AddFrame(fSiblingTracksListBox, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
                fSiblingTracksListBox->Resize(w/2, siblingHeight);

                fShowMCButton = new TGCheckButton(parentSiblingTracksFrame, "Show MC ");
                parentSiblingTracksFrame->AddFrame(fShowMCButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
                fShowMCButton->SetState(kButtonUp);
            }

            TGVerticalFrame* daughterTracksFrame = new TGVerticalFrame(tracksFrame, w, mcHeightTotal);
            tracksFrame->AddFrame(daughterTracksFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
            {
                TGLabel *labelDaughterTracks = new TGLabel(daughterTracksFrame, "Children (MeV)");
                daughterTracksFrame->AddFrame(labelDaughterTracks, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
                
                fDaughterTracksListBox = new TGListBox(daughterTracksFrame);
                daughterTracksFrame->AddFrame(fDaughterTracksListBox, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
                fDaughterTracksListBox->Resize(w/2, daughterHeight);
            }
        }
        TGLabel *processLabel = new TGLabel(mcGroupFrame, 
            "Process List\n\
   1.primary  2.compt  3.phot  4.annihil  5.eIoni\n\
   6.eBrem  7.conv  8.muIoni  9.muMinusCaptureAtRest\n\
   10.NeutronInelastic  11.nCapture  12.hadElastic");
        mcGroupFrame->AddFrame(processLabel, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    }

    // // Palette Selection
    // fColorScaleFrame = new TGHorizontalFrame(this, w, 100);
    // paletteButtonGroup = new TGButtonGroup(fColorScaleFrame, "Color Theme", kVerticalFrame); 
    // rainbowPaletteButton = new TGRadioButton(paletteButtonGroup, "Rainbow (Night)"); 
    // grayPaletteButton = new TGRadioButton(paletteButtonGroup, "Film (Night)"); 
    // summerPaletteButton = new TGRadioButton(paletteButtonGroup, "Sea (Day)"); 
    // grayinvPaletteButton = new TGRadioButton(paletteButtonGroup, "Ink (Day)"); 
    // rainbowPaletteButton->SetState(kButtonDown);

    // // fZTSlider = new TGDoubleVSlider(fColorScaleFrame, 90, 2, -1, kVerticalFrame, GetDefaultFrameBackground(), kTRUE);

    // fColorScaleFrame->AddFrame(paletteButtonGroup, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    // // fColorScaleFrame->AddFrame(fZTSlider, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    // AddFrame(fColorScaleFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

}

ControlWindow::~ControlWindow()
{
}
