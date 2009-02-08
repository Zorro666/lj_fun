#include "LJ.h"

#include "LJ_fltk.h"

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Window.H>

Fl_Slider* width_slider;
Fl_Slider* height_slider;

void LJ_fltkInit( LJ_int argc, LJ_char** argv )
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

void LJ_fltkReset( void )
{
}

void LJ_fltkShutdown( void )
{
}

void LJ_fltkTick( void )
{
	Fl::check();
}

// returns which button was pressed
LJ_int LJ_fltkMessageBox( const LJ_int numButtons, const LJ_char* const buttonTexts[], 
						  const LJ_char* const title, const LJ_char* const message )
{
	const LJ_int boxWidth = 600;
	const LJ_int boxHeight = 300;

	const LJ_int buttonWidth = 80;
	const LJ_int buttonHeight = 40;

	const LJ_int textX0 = 10;
	const LJ_int textY0 = 10;
	const LJ_int textWidth = boxWidth - textX0 * 2;
	const LJ_int textHeight = boxHeight - buttonHeight - buttonHeight - textY0 * 2;

	const LJ_int buttonSpacing = ( boxWidth - numButtons * buttonWidth ) / ( numButtons + 1 );

	const LJ_int buttonX0 = buttonSpacing;
	const LJ_int buttonY0 = boxHeight - buttonHeight - ( buttonHeight / 2 );

	Fl_Window window( 300, 300, boxWidth, boxHeight, title );

	Fl_Multiline_Output text( textX0, textY0, textWidth, textHeight );
	text.value( message );

	LJ_int x = buttonX0;
	LJ_int y = buttonY0;

	Fl_Button** const buttons = LJ_MEM_ALLOC( LJ_memGetSystemAllocHandle(), Fl_Button*, numButtons );
	for ( LJ_int i = 0; i < numButtons; i++ )
	{
		buttons[i] = new Fl_Button( x, y, buttonWidth, buttonHeight, buttonTexts[i] );
		x += buttonWidth + buttonSpacing;
	}

	window.end();
	window.set_modal();
	window.show();

	LJ_int whichButton = -2;

	while ( whichButton == -2 )
	{
		Fl::check();

		// Handle if the window is closed without pressing a button
		if ( Fl::modal() == LJ_NULL )
		{
			whichButton = -1;
			break;
		}

		for ( LJ_int i = 0; i < numButtons; i++ )
		{
			if ( buttons[i]->value() == 1 )
			{
				whichButton = i;
				break;
			}
		}
	}

	LJ_MEM_FREE( buttons );

	return whichButton;
}

