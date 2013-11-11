#include "MainComponent.h"

//==============================================================================
const float MainComponent::m_fPointableRadius = 0.05f;
const float MainComponent::m_fFrameScale = 0.0075f;
const int MainComponent::timerDelay = 16;

MainComponent::MainComponent() : Component( "Main component" )
{
    init();
}

MainComponent::MainComponent(vector<string>& commands) : Component( "Main component" )
{
	init();
	setGesturesPaths(commands);
}

void MainComponent::init()
{
	setSize (500, 400);

	addAndMakeVisible (&toolbar);

	factory = new MainToolbarItemFactory(this, this);
	toolbar.addDefaultItems (*factory);
	toolbar.setBounds (getLocalBounds().removeFromTop(50));

	frameSlider.setBounds (getLocalBounds().removeFromBottom(50).removeFromRight(this->getWidth()-100));
	frameSlider.setEnabled(false);
	frameSlider.setComponentID("FRAME_SLIDER");
	frameSlider.addListener(this);
	addAndMakeVisible(&frameSlider);

	playButton.setBounds (getLocalBounds().removeFromBottom(35).removeFromRight(this->getWidth()-10).removeFromLeft(80));
	playButton.setEnabled(false);
	playButton.setSize(80, 20);
	playButton.setButtonText("play");
	playButton.setComponentID("PLAY_STOP");
	playButton.addListener(this);
	playing = false;
	addAndMakeVisible(&playButton);
	
	m_bPaused = false;
	m_bShowHelp = false;
	m_strHelp = "ESC - quit\n"
                "h - Toggle help and frame rate display\n"
                "p - Toggle pause\n"
                "Mouse Drag  - Rotate camera\n"
                "Mouse Wheel - Zoom camera\n"
                "Arrow Keys  - Rotate camera\n"
                "Space       - Reset camera";

    m_strPrompt = "Press 'h' for help";

	//OpenGl init
	m_openGLContext.setRenderer (this);
    m_openGLContext.setComponentPaintingEnabled (true);
    m_openGLContext.attachTo (*this);
    setBounds( 0, 0, 1024, 768 );
	
	initColors();
	resetCamera();

	// Keyboard
	setWantsKeyboardFocus( true );
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (Graphics& g)
{
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

	toolbar.setBounds (getLocalBounds().removeFromTop  (50));
	frameSlider.setBounds (getLocalBounds().removeFromBottom(50).removeFromRight(this->getWidth()-100));
	playButton.setBounds (getLocalBounds().removeFromBottom(35).removeFromRight(this->getWidth()-10).removeFromLeft(80));
	playButton.setSize(80, 20);
}

void MainComponent::timerCallback()
{
	int currFrame = frameSlider.getValue() - 1;
	currFrame = (currFrame + 1) % frameList.size();
	frameSlider.setValue(currFrame + 1);
}

void MainComponent::newOpenGLContextCreated()
{
	glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LESS);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);

    m_fixedFont = Font("Courier New", 24, Font::plain );
}

void MainComponent::renderOpenGL()
{
    /*double  curSysTimeSeconds = Time::highResolutionTicksToSeconds(Time::getHighResolutionTicks());
    float   fRenderDT = static_cast<float>(curSysTimeSeconds - m_fLastRenderTimeSeconds);
    fRenderDT = m_avgRenderDeltaTime.AddSample( fRenderDT );
    m_fLastRenderTimeSeconds = curSysTimeSeconds;

    float fRenderFPS = (fRenderDT > 0) ? 1.0f/fRenderDT : 0.0f;

    m_strRenderFPS = String::formatted( "RenderFPS: %4.2f", fRenderFPS );*/

    LeapUtilGL::GLMatrixScope sceneMatrixScope;

    setupScene();

    // draw the grid background
    {
        LeapUtilGL::GLAttribScope colorScope(GL_CURRENT_BIT);

        glColor3f( 0, 0, 1 );

        {
            LeapUtilGL::GLMatrixScope gridMatrixScope;

            glTranslatef( 0, 0.0f, -1.5f );

            glScalef( 3, 3, 3 );

            LeapUtilGL::drawGrid( LeapUtilGL::kPlane_XY, 20, 20 );
        }

        {
            LeapUtilGL::GLMatrixScope gridMatrixScope;

            glTranslatef( 0, -1.5f, 0.0f );

            glScalef( 3, 3, 3 );

            LeapUtilGL::drawGrid( LeapUtilGL::kPlane_ZX, 20, 20 );
        }
    }

	drawPointables((int)frameSlider.getValue() - 1);
    renderOpenGL2D();
}

void MainComponent::renderOpenGL2D() 
{
    LeapUtilGL::GLAttribScope attribScope( GL_ENABLE_BIT );

    // when enabled text draws poorly.
    //glDisable(GL_CULL_FACE);

    ScopedPointer<LowLevelGraphicsContext> glRenderer (createOpenGLGraphicsContext (m_openGLContext, getWidth(), getHeight()));

    if (glRenderer != nullptr)
    {
        Graphics g(*glRenderer.get());

        int iMargin   = 10;
        int iFontSize = static_cast<int>(m_fixedFont.getHeight());
        int iLineStep = iFontSize + (iFontSize >> 2);
        int iBaseLine = 20;
        Font origFont = g.getCurrentFont();

        const Rectangle<int>& rectBounds = getBounds();

        if ( m_bShowHelp )
        {
            g.setColour( Colours::seagreen );
            g.setFont( static_cast<float>(iFontSize) );

           /* if ( !m_bPaused )
            {
                g.drawSingleLineText( m_strUpdateFPS, iMargin, iBaseLine );
            }

            g.drawSingleLineText( m_strRenderFPS, iMargin, iBaseLine + iLineStep );*/

            g.setFont( m_fixedFont );
            g.setColour( Colours::slateblue );

            g.drawMultiLineText(  m_strHelp,
                                    iMargin,
                                    iBaseLine + iLineStep * 2,
                                    rectBounds.getWidth() - iMargin*2 );
        }

        g.setFont( origFont );
        g.setFont( static_cast<float>(iFontSize) );

        g.setColour( Colours::salmon );
        g.drawMultiLineText(  m_strPrompt,
                                iMargin,
                                rectBounds.getBottom() - (iFontSize + iFontSize + iLineStep),
                                rectBounds.getWidth()/4 );
    }
}

void MainComponent::drawPointables( int frameIndex )
{
    LeapUtilGL::GLAttribScope colorScope( GL_CURRENT_BIT | GL_LINE_BIT );

    const float fScale = m_fPointableRadius;

    glLineWidth( 3.0f );

	if (frameIndex < frameList.size())
	{
		GestureFrame currentFrame = frameList.at(frameIndex);

		for (int i = 0; i<currentFrame.getFingerCount(); i++)
		{
			Leap::Vector fingerTip(currentFrame.getFingerTipPosition(i).getX(),
				currentFrame.getFingerTipPosition(i).getY(), currentFrame.getFingerTipPosition(i).getZ());
			Leap::Vector vStartPos   = m_mtxFrameTransform.transformPoint(fingerTip * m_fFrameScale);

			glColor3fv( m_avColors[i].toFloatPointer() );
			{
                LeapUtilGL::GLMatrixScope matrixScope;

                glTranslatef( vStartPos.x, vStartPos.y, vStartPos.z );

                /*glBegin(GL_LINES);

                glVertex3f( 0, 0, 0 );
                glVertex3fv( vEndPos.toFloatPointer() );

                glEnd();*/

                glScalef( fScale, fScale, fScale );

                LeapUtilGL::drawSphere( LeapUtilGL::kStyle_Solid );
            }
		}
	}
}

void MainComponent::openGLContextClosing()
{

}

void MainComponent::setupScene()
{
    OpenGLHelpers::clear (Colours::black.withAlpha (1.0f));

    m_camera.SetAspectRatio( getWidth() / static_cast<float>(getHeight()) );

    m_camera.SetupGLProjection();

    m_camera.ResetGLView();

    // left, high, near - corner light
    LeapUtilGL::GLVector4fv vLight0Position( -3.0f, 3.0f, -3.0f, 1.0f );
    // right, near - side light
    LeapUtilGL::GLVector4fv vLight1Position(  3.0f, 0.0f, -1.5f, 1.0f );
    // near - head light
    LeapUtilGL::GLVector4fv vLight2Position( 0.0f, 0.0f,  -3.0f, 1.0f );

    /// JUCE turns off the depth test every frame when calling paint.
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GLColor(Colours::darkgrey));

    glLightfv(GL_LIGHT0, GL_POSITION, vLight0Position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, GLColor(Colour(0.5f, 0.40f, 0.40f, 1.0f)));
    glLightfv(GL_LIGHT0, GL_AMBIENT, GLColor(Colours::black));

    glLightfv(GL_LIGHT1, GL_POSITION, vLight1Position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, GLColor(Colour(0.0f, 0.0f, 0.25f, 1.0f)));
    glLightfv(GL_LIGHT1, GL_AMBIENT, GLColor(Colours::black));

    glLightfv(GL_LIGHT2, GL_POSITION, vLight2Position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, GLColor(Colour(0.15f, 0.15f, 0.15f, 1.0f)));
    glLightfv(GL_LIGHT2, GL_AMBIENT, GLColor(Colours::black));

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);

    m_camera.SetupGLView();
}

void MainComponent::resetCamera()
{
    m_camera.SetOrbitTarget( Leap::Vector::zero() );
    m_camera.SetPOVLookAt( Leap::Vector( 0, 2, 4 ), m_camera.GetOrbitTarget() );
}

void MainComponent::initColors()
{
    float fMin      = 0.0f;
    float fMax      = 1.0f;
    float fNumSteps = static_cast<float>(pow( kNumColors, 1.0/3.0 ));
    float fStepSize = (fMax - fMin)/fNumSteps;
    float fR = fMin, fG = fMin, fB = fMin;

    for ( uint32_t i = 0; i < kNumColors; i++ )
    {
    m_avColors[i] = Leap::Vector( fR, fG, LeapUtil::Min(fB, fMax) );

    fR += fStepSize;

    if ( fR > fMax )
    {
        fR = fMin;
        fG += fStepSize;

        if ( fG > fMax )
        {
        fG = fMin;
        fB += fStepSize;
        }
    }
    }

    Random rng(0x13491349);

    for ( uint32_t i = 0; i < kNumColors; i++ )
    {
    uint32_t      uiRandIdx    = i + (rng.nextInt() % (kNumColors - i));
    Leap::Vector  vSwapTemp    = m_avColors[i];

    m_avColors[i]          = m_avColors[uiRandIdx];
    m_avColors[uiRandIdx]  = vSwapTemp;
    }
}

bool MainComponent::keyPressed( const KeyPress &keyPress )
{
    int iKeyCode = toupper(keyPress.getKeyCode());

    if ( iKeyCode == KeyPress::escapeKey )
    {
		JUCEApplication::quit();
		return true;
    }
    if ( iKeyCode == KeyPress::upKey )
    {
		m_camera.RotateOrbit( 0, 0, LeapUtil::kfHalfPi * -0.05f );
		m_openGLContext.triggerRepaint();
		return true;
    }
    if ( iKeyCode == KeyPress::downKey )
    {
		m_camera.RotateOrbit( 0, 0, LeapUtil::kfHalfPi * 0.05f );
		m_openGLContext.triggerRepaint();
		return true;
    }
    if ( iKeyCode == KeyPress::leftKey )
    {
		m_camera.RotateOrbit( 0, LeapUtil::kfHalfPi * -0.05f, 0 );
		m_openGLContext.triggerRepaint();
		return true;
    }
    if ( iKeyCode == KeyPress::rightKey )
    {
		m_camera.RotateOrbit( 0, LeapUtil::kfHalfPi * 0.05f, 0 );
		m_openGLContext.triggerRepaint();
		return true;
    }

	switch( iKeyCode )
    {
		case ' ':
			resetCamera();
			break;
		case 'H':
			m_bShowHelp = !m_bShowHelp;
			break;
		case 'P':
			m_bPaused = !m_bPaused;
			break;
		default:
			return false;

    }

	m_openGLContext.triggerRepaint();
    return true;
}

void MainComponent::mouseDown (const MouseEvent& e)
{
    m_camera.OnMouseDown( LeapUtil::FromVector2( e.getPosition() ) );
}

void MainComponent::mouseDrag (const MouseEvent& e)
{
    m_camera.OnMouseMoveOrbit( LeapUtil::FromVector2( e.getPosition() ) );
    m_openGLContext.triggerRepaint();
}

void MainComponent::mouseWheelMove ( const MouseEvent& e,
                        const MouseWheelDetails& wheel )
{
    (void)e;
    m_camera.OnMouseWheel( wheel.deltaY );
    m_openGLContext.triggerRepaint();
}

void MainComponent::buttonClicked (Button* clickedButton)
{
	if (clickedButton->getComponentID() == "OPEN_GESTURES_FILES")
	{
		FileChooser fc ("Choose a gestures file to open...", File::getCurrentWorkingDirectory(), "*", true);

		if (fc.browseForMultipleFilesToOpen())
        {
			vector<string> gesturesPaths;
			for (int i = 0; i < fc.getResults().size(); ++i)
			{
				gesturesPaths.push_back(fc.getResults().getReference(i).getFullPathName().toStdString());
			}

			setGesturesPaths(gesturesPaths);
		}            
	}

	if (clickedButton->getComponentID() == "PLAY_STOP")
	{
		if(playing)
		{
			playButton.setButtonText("play");
			toolbar.setEnabled(true);
			stopTimer();
		}
		else
		{
			playButton.setButtonText("stop");
			toolbar.setEnabled(false);
			startTimer(timerDelay);
		}

		playing = !playing;
	}
}

void MainComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged->getComponentID() == "CHOOSE_GESTURE_FILE")
	{
		if(parseFile(gestures[comboBoxThatHasChanged->getSelectedId()-1]) == false)
		{
			AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          "Alert Box",
                                          "Incorrect file or file path.");
			return;
		}

		if (gestures.size() > 0)
		{
			frameSlider.setRange (1, frameList.size(), 1);
			frameSlider.setValue(1);
			frameSlider.setEnabled(true);
			playButton.setEnabled(true);
		}
		else
		{
			frameSlider.setEnabled(false);
			playButton.setEnabled(false);
			AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          "Alert Box",
                                          "File is empty");
		}
	}
}

void MainComponent::sliderValueChanged (Slider* slider)
{
	if (slider->getComponentID() == "FRAME_SLIDER")
	{

	}
}

void MainComponent::setGesturesPaths(vector<string> gesturesPaths)
{
	gestures.clear();
	gestures = gesturesPaths;

	StringArray gesturesNames;
	for (int i=0; i<gesturesPaths.size(); i++)
	{
		unsigned found = gesturesPaths.at(i).find_last_of("/\\");
		String gestureName = gesturesPaths.at(i).substr(found+1);
		gesturesNames.add(gestureName);
	}
	
	if (factory != NULL)
	{
		factory->setGesturesList(gesturesNames);
	}
}

bool MainComponent::parseFile(String filePath)
{
	fstream file;
	file.open(filePath.toStdString());

	string line;
	frameList.clear();

	if(file.is_open())
	{
		while ( getline(file,line) )
		{
			frameList.push_back(parseLine(line));
		}
	}
	else
	{
		return false;
	}

    file.close();
	
	return true;
}

GestureFrame MainComponent::parseLine(string line)
{
    vector<string> fingers;
	GestureFrame tempFrame;

	StringHelper::split(line, '#', fingers);

	for(int i=0; i<fingers.size(); i++)
	{
		if (fingers[i].size() > 0)
		{
			Position* newPos = parseFinger(fingers[i]);
			if (newPos != NULL)
			{
				tempFrame.addFingerTip(*newPos);
			}
		}
	}

	return tempFrame;
}


Position* MainComponent::parseFinger(string finger)
{
	vector<string> pos;
	StringHelper::split(finger, ';', pos);

	if (pos.size() == 3)
	{
		return new Position(strtod(pos[0].c_str(), NULL), strtod(pos[1].c_str(), NULL), strtod(pos[2].c_str(), NULL));
	}
	else
	{
		return NULL;
	}
}

