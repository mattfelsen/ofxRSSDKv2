#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetWindowShape(640, 480);
	mRSSDK = RSDevice::createUniquePtr();	
	
	if(!mRSSDK->init())
	{
		ofLogError("Unable to create ofxRSSDK object");
		exit();
	}
	else
	{
		mRSSDK->initPersonTracking();
		mRSSDK->initDepth(DepthRes::R200_SD, 30, true);
		mRSSDK->initRgb(RGBRes::VGA, 30);
		mTexRgb.allocate(mRSSDK->getRgbWidth(), mRSSDK->getRgbHeight(),GL_RGBA);
		mRSSDK->start();
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	mRSSDK->update();
	mTexRgb.loadData(mRSSDK->getRgbFrame(), GL_BGRA);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofClear(ofColor::black);
	mTexRgb.draw(0, 0);
	
	auto expr = mRSSDK->getTrackedPersonExpressions();
	if (lastNeutral.size() != expr.size()) lastNeutral.resize(expr.size());
	if (lastHappines.size() != expr.size()) lastHappines.resize(expr.size());
	
	for (int i = 0; i < expr.size(); i++) {
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(200 * i, 400);
		ofSetColor(ofColor::red);
		
		int j = -1;
		int x = 0;
		int spacing = 85;
		int width = 15;
		int height = 375;

		ofDrawLine(0, 0, 400, 0);
		ofDrawLine(0, -height, 400, -height);

		x = ++j * spacing;
		lastNeutral[i] = ofLerp(lastNeutral[i], expr[i].neutral, 0.2);
		ofDrawBitmapString("neutral", x, 0);
		ofDrawBitmapString("raw : " + ofToString(expr[i].neutral), x, 20);
		ofDrawBitmapString("filt: " + ofToString(lastNeutral[i]), x, 40);
		ofDrawRectangle(x, 0, width, -ofMap(lastNeutral[i], 0, 100, 0, height, true));

		x = ++j * spacing;
		lastHappines[i] = ofLerp(lastHappines[i], expr[i].happiness, 0.2);
		ofDrawBitmapString("happiness", x, 0);
		ofDrawBitmapString("raw : " + ofToString(expr[i].happiness), x, 20);
		ofDrawBitmapString("filt: " + ofToString(lastHappines[i]), x, 40);
		ofDrawRectangle(x, 0, width, -ofMap(lastHappines[i], 0, 100, 0, height, true));

		ofPopMatrix();
		ofPopStyle();
	}

	ofDrawBitmapString("Num people: " + ofToString(expr.size()), ofPoint(10, 20));
}


void ofApp::exit()
{
	mRSSDK->stop();
}
