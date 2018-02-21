#pragma once
class Thing
{
public:
	Thing();
	Thing(int texture, float posX,float posY, float rot, float w, float h);
	~Thing();

	void Draw();
	
	float x;
	float y;
	float rotation;
	int textureID;
	float width;
	float height;

	float speed;
	float direction_x;
	float direction_y;
};

