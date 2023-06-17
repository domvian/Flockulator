#pragma once

#include "ofMain.h"
#include "Boid.h"
#include "ofxGui.h"
#include "ofxMaxim.h"
#include "maxiGrains.h"
#include "ofxOsc.h"

#define PORT 12000

typedef hannWinFunctor grainPlayerWin;

class ofApp : public ofBaseApp
{

public:
    void setup();
    void update();
    void draw();

    void audioOut(ofSoundBuffer &output) override; // output method
    void audioIn(ofSoundBuffer &input) override;   // input method
    ofSoundStream soundStream;

    ofLight light;
    ofEasyCam cam;

    ofxOscReceiver osc;

    float oscSeparation, oscSpeed, oscCohesion, oscAlignment, oscDistance, oscRadius, oscEdgeBounciness, oscUp, oscDown, oscLeft, oscRight;

    vector<Boid> flock;
    vector<maxiTimePitchStretch<grainPlayerWin, maxiSample> *> stretches;
    double wave, waveOut, outputs[2];
    maxiSample samp;
    maxiMix mymix;

    int current;
    int amount;

    ofVec3f avgPos;
    float xPos, yPos;
    float speed, grainLength, pitch;
    int overlaps;
};
