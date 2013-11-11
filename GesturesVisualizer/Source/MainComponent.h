#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "Header.h"

using namespace std;

//========================================================================== ====
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component, public OpenGLRenderer,
	public ButtonListener, public ComboBoxListener, public SliderListener,
	public Timer
{
public:
    //==============================================================================
    MainComponent();
	MainComponent(vector<string>&);

    ~MainComponent();

    void paint (Graphics&);
    void resized();

	void timerCallback();

	void newOpenGLContextCreated();
	void renderOpenGL();
	void renderOpenGL2D();
	void openGLContextClosing();
	void initColors();
	
	void buttonClicked (Button*);
	void comboBoxChanged (ComboBox*);
	void sliderValueChanged (Slider*);

	bool keyPressed( const KeyPress&);
	void mouseDown (const MouseEvent&);
	void mouseDrag (const MouseEvent&);
	void mouseWheelMove ( const MouseEvent&,
							const MouseWheelDetails&);
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
	
	// CONSTANTS
	static const float m_fPointableRadius;
	static const float m_fFrameScale;
	static const int timerDelay;

	// STRUCTURES
	enum  { kNumColors = 256 };

	// VARIABLES
	Toolbar toolbar;
	MainToolbarItemFactory *factory;
	vector<string> gestures;
	vector<GestureFrame> frameList;
	
	Slider frameSlider;
	TextButton playButton;
	bool playing;

	OpenGLContext m_openGLContext;
	Font m_fixedFont;
	LeapUtilGL::CameraGL m_camera;
	Leap::Matrix m_mtxFrameTransform;
	Leap::Vector m_avColors[kNumColors];
	String m_strPrompt;
    String m_strHelp;
	bool m_bShowHelp;
	bool m_bPaused;
	
	// METHODS
	void init();

	void setGesturesPaths(vector<string>);

	bool parseFile(String filePath);
	GestureFrame parseLine(string);
	Position* parseFinger(string);

	void setupScene();
	void resetCamera();
	void drawPointables(int);
};


#endif  // MAINCOMPONENT_H_INCLUDED
