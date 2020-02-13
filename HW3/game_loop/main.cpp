
#include "Game.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

using namespace std;

int main() {


	float frames_per_sec{ 60 }; //frame rate 60 frames per second
	float delta_time{ 1.0f / frames_per_sec }; // in seconds 60Hz

   std::cout << "Creating game" << std::endl;

   std::unique_ptr<Game> game;
   try { // try to iniitialize game obj
           game = std::make_unique<Game>("1st Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
   } catch (const std::runtime_error& e) { // if fails, return with error code
           std::cout << e.what() << std::endl;
	   return -1;
   }

   std::cout << "Starting game loop" << std::endl;
   while (game->running()) {

	   //start clock
	   auto stt = std::chrono::high_resolution_clock::now(); 

	   game->handle_events();
	   game->update();
	   game->render();

	   //end clock
	   auto stp = std::chrono::high_resolution_clock::now(); 

	   //compute duration in miliseconds
	   std::chrono::duration<double, std::milli> elapsed_t = stp - stt; 


	   // compute difference miliseconds between delta_time (60 frames per seconds) and elapsed time elapsed_t
	   //elapsed_t was converted to sec in order to subtract it from delta_time then the result was converted to ms
	   int time_gap = (delta_time - (elapsed_t.count() * std::chrono::milliseconds::period::num / std::chrono::milliseconds::period::den)) * 1000; 
	  
	  


	   //If elapsed time is shorter than 60Hz, go to sleep for the reminder
	   if ((elapsed_t.count() * std::chrono::milliseconds::period::num /
		   std::chrono::milliseconds::period::den) < delta_time)
	   {
		     
		   std::this_thread::sleep_for(std::chrono::milliseconds(time_gap));

	   }

   }

   return 0;
}

