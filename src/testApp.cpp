//
// testApp.cpp - RAMDanceToolkit
//
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testApp.h"

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
    
	/// ram setup
	// ------------------
	ramInitialize(10000);
    
    sender.setup( "192.168.12.107", 12345);
    //fbo.allocate( ofGetWidth(), ofGetHeight(), GL_RGBA, 4);
}

//--------------------------------------------------------------
void testApp::update()
{
	
}

//--------------------------------------------------------------
void testApp::draw()
{
}



void testApp::drawPart(const ramActor &actor, PART_NAME part){
    ramNode n1;
    ramNode n2;
    ramNode n3;
    ramNode n4;
    
    switch (part) {
        case LEFT_HAND:
            n1 = actor.getNode( ramActor::JOINT_LEFT_SHOULDER );
            n2 = actor.getNode( ramActor::JOINT_LEFT_ELBOW );
            n3 = actor.getNode( ramActor::JOINT_LEFT_WRIST );
            n4 = actor.getNode( ramActor::JOINT_LEFT_HAND );
            break;
        case LEFT_FOOT:
            n1 = actor.getNode( ramActor::JOINT_LEFT_HIP );
            n2 = actor.getNode( ramActor::JOINT_LEFT_KNEE );
            n3 = actor.getNode( ramActor::JOINT_LEFT_ANKLE );
            n4 = actor.getNode( ramActor::JOINT_LEFT_TOE );
            break;
        case RIGHT_HAND:
            n1 = actor.getNode( ramActor::JOINT_RIGHT_SHOULDER );
            n2 = actor.getNode( ramActor::JOINT_RIGHT_ELBOW );
            n3 = actor.getNode( ramActor::JOINT_RIGHT_WRIST );
            n4 = actor.getNode( ramActor::JOINT_RIGHT_HAND );
            break;
        case RIGHT_FOOT:
            n1 = actor.getNode( ramActor::JOINT_RIGHT_HIP );
            n2 = actor.getNode( ramActor::JOINT_RIGHT_KNEE );
            n3 = actor.getNode( ramActor::JOINT_RIGHT_ANKLE );
            n4 = actor.getNode( ramActor::JOINT_RIGHT_TOE );
            break;
        default:
            n1 = actor.getNode( ramActor::JOINT_LEFT_SHOULDER );
            n2 = actor.getNode( ramActor::JOINT_LEFT_ELBOW );
            n3 = actor.getNode( ramActor::JOINT_LEFT_WRIST );
            n4 = actor.getNode( ramActor::JOINT_LEFT_HAND );
            break;
    }
    
    ofScale( 1, -1, 1);
    ofTranslate(-n1.getGlobalPosition());
    ramLine( n2 );
    
    ofNoFill();
    ofVec3f pos1 = n1.getGlobalPosition();
    ofVec3f pos2 = n2.getGlobalPosition();
    ofVec3f pos3 = n3.getGlobalPosition();
    ofVec3f pos4 = n4.getGlobalPosition();
    
    ofCurve(pos1.x,pos1.y,pos1.z,pos2.x,pos2.y,pos2.z,pos3.x,pos3.y,pos3.z,pos4.x,pos4.y,pos4.z);
}

void testApp::sendOSC(const ramActor &actor){
    static ofVec3f oldQ;
    static ofVec3f Q;
    Q = actor.getNode( ramActor::JOINT_HIPS ).getAcceleration() * 0.002 + oldQ * 0.998;
    oldQ = Q;
    
    ramNode n = actor.getNode( ramActor::JOINT_LEFT_HAND );
    ramNode n2 = actor.getNode( ramActor::JOINT_RIGHT_KNEE );
    ramNode n3 = actor.getNode( ramActor::JOINT_LEFT_TOE );
    ramNode n4 = actor.getNode( ramActor::JOINT_RIGHT_HAND );
    ramNode n5 = actor.getNode( ramActor::JOINT_RIGHT_TOE );
    ramNode n6 = actor.getNode( ramActor::JOINT_HEAD );
    
    static ofVec3f oldLH;
    static ofVec3f LH;
    LH = n.getVelocity() * 0.002 + oldLH * 0.998;
    oldLH = LH;
    
    static ofVec3f oldRH;
    static ofVec3f RH;
    RH = n4.getVelocity() * 0.002 + oldRH * 0.998;
    oldRH = RH;
    
    static ofVec3f oldLT;
    static ofVec3f LT;
    LT = n3.getVelocity() * 0.002 + oldLT * 0.998;
    oldLT = LT;
    
    static ofVec3f oldRT;
    static ofVec3f RT;
    RT = n5.getVelocity() * 0.002 + oldRT * 0.998;
    oldRT = RT;
    
    static ofVec3f oldH;
    static ofVec3f H;
    H = n6.getVelocity() * 0.002 + oldH * 0.998;
    oldH = H;
    
    ofxOscMessage m;
//    m.setAddress("/HIPS");
//    m.addFloatArg( Q.x );
//    m.addFloatArg( Q.y );
//    m.addFloatArg( Q.z );
//    sender.sendMessage(m);
    m.clear();
    m.setAddress("/m");
    m.addFloatArg( H.length() );
    m.addFloatArg( LH.length() );
    m.addFloatArg( LT.length() );
    m.addFloatArg( RH.length() );
    m.addFloatArg( RT.length() );
    sender.sendMessage(m);
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
    if( actor.getName() == "Motioner1" ){
        sendOSC(actor);
        
        ofSetColor(100);
        for(int i = 0; i<600; i++){
            float x = ofNoise(i*0.77, ofGetElapsedTimef()*0.001)-0.5;
            float z = ofNoise(i*i*0.001, ofGetElapsedTimef()*0.001)-0.5;
            x *= 300 * 2;
            z *= 300 * 2;
            float angle = atan2( z, x );
            
            x += cos( angle ) * 200;
            z += sin( angle ) * 200;
            
            ofPushMatrix();
            ofTranslate(x, 0, z);
            
            ramNode hip = actor.getNode( ramActor::JOINT_HIPS );
            float bias = ofNoise(hip.getGlobalPosition().x*0.001, hip.getGlobalPosition().y*0.001) * 360;
            ofRotateY( x/50*10+z/50+bias );
            if(x > 0){
                if(z>0){
                    drawPart(actor, LEFT_HAND);
                } else {
                    drawPart(actor, RIGHT_HAND);
                }
            } else {
                if(z>0){
                    drawPart(actor, LEFT_FOOT);
                } else {
                    drawPart(actor, RIGHT_FOOT);
                }
            }
            ofPopMatrix();
        }
        
        ofSetColor(220,64,128);
        ofSetLineWidth(5);
        
        ofPushMatrix();
        
        drawPart(actor, LEFT_HAND);
        ramNode n = actor.getNode( ramActor::JOINT_LEFT_HAND );
        ramNode n2 = actor.getNode( ramActor::JOINT_RIGHT_KNEE );
    
        for(int i = 0; i<5; i++){
            ofTranslate( n.getGlobalPosition().x, n.getGlobalPosition().y, n.getGlobalPosition().z );
            ofQuaternion curRot = n2.getGlobalOrientation();
            ofVec3f axis;
            float angle;
            curRot.getRotate(angle, axis);
            ofRotate(angle, axis.x, axis.y, axis.z);
            drawPart(actor, LEFT_HAND);
        }
        
        ofPopMatrix();
    }
}

//--------------------------------------------------------------
void testApp::drawRigid(const ramRigidBody &rigid)
{
	
}


#pragma mark - ram Events
//--------------------------------------------------------------
void testApp::onActorSetup(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::onActorExit(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::onRigidSetup(const ramRigidBody &rigid)
{
	
}

//--------------------------------------------------------------
void testApp::onRigidExit(const ramRigidBody &rigid)
{
	
}


#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    static bool hide = true;
    if(key == 'h' || key == 'H' ){
        if(hide){
            ofHideCursor();
            hide = false;
        } else {
            ofShowCursor();
            hide = true;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
}
