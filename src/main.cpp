/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: tg
 *
 * Created on 27. Oktober 2016, 10:56
 */

#include <cstdlib>
#include <memory>
#include <chrono>
#include "simulation.h"
#include "display2D.h"
#include "simpleRender.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    float frameTime; //current frame time
    bool enable_tectonic = true; //bool for enabling/disabling tectonic
    bool renderAgeMap = false; //bool for enabling/disabling age map rendering
    int windowWidth = 800, windowHeight= 600; //start values for window dimensions
    int textureWidth = 800, textureHeight= 600; 
    srand(time(NULL)); //initialize random number generator

    //generate a new lithosphere.

    //initalize rendering
    std::unique_ptr<simpleRender> render = std::make_unique<simpleRender>(windowWidth,windowHeight,"RTplatec");
    
    if(render->init() != 0) //if something faild
    {
        std::cout << "Failed to init window!";
        return 0; //close program
    }
        //initalize input handler
    std::unique_ptr<inputHandler> input = std::make_unique<inputHandler>(render->getWindow());
    
    std::unique_ptr<simulation> sim = std::make_unique<simulation>(textureWidth,textureHeight);
    sim->init();
    sim->initLithosphere(0.56,0.01,8000000, 3.00,10,0.7);

    std::unique_ptr<display2D> disp = std::make_unique<display2D>("display/2D/2d_display");
    

    //initialize time counter
    auto timeBeforeLoop =  std::chrono::high_resolution_clock::now();        
    
    /** Init OpenGL stuff*/

     while( !glfwGetKey(render->getWindow(),GLFW_KEY_ESCAPE)) //as long as "ESCAPE" isn't pressed
    {
         
        timeBeforeLoop =  std::chrono::high_resolution_clock::now(); //get timestamp
        //get current window size
        glfwGetWindowSize(render->getWindow(),&windowWidth, &windowHeight);
        
        render->clearWindow(); //clear window

         sim->update();
         disp->render({windowWidth,windowHeight},sim->getCurrentDisplay(),sim->getTextureID(sim->getCurrentDisplay()));
                  input->update(); //handle input

         
         
        render->render(); //swap buffers and render

        //calculate frame time
        frameTime = 1.f/(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timeBeforeLoop).count()/1000.f);        // the difference
  
    }
    

    
    input->exit(); //close input stuff
    sim->exit();
    disp->exit();
    render->exit(); //close opengl Context
    

    //quit
    return 0;
}

