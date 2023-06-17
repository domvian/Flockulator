#include "Boid.h"

// Constructor
Boid::Boid(ofVec3f pos, ofColor _color)
{

    velocity = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
    color = _color;
    sphere.set(1, 0.5);
    sphere.setPosition(pos);
    material.setDiffuseColor(_color);
}
Boid::Boid(const Boid &b)
{

    velocity = b.velocity;
    color = b.color;
    sphere = b.sphere;
    material = b.material;
}

Boid::~Boid()
{
}

//--------------------------------------------------------------
void Boid::update(vector<Boid> &_flock, float speedlimit, float separationFactor, float cohesionFactor, float alignmentFactor, float checkdist, float boundradius, float bouncespeed)
{

    distToCheck = checkdist;
    checkBoundary(bouncespeed, boundradius);

    // Add flocking behavior
    flock(_flock, separationFactor, cohesionFactor, alignmentFactor);

    // Limit how fast a boid can go
    velocity = velocity.limit(speedlimit);

    // Move the Boid
    sphere.move(velocity);
}

void Boid::draw()
{

    material.begin();
    sphere.draw();
    material.end();
}

void Boid::flock(vector<Boid> &_flock, float separationFactor, float cohesionFactor, float alignmentFactor)
{
    ofVec3f separation = ofVec3f(0, 0, 0);
    ofVec3f cohesion = ofVec3f(0, 0, 0);
    ofVec3f align = ofVec3f(0, 0, 0);
    int numCohesionNeighbours = 0;
    int numAlignNeighbours = 0;

    // query the entire flock
    for (int i = 0; i < _flock.size(); i++)
    {

        float dist = getPosition().distance(_flock[i].getPosition());

        if (dist < distToCheck)
        {
            // it's close enough >> add a difference vector
            separation -= (_flock[i].getPosition() - getPosition());
        }

        if (dist < distToCheck + 20 && dist > 10)
        {
            // its close enough >> add its position to the avarage
            cohesion += _flock[i].getPosition();
            numCohesionNeighbours++;
        }

        if (dist < distToCheck + 10 && dist > 10)
        {
            // it's close enough >> add its velocity to the avarage
            align += _flock[i].getVelocity();
            numAlignNeighbours++;
        }
    }

    separation *= separationFactor;
    velocity += separation;

    if (numCohesionNeighbours == 0)
    {
        cohesion = ofVec3f(0, 0, 0);
    }
    else
    {
        cohesion /= numCohesionNeighbours;
        cohesion -= getPosition();
        cohesion *= cohesionFactor;
    }

    velocity += cohesion;

    if (numAlignNeighbours == 0)
    {
        align = ofVec3f(0, 0, 0);
    }
    else
    {
        align /= numAlignNeighbours;
        align *= alignmentFactor;
    }

    velocity += align;
}

void Boid::checkBoundary(float bouncespeed, float size)
{
    // check if the boid is within the boundaries and bounce it back
    ofVec3f position = getPosition();
    ofVec3f desired = ofVec3f(0, 0, 0) - position;

    if (desired.length() > size)
    {
        desired = desired.normalize() * bouncespeed;
        velocity = desired - velocity;
    }
}

ofVec3f Boid::getPosition()
{
    return sphere.getPosition();
}

ofVec3f Boid::getVelocity()
{
    return velocity;
}
