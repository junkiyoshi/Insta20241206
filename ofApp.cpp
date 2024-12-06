#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->line.clear();

	float phi_deg_step = 8;
	float theta_deg_step = 20;

	float R = 230;
	float threshold_1 = 0.55;
	float threshold_2 = 0.65;
	float r = R * 0.3;

	int target_deg_start = ofGetFrameNum() * 1.44;

	for (int target_deg = target_deg_start; target_deg <= target_deg_start + 180; target_deg += 180) {

		auto target = glm::vec3(R * cos(target_deg * DEG_TO_RAD), R * sin(target_deg * DEG_TO_RAD), 0);

		for (float phi_deg = 0; phi_deg < 360; phi_deg += phi_deg_step) {

			for (float theta_deg = 0; theta_deg < 360; theta_deg += theta_deg_step) {

				auto location = this->make_point(R, r, theta_deg, phi_deg);
				auto distance = glm::distance(target, location);

				if (distance > 250) { continue; }

				auto power = distance < 150 ? 1 : ofMap(distance, 150, 250, 1, 0);

				auto index = this->face.getNumVertices();
				vector<glm::vec3> vertices;

				vertices.push_back(this->make_point(R, r, theta_deg - theta_deg_step * 0.49 * power - 2, phi_deg - phi_deg_step * 0.49 * power - 2));
				vertices.push_back(this->make_point(R, r, theta_deg + theta_deg_step * 0.49 * power - 2, phi_deg - phi_deg_step * 0.49 * power - 2));
				vertices.push_back(this->make_point(R, r, theta_deg - theta_deg_step * 0.49 * power - 2, phi_deg + phi_deg_step * 0.49 * power - 2));
				vertices.push_back(this->make_point(R, r, theta_deg + theta_deg_step * 0.49 * power - 2, phi_deg + phi_deg_step * 0.49 * power - 2));

				this->face.addVertices(vertices);
				this->line.addVertices(vertices);

				this->face.addIndex(index + 0); this->face.addIndex(index + 1); this->face.addIndex(index + 3);
				this->face.addIndex(index + 0); this->face.addIndex(index + 3); this->face.addIndex(index + 2);

				this->line.addIndex(index + 0); this->line.addIndex(index + 1);
				this->line.addIndex(index + 0); this->line.addIndex(index + 2);
				this->line.addIndex(index + 1); this->line.addIndex(index + 3);
				this->line.addIndex(index + 2); this->line.addIndex(index + 3);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(90);

	ofSetColor(255, 128, 255);
	this->line.draw();

	ofSetColor(0);
	this->face.draw();

	this->cam.end();

	/*
	int start = 500;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
glm::vec3 ofApp::make_point(float R, float r, float u, float v, float scale) {

	// 数学デッサン教室 描いて楽しむ数学たち　P.31

	u *= DEG_TO_RAD;
	v *= DEG_TO_RAD;

	auto x = (R + r * cos(u) * scale) * cos(v);
	auto y = (R + r * cos(u) * scale) * sin(v);
	auto z = r * sin(u) * scale;

	return glm::vec3(x, y, z);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}