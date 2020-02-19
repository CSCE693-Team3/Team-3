
#ifndef __GameObject_HPP__
#define __GameObject_HPP__

#include "SDL2/SDL.h"



//Abstract Class
class GameObject {
public:
   GameObject(const char*, const float, const float, const float, const float);
   ~GameObject();


   void update(const float dt);

   //pure virtual method required for abstract class
   virtual void render()= 0;

private:
   float xpos{}, ypos{};
   float xvel{}, yvel{};
protected:
   SDL_Texture* texture{};
   SDL_Rect src_rect, dest_rect;
};
#endif

//class definition for Tank- like a header file
class Tank : public GameObject {
public:
	Tank(const char* Tank_img, const float xpos, const float ypos, const float xvel, const float yvel);
	~Tank();


	void render();

};

//class definition for Pacman- like a header file
class Pacman : public GameObject {
public:
	Pacman(const char* Tank_img, const float xpos, const float ypos,
		const float xvel, const float yvel);
	~Pacman();


	void render();

};

//class definition for Chopper- like a header file
class Chopper : public GameObject
{
public:
	Chopper(const char* Tank_img, const float xpos, const float ypos,
		const float xvel, const float yvel);
	~Chopper();


	void render();
private:

};
