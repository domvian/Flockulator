#pragma once
#include "ofMain.h"

class Boid{
    
public:
    
    Boid(ofVec3f pos, ofColor _color);
    Boid(const Boid& b);
    ~Boid();
    
    void draw();
    void update(vector<Boid>& _flock, float speedlimit, float separationFactor, float cohesionFactor, float alignmentFactor, float checkdist, float boundradius, float bouncespeed);
    ofVec3f getPosition();
    ofVec3f getVelocity();
    
private:
    
    void checkBoundary(float speed, float size);
    void flock(vector<Boid>& _flock, float separationFactor, float cohesionFactor, float alignmentFactor);
    
    ofVec3f velocity;
    float distToCheck;
    ofColor color;
    ofMaterial material;

    ofIcoSpherePrimitive sphere;
    ofBoxPrimitive box;
    ofConePrimitive beak;
    ofConePrimitive wing2;
    ofConePrimitive wing1;
    
};

