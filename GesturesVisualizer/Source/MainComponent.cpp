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
	gestureStorageDriver = new BinaryFileStorageDriver();
	lmRecorder = new LMRecorder(gestureStorageDriver);
	
	setSize (500, 400);
	
	lmRecorder->addListener(this);

	addAndMakeVisible (&toolbar);

	factory = new MainToolbarItemFactory(this, this);
	toolbar.addDefaultItems (*factory);
	toolbar.setBounds (getLocalBounds().removeFromTop(50));

	isVisualizerMode = true;
	factory->changeWorkMode(isVisualizerMode);

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
	
	recording = false;
	
	m_bShowHelp = false;
	m_strHelp = "ESC - quit\n"
                "h - Toggle help and frame rate display\n"
                "p - Toggle pause\n"
                "Mouse Drag  - Rotate camera\n"
                "Mouse Wheel - Zoom camera\n"
                "Arrow Keys  - Rotate camera\n"
                "Space       - Reset camera";

	m_strPrompt = "Press 'h' for help";
	m_recorderStartStr = "Press 'Space' for start recording";
	m_recorderStopStr = "Press 'Space' for stop recording";

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
	delete gestureStorageDriver;
	delete lmRecorder;
}

void MainComponent::paint (Graphics& g)
{
}

void MainComponent::resized()
{
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

	drawFrame((int)frameSlider.getValue() - 1);
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

		    g.setFont( m_fixedFont );
		    g.setColour( Colours::slateblue );

		    g.drawMultiLineText(  m_strHelp,
					    iMargin,
					    iBaseLine + iLineStep * 2,
					    rectBounds.getWidth() - iMargin*2 );
		}

		g.setFont( origFont );
		g.setFont( static_cast<float>(iFontSize) );

		if(isVisualizerMode)
		{
			g.setColour( Colours::salmon );
			g.drawMultiLineText(  m_strPrompt,
					iMargin,
					rectBounds.getBottom() - (iFontSize + iFontSize + iLineStep),
					rectBounds.getWidth()/4 );
		}
		else
		{
			if (recording)
			{
				g.setColour( Colours::red );
				g.drawMultiLineText(  m_recorderStopStr,
					iMargin,
					rectBounds.getBottom() - (iFontSize + iFontSize + iLineStep),
					rectBounds.getWidth()/4 );			
			}
			else
			{
				g.setColour( Colours::white );
				g.drawMultiLineText(  m_recorderStartStr,
					iMargin,
					rectBounds.getBottom() - (iFontSize + iFontSize + iLineStep),
					rectBounds.getWidth()/4 );
			}
		}
	}
}

void MainComponent::drawFrame( int frameIndex )
{	
	GestureFrame currentFrame;
	
	if (isVisualizerMode && frameIndex < frameList.size())
	{
		currentFrame = frameList.at(frameIndex);
		drawGestureFrame(&currentFrame);
	}
	else if (!isVisualizerMode && recording && lmRecorder->getCurrentFrame(currentFrame))
	{
		drawGestureFrame(&currentFrame);
	}
}

void MainComponent::drawGestureFrame(GestureFrame *currentFrame)
{
	LeapUtilGL::GLAttribScope colorScope( GL_CURRENT_BIT | GL_LINE_BIT );

	const float fScale = m_fPointableRadius;
	const float fHandScale = m_fPointableRadius/3;

	glLineWidth( 3.0f );

	for (int i = 0; i<currentFrame->getHandCount(); i++)
	{
		GestureHand* currHand = currentFrame->getHand(i);
		if (currHand != NULL)
		{	
			Leap::Vector handPosition(currHand->getPalmPosition().getX(), currHand->getPalmPosition().getY(), currHand->getPalmPosition().getZ());
			Leap::Vector handStabilizedPosition(currHand->getStabilizedPalmPosition().getX(),
				currHand->getStabilizedPalmPosition().getY(), currHand->getStabilizedPalmPosition().getZ());
			Leap::Vector handNormal(currHand->getPalmNormal().getX(), currHand->getPalmNormal().getY(), currHand->getPalmNormal().getZ());
			Leap::Vector handDirection(currHand->getDirection().getX(), currHand->getDirection().getY(), currHand->getDirection().getZ());
			
			Leap::Vector vStartPos		= m_mtxFrameTransform.transformPoint(handPosition * m_fFrameScale);
			Leap::Vector vNormalEndPos	= m_mtxFrameTransform.transformDirection( handNormal ) * 0.25;
			Leap::Vector vDirectionEndPos	= m_mtxFrameTransform.transformDirection( handDirection ) * 0.25;
			
			glColor3fv( m_avColors[i].toFloatPointer() );
			{
				LeapUtilGL::GLMatrixScope matrixScope;

				glTranslatef( vStartPos.x, vStartPos.y, vStartPos.z );

				glBegin(GL_LINES);

				glVertex3f( 0, 0, 0 );
				glVertex3fv( vNormalEndPos.toFloatPointer() );

				glEnd();
				
				glBegin(GL_LINES);

				glVertex3f( 0, 0, 0 );
				glVertex3fv( vDirectionEndPos.toFloatPointer() );

				glEnd();
				
				glScalef( fHandScale, fHandScale, fHandScale );

				LeapUtilGL::drawSphere( LeapUtilGL::kStyle_Outline );
			}
			
			for (int j=0; j<currHand->getFingerCount(); j++)
			{
				GestureFinger* currFinger = currHand->getFinger(j);
				if (currFinger != NULL)
				{
					Leap::Vector fingerTipPosition(currFinger->getTipPosition().getX(), currFinger->getTipPosition().getY(), currFinger->getTipPosition().getZ());
					Leap::Vector fingerStabilizedTipPosition(currFinger->getStabilizedTipPosition().getX(),
						currFinger->getStabilizedTipPosition().getY(), currFinger->getStabilizedTipPosition().getZ());
					Leap::Vector fingerDirection(currFinger->getDirection().getX(), currFinger->getDirection().getY(), currFinger->getDirection().getZ());
					float fingerLength = currFinger->getLength();
					float fingerWidth = currFinger->getWidth();

					Leap::Vector vStartPos   = m_mtxFrameTransform.transformPoint(fingerTipPosition * m_fFrameScale);
					Leap::Vector vEndPos     = m_mtxFrameTransform.transformDirection( fingerDirection ) * -fingerLength * m_fFrameScale;

					glColor3fv( m_avColors[j].toFloatPointer() );
					{
						LeapUtilGL::GLMatrixScope matrixScope;

						glTranslatef( vStartPos.x, vStartPos.y, vStartPos.z );

						glBegin(GL_LINES);

						glVertex3f( 0, 0, 0 );
						glVertex3fv( vEndPos.toFloatPointer() );

						glEnd();

						glScalef( fScale, fScale, fScale );

						LeapUtilGL::drawSphere( LeapUtilGL::kStyle_Outline );
					}
				}
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
			if (isVisualizerMode)
			{
				resetCamera();
			}
			else
			{
				if (recording)
				{
					stopRecording();
				}
				else
				{
					startRecording();
				}
			}				
			break;
		case 'H':
			if (isVisualizerMode)
			{
				m_bShowHelp = !m_bShowHelp;
			}			
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
		FileChooser fc ("Choose a gestures files to open", File::getCurrentWorkingDirectory(), "*.lmr", false);

		if (fc.browseForMultipleFilesToOpen())
        	{
			vector<string> gesturesPaths;
			for (int i = 0; i < fc.getResults().size(); i++)
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
	else if (clickedButton->getComponentID() == "CHANGE_WORK_MODE")
	{
		stopRecording();
		isVisualizerMode = !isVisualizerMode;
		
		playButton.setVisible(isVisualizerMode);
		frameSlider.setVisible(isVisualizerMode);
		
		factory->changeWorkMode(isVisualizerMode);
	}
}

void MainComponent::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{   
	if (comboBoxThatHasChanged->getComponentID() == "CHOOSE_GESTURE_FILE")
	{
        	frameList.clear();         
		string path = gestures[comboBoxThatHasChanged->getSelectedId()-1];
		AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          "Alert Box",
                                          path);
		if(gestureStorageDriver->loadAllGestureFrames(path, frameList) == false)
		{
			AlertWindow::showMessageBoxAsync (AlertWindow::InfoIcon,
                                          "Alert Box",
                                          "Incorrect file or file path.");
			return;
		}

		if (frameList.size() > 0)
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

void MainComponent::startRecording()
{
	if (!recording)
	{
		lmRecorder->startRecording();
		recording = true;
		factory->setWorkModeButtonEnable(false);
	}
}

void MainComponent::stopRecording()
{
	if (recording)
	{
		lmRecorder->stopRecording();
	
		FileChooser fc ("Browse for gestures file to save...", File::getCurrentWorkingDirectory(), "*.lmr", false);
		if (fc.browseForFileToSave(true))
		{
			string path = fc.getResult().getFullPathName().toStdString();
		
			lmRecorder->saveRecording(path);
		}
		recording = false;
		factory->setWorkModeButtonEnable(true);
	} 
}

