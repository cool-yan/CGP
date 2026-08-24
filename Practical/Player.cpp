#include <d3dx9.h>

class MovableCharacter {
public:
	LPDIRECT3DTEXTURE9 texture;
	float width, height, distanceFromFG, mass, groundLevel;
	RECT rect;
	D3DXVECTOR3 position, velocity;

	MovableCharacter() {};
	MovableCharacter(LPDIRECT3DTEXTURE9 texture, RECT re, 
		float width, float height, float groundLevel, float mass) {
		position = D3DXVECTOR3(200, 200, 0);
		this->texture = texture;
		rect = re;
		this->width = width;
		this->height = height;
		this->groundLevel = groundLevel;
		this->mass = mass;
	};

	void Jump(D3DXVECTOR3 force) {
		velocity += force / mass;
	}
	void Update() {
		if (position.y <= groundLevel) {
			velocity.y += 9.8;
			position += velocity;
		}
	}
	void Draw(LPD3DXSPRITE sprite, D3DXVECTOR3 cameraOffset) {
		cameraOffset += position;
		sprite->Draw(texture, &rect, NULL, &cameraOffset, D3DCOLOR_XRGB(255, 255, 255));
	}
};