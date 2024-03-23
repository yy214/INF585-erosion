#include "colorInterp.hpp"

using namespace std;

cgp::vec3 colorInterpolation::getColor(float height)
{
	if (height > 1) {
		height = 1;
	}

	cgp::vec3 newColor;

	cgp::vec3 whiteVec = cgp::vec3(1.0, 1.0, 1.0);
	cgp::vec3 brownVec = cgp::vec3(0.8, 0.5, 0.3);
	cgp::vec3 greenVec = cgp::vec3(0.2, 0.5, 0.3);

	float cutoff = 0.2;
	float cutoffTwo = 0.70;


	if (height < cutoff) {
		float alpha = (cutoff - height) / cutoff;
		newColor = alpha * greenVec + (1.0 - alpha) * brownVec;
	}
	else {
		float beta = (cutoffTwo - height) / (cutoffTwo - cutoff);
		beta = std::pow(beta,0.3);
		newColor = beta * brownVec + (1.0 - beta) * whiteVec;


	
	}
	return newColor;
}
