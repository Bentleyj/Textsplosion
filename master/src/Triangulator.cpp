#include "Triangulator.h"

bool Triangulator::generateTriangulation(ofMesh * input, ofMesh * output)
{
	//output->clear();

	vector<ofPoint> inputVertices = input->getVertices();
	vector<unsigned int> inputIndices = input->getIndices();
	vector<ofFloatColor> inputColors = input->getColors();

	for (int i = 0; i < inputIndices.size(); i++) {
		output->addVertex(inputVertices[inputIndices[i]]);
		output->addColor(inputColors[inputIndices[i]]);
	}

	return true;
}
