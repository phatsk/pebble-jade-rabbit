#include <pebble.h>

static Window          *MainWindow;
static TextLayer       *TimeLayer;

// Background pointers
static BitmapLayer     *BackgroundLayer;
static GBitmap         *BackgroundBitmap;

// Function to update the time
static void update_time() {
	// Time structure
	time_t temp = time(NULL);
	struct tm *tick_time = localtime(&temp);

	// Make a buffer
	static char buffer[] = "00:00";

	// Write the crrent time to the buffer
	if ( clock_is_24h_style() == true ) {
		strftime( buffer, sizeof( "00:00" ), "%H:%M", tick_time );
	} else {
		strftime( buffer, sizeof( "00:00" ), "%I:%M", tick_time );
	}

	// Display it on the TimeLayer
	text_layer_set_text( TimeLayer, buffer );
}

static void tick( struct tm *tick_time, TimeUnits units_change ) {
	update_time();
}

static void main_window_load( Window *window ) {
	// Create the bitmap layer
	BackgroundBitmap = gbitmap_create_with_resource( RESOURCE_ID_IMAGE_JADE_RABBIT_NO_TEXT_LIGHT );
	BackgroundLayer  = bitmap_layer_create( GRect( 0, 0, 144, 168 ) );

	bitmap_layer_set_bitmap( BackgroundLayer, BackgroundBitmap );
	layer_add_child( window_get_root_layer( window ), bitmap_layer_get_layer( BackgroundLayer ) );

	// Create our text layer
	TimeLayer = text_layer_create( GRect( 0, 114, 144, 50 ) );
	text_layer_set_background_color ( TimeLayer, GColorClear );
	text_layer_set_text_color       ( TimeLayer, GColorBlack );

	// Font and alignment
	text_layer_set_font             ( TimeLayer, fonts_get_system_font( FONT_KEY_BITHAM_42_BOLD ) ); // @TODO try new fonts
	text_layer_set_text_alignment   ( TimeLayer, GTextAlignmentCenter );

	// Finally, add it to the window
	layer_add_child( window_get_root_layer( window ), text_layer_get_layer( TimeLayer ) );
}

static void main_window_unload( Window *window ) {
	text_layer_destroy( TimeLayer );
	gbitmap_destroy( BackgroundBitmap );
	bitmap_layer_destroy( BackgroundLayer );
}

static void init() {
	MainWindow = window_create(); // Make a new window, assign to the pointer

	// Subscribe to the tick handler
	tick_timer_service_subscribe( MINUTE_UNIT, tick );

	// Assign handlers
	window_set_window_handlers( MainWindow, (WindowHandlers) {
		.load   = main_window_load,
		.unload = main_window_unload
	});

	// Show the watchface
	window_stack_push( MainWindow, true );
	update_time();
}

static void deinit() {
	window_destroy( MainWindow );
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
