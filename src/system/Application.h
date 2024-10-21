#pragma once
#include "MatrixManager.h"
#include "ControlManager.h"
#include "Event.h"
/**
 * Represents an application that can be run on the matrix. Your are supposed
 * to create an additional static method to instantiate your application.
 * The reference to the create method is used in the SystemManager to register
 * your application with its name and author.
 */
class Application {
    public:
        Application() {
            
        }

        virtual ~Application() {};

        /**
         * Initialize your application. This method is called once when the application is started.
         * You can use the MatrixManager and ControlManager to initialize your application. (setting controls, tps, etc.)
         * @param mm MatrixManager
         * @param cm ControlManager
         */

        virtual void init(MatrixManager * mm, ControlManager * cm) = 0;

        /**
         * Draw your application. This method is called every frame. The frequency of this method can vary
         * and is independent of the current tps. You shouldn't modify the state of your application within the draw
         * call.
         * @param mm MatrixManager
         * @param cm ControlManager
         */
        virtual void draw(MatrixManager * mm, ControlManager *cm) = 0;

        /**
         * Update the logic of your application at a fixed frequency. Avoid drawing to the matrix
         * as the updated pixels won't be applied until the next draw cycle.
         * @param mm MatrixManager
         * @param cm ControlManager
         */
        virtual void game_loop(MatrixManager * mm, ControlManager * cm) = 0;

        /**
         * Called when the application is stopped. You can use this method to free resources.
         * @param mm MatrixManager
         * @param cm ControlManager
         */
        virtual void clean_up(MatrixManager * mm) = 0;

        /**
         * This method is called when an event is triggered. You can use the MatrixManager and ControlManager
         * to handle the event. Events are triggered by the user (controls, ...) or the system.
         * @param e Event
         * @param mm MatrixManager
         * @param cm ControlManager
         */
        virtual void on_event(Event e,MatrixManager * mm, ControlManager * cm) = 0;

      //  /**
       //  * Create a new instance of your application
       //  * @return pointer to your application
      //   */
      //  static Application *create()
      //  {
      //      return new Application();
      //  }
    
    private:

};