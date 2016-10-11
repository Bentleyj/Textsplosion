#include "triangulator.h"

bool Triangulator::generateTriangulation(ofMesh * input, ofMesh * output)
{
	//output->clear();

	vector<ofPoint> inputVertices = input->getVertices();
	vector<unsigned int> inputIndices = input->getIndices();

	for (int i = 0; i < inputIndices.size(); i++) {
		output->addVertex(inputVertices[inputIndices[i]]);
	}

	return true;
}
