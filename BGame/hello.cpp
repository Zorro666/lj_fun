#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Slider.H>
#include <FL/filename.H>

extern "C" void LJ_guiinit( int argc, char** argv );
extern "C" void LJ_guitick( void );
extern "C" float g_width;
extern "C" float g_height;

Fl_Slider* width_slider;
Fl_Slider* height_slider;

void LJ_guiinit( int argc, char** argv )
{
	Fl_Window* const window = new Fl_Window(300,250);
	Fl_Box* const box = new Fl_Box( FL_UP_BOX, 20, 40, 260, 50, "Hello, World!" );
	box->labelfont( FL_BOLD + FL_ITALIC );
	box->labelsize( 36 );
	box->labeltype( FL_SHADOW_LABEL );
	width_slider = new Fl_Slider( FL_HOR_NICE_SLIDER, 20, 100, 260, 30, "Texture Width" );
	width_slider->range( 0.0f, 2.0f );
	width_slider->value( 1.0f );
	height_slider = new Fl_Slider( FL_HOR_SLIDER, 20, 150, 260, 30, "Texture Height" );
	height_slider->range( 0.0f, 2.0f );
	height_slider->value( 1.0f );
	window->end();
	window->show( argc, argv );
}

void LJ_guitick( void )
{
	g_width = width_slider->value();
	g_height = height_slider->value();
	Fl::check();
}

