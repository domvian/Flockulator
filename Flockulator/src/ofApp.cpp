

/*
The aim of this application is to develop an interactive and engaging audio-visual installation. The app employs machine learning
to translate facial gestures into flocking movements. The facial data is transmitted from Max MSP to Wekinator, which has been
trained to identify various facial expressions. This connection links the facial data to distinct parameters, which alter
the behavior of the flocking algorithm. The flocking boids then regulate the granulator's parameters. The generative nature
of the algorithm will create an organic visual and auditory experience. The flocking algorithm simulates the behavior of a
group of birds, where each bird follows a set of simple rules such as alignment, cohesion, and separation.
These rules allow the birds to move together as a group while avoiding collisions with each other.
The combination of the flocking algorithm and granulator creates a unique and immersive audio-visual experience that
responds in real-time to the user's facial expressions. This application demonstrates the potential of using reactive
visuals to enhance and enrich the auditory experience*/

#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup()
{

    ofSetBackgroundColor(0);
    ofEnableNormalizedTexCoords();
    light.setPointLight();
    light.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
    light.setAmbientColor(ofFloatColor(0.3, 0.3, 0.3));
    light.setPosition(ofGetWidth() * .5, ofGetHeight() * .5, 500);

    // loading the sound file
    samp.load(ofToDataPath("split1.wav"));
    int sampleRate = 44100;
    int bufferSize = 512;

    // granulator setup
    stretches.push_back(new maxiTimePitchStretch<grainPlayerWin, maxiSample>(&samp));
    speed = 1.0;
    grainLength = 0.5;
    pitch = 1.5;
    overlaps = 4;
    current = 0;
    amount = 1000;

    ofSoundStreamSetup(2, 0, this, sampleRate, bufferSize, 4);

    // OSC setup
    osc.setup(12000);
    oscSeparation = 0.135;
    oscSpeed = 3.28;
    oscCohesion = 0.045;
    oscAlignment = 0.335;
    oscDistance = 22.5;
    oscRadius = 137.5;
    oscEdgeBounciness = 15.5845;
    oscUp = 0.0;
    oscDown = 0.0;
    oscLeft = 0.0;
    oscRight = 0.0;

    // Flocking setup
    for (int i = 0; i < amount; i++)
    {
        ofVec3f pos = ofVec3f(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
        Boid b{pos, ofColor(255, 128, 0)};
        flock.push_back(b);
    }
}

//--------------------------------------------------------------
void ofApp::update()
{

    // OSC
    while (osc.hasWaitingMessages())
    {
        // recieve the OSC messages from wekinator and assign them to variables
        ofxOscMessage m;
        osc.getNextMessage(&m);

        if (m.getAddress() == "/wek/outputs")
        {
            oscSeparation = m.getArgAsFloat(0);
            oscSpeed = m.getArgAsFloat(1);
            oscCohesion = m.getArgAsFloat(2);
            oscAlignment = m.getArgAsFloat(3);
            oscDistance = m.getArgAsFloat(4);
            oscRadius = m.getArgAsFloat(5);
            oscEdgeBounciness = m.getArgAsFloat(6);
            oscUp = m.getArgAsFloat(7);
            oscDown = m.getArgAsFloat(8);
            oscLeft = m.getArgAsFloat(9);
            oscRight = m.getArgAsFloat(10);
        }
    }

    // Loop through all the boids and update their positions and velocities
    for (int i = 0; i < flock.size(); i++)
    {
        flock[i].update(flock, oscSpeed, oscSeparation, oscCohesion, oscAlignment, oscDistance, oscRadius, oscEdgeBounciness);
    }

    // Loop through all the boids again and calculate the average position, distance from center and speed

    // calculate center of 3D space
    ofVec3f center = ofVec3f(0, 0, 0);
    float totalDistance = 0.0;
    float totalVel = 0.0;
    for (int i = 0; i < flock.size(); i++)
    {
        avgPos += flock[i].getPosition();
        xPos += flock[i].getPosition().x;
        yPos += flock[i].getPosition().y;
        float distance = flock[i].getPosition().distance(center);
        totalDistance += distance;
        ofVec3f velocity = flock[i].getVelocity();
        totalVel += velocity[0] + velocity[1] + velocity[2];
    }

    // calculate the min and max values for x, y and z positions
    float min_xPos = xPos;
    float max_xPos = xPos;
    float min_yPos = yPos;
    float max_yPos = yPos;

    for (int i = 0; i < flock.size(); i++)
    {
        if (flock[i].getPosition().x < min_xPos)
        {
            min_xPos = flock[i].getPosition().x;
        }
        if (flock[i].getPosition().x > max_xPos)
        {
            max_xPos = flock[i].getPosition().x;
        }
        if (flock[i].getPosition().y < min_yPos)
        {
            min_yPos = flock[i].getPosition().y;
        }
        if (flock[i].getPosition().y > max_yPos)
        {
            max_yPos = flock[i].getPosition().y;
        }
    }

    // calculate average distance
    float averageDistance = totalDistance / flock.size();
    float avgVel = totalVel / flock.size();

    // map the average values to the granulator parameters
    pitch = ofMap(avgVel, -1.0, 1.0, 0.2, 5);
    overlaps = ofMap(averageDistance, 0, 150, 8, 12);
    speed = ofMap(avgPos.x, min_xPos, max_xPos, 0.5, 1.0);
    grainLength = ofMap(avgPos.y, min_yPos, max_yPos, 0.05, 0.5);
}
//--------------------------------------------------------------
void ofApp::draw()
{

    ofEnableDepthTest();
    cam.begin();
    ofEnableLighting();
    light.enable();

    // draw the boids
    for (int i = 0; i < flock.size(); i++)
    {
        flock[i].draw();
    }

    light.disable();
    ofDisableLighting();

    cam.end();
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer &output)
{
    std::size_t nChannels = output.getNumChannels();

    for (std::size_t i = 0; i < output.getNumFrames(); i++)
    {

        wave = stretches[current]->play(pitch, speed, grainLength, overlaps, 0.0);

        // play result
        mymix.stereo(wave, outputs, 0.5);
        output[i * nChannels] = outputs[0];
        output[i * nChannels + 1] = outputs[1];
    }
}
//--------------------------------------------------------------
void ofApp::audioIn(ofSoundBuffer &input)
{
}
