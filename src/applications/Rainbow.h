#include <system/MatrixManager.h>
#include <system/Application.h>

class Rainbow: public Application {
    public:
        Rainbow() {
            
        }
        void init(MatrixManager * mm, ControlManager * cm) override {
            mm->set_tps(30);
            cm->set_controls(all_buttons);
        }
        void game_loop(MatrixManager * mm, ControlManager * cm) override {
           if(fade_down) {
            this->currentBrightness--;
           }else{
              this->currentBrightness++;

           }
           if(this->currentBrightness > 254 || this->currentBrightness < 1) {
            fade_down=!fade_down;
           }


        }

        void draw(MatrixManager* mm, ControlManager *cm) {
        for(int x=0;x<12;x++) {
            mm->line(x,0,0,x,color_palette[x%4]);
            mm->line(11-x,11,11,11-x,color_palette[x%4]);
        }
        }
        
        void clean_up(MatrixManager * mm) override {
            
        }
        void on_event(Event e,MatrixManager * mm, ControlManager * cm) {
            
        }

        static Application* create() {
        return new Rainbow();
        }
      private:
        uint8_t currentBrightness = 0;
        std::vector<uint32_t> color_palette = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00};
        bool fade_down = false;
};