#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "GestureFrame.h"
#include "MainToolbarItemFactory.h"
#include "LMRecorder.h"
#include "LMRecorderListener.h"

//========================================================================== ====
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component, public OpenGLRenderer,
	public ButtonListener, public ComboBoxListener, public SliderListener,
	public Timer, public LMRecorderListener
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
	void mouseWheelMove ( const MouseEvent&, const MouseWheelDetails&);
	
	void onGestureFrameUpdate() {
		m_openGLContext.triggerRepaint();
	}

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
	LMRecorder lmRecorder;
	
	Toolbar toolbar;
	MainToolbarItemFactory *factory;
	vector<string> gestures;
	vector<GestureFrame> frameList;
	
	bool isVisualizerMode;
	Slider frameSlider;
	TextButton playButton;
	bool playing;
	bool recording;

	OpenGLContext m_openGLContext;
	Font m_fixedFont;
	LeapUtilGL::CameraGL m_camera;
	Leap::Matrix m_mtxFrameTransform;
	Leap::Vector m_avColors[kNumColors];
	String m_strPrompt;
	String m_recorderStartStr;
	String m_recorderStopStr;
    	String m_strHelp;
	bool m_bShowHelp;
	
	// METHODS
	void init();

	void setGesturesPaths(vector<string>);

	bool parseFile(String filePath);
	bool parseLine(GestureFrame&, string);
	bool parseFinger(GestureFrame&, string);

	void setupScene();
	void resetCamera();
	void drawFrame(int);
	void drawGestureFrame(GestureFrame);
	
	void startRecording();
	void stopRecording();
};


#endif  // MAINCOMPONENT_H_INCLUDED
