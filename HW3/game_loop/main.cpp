
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
   auto game = std::make_unique<Game>("1st Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

   std::cout << "Starting game loop" << std::endl;
   while (game->running()) {

	   auto stt = std::chrono::high_resolution_clock::now(); //start clock

	   game->handle_events();
	   game->update();
	   game->render();

	   auto stp = std::chrono::high_resolution_clock::now(); //end clock
	   std::chrono::duration<double, std::milli> elapsed_t = stp - stt; //compute duration in miliseconds

	   int time_gap = (delta_time - (elapsed_t.count() * std::chrono::milliseconds::period::num /
		   std::chrono::milliseconds::period::den)) * 1000; // compute difference miliseconds between 60Hz and elapsed time

	   /*
	   std::cout << "time gap is" << time_gap << std::endl;
	   std::cout << "delta time" << delta_time << std::endl;
	   std::cout << "elapsed time " << (elapsed_t.count() * std::chrono::milliseconds::period::num /
	   std::chrono::milliseconds::period::den) << std::endl;//display elapsed time in seconds
	   */


	   //If elapsed time is shorter than 60Hz, go to sleep for the reminder
	   if ((elapsed_t.count() * std::chrono::milliseconds::period::num /
		   std::chrono::milliseconds::period::den) < delta_time)
	   {
		  
		  // std::cout << "sleeping..." << std::endl;
		   std::this_thread::sleep_for(std::chrono::milliseconds(time_gap));

	   }

   }

   return 0;
}

