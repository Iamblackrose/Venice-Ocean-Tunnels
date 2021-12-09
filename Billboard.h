#pragma once
#include "Texture.h"

namespace GE {
	class Billboard {
	public:
		//constructor
		Billboard(Texture* t) {
			billTex = t;

			x = y = z = 0.0f;

			scaleX = scaleY = scaleZ = 1.0f;
		}

		~Billboard(){}

		float getX() {
			return x;
		}

		float getY() {
			return y;
		}

		float getZ() {
			return z;
		}

		void setX(float newX) {
			x = newX;
		}

		void setY(float newY) {
			y = newY;
		}

		void setZ(float newZ) {
			z = newZ;
		}

		Texture* getTexture() {
			return billTex;
		}

		void setScaleX(float newX) {
			scaleX = newX;
		}

		void setScaleY(float newY) {
			scaleY = newY;
		}

		float getScaleX() {
			return scaleX;
		}

		float getScaleY() {
			return scaleY;
		}

	private:

		float x, y, z;

		float scaleX, scaleY, scaleZ;

		Texture* billTex;
	};
}