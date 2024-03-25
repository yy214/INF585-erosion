#include "colorInterp.hpp"

using namespace std;

cgp::vec3 colorInterpolation::getColor(float height)
{

	cgp::vec3 newColor;
	
	// White is for the snow
	cgp::vec3 whiteVec = cgp::vec3(1.0, 1.0, 1.0);
	
	// Brown is for the mountains
	cgp::vec3 brownVec = cgp::vec3(0.8, 0.5, 0.3);
	
	// Green is for the terrain close to the sea
	cgp::vec3 greenVec = cgp::vec3(0.2, 0.5, 0.3);

	// Blue is for the Sea
	cgp::vec3 blueVec = cgp::vec3(0.2, 0.4, 0.6);

	// Black is for the depths of the sea
	cgp::vec3 blackVec = cgp::vec3(0.0, 0.0, 0.0);

	float cutoff = 0.f;
	float cutoffOne = 0.3f;
	float cutoffTwo = 0.7f;


	if (height < cutoff) {
		// Coloring the Sea
		float alpha = std::min(-height, 1.f);
		newColor = alpha * blackVec + (1-alpha)*blueVec;
	}
	else if (height < cutoffOne) {
		// Coloring the low-ground terrain
		float alpha = (cutoffOne- height) / (cutoffOne - cutoff);
		newColor = alpha * greenVec + (1.0 - alpha) * brownVec;
	
	}
	else if(height < cutoffTwo){
		// Coloring the Mountain and Peaks
		float beta = (cutoffTwo - height) / (cutoffTwo - cutoff);
		beta = std::pow(beta,0.3);
		newColor = beta * brownVec + (1.0 - beta) * whiteVec;
	}
	else {
		newColor = whiteVec;
	}
	return newColor;
}
