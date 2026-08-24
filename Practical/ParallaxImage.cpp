#include <d3dx9.h>

class ParallaxImage {

public:
	LPDIRECT3DTEXTURE9 parallaxBG;
	float width, height, distanceFromFG;
	RECT rect;
	D3DXVECTOR3 position;

	ParallaxImage() {};

	ParallaxImage(LPDIRECT3DTEXTURE9 texture, RECT re, float width, float height) {
		position = D3DXVECTOR3(0, 0, 0);
		parallaxBG = texture;
		rect = re;
		this->width = width;
		this->height = height;
	}

	void Draw(LPD3DXSPRITE sprite, D3DXVECTOR3 cameraOffset, float cameraWidth, float cameraHeight)
	{
		cameraOffset = cameraOffset/ distanceFromFG + position;
			sprite->Draw(parallaxBG, &rect, NULL, &cameraOffset, D3DCOLOR_XRGB(255, 255, 255));
			
	}
};