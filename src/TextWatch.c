#include <pebble.h>

#include "num2words.h"

#define DEBUG 0

#define NUM_LINES 4
#define LINE_LENGTH 7
#define BUFFER_SIZE (LINE_LENGTH + 2)
#define ROW_HEIGHT 37
#define TOP_MARGIN 10

#define TOOLBAR_HEIGHT 16
#define TOOLBAR_BT_WIDTH 16
#define TOOLBAR_BATT_WIDTH 36
#define BUFFER_SIZE_DATE 20
#define BUFFER_SIZE_BATT 6
#define BUFFER_SIZE_BT 10

#define INVERT_KEY 0
#define TEXT_ALIGN_KEY 1
#define LANGUAGE_KEY 2
#define DISPLAY_TOOLBAR_KEY 3

#define TEXT_ALIGN_CENTER 0
#define TEXT_ALIGN_LEFT 1
#define TEXT_ALIGN_RIGHT 2

// The time it takes for a layer to slide in or out.
#define ANIMATION_DURATION 400
// Delay between the layers animations, from top to bottom
#define ANIMATION_STAGGER_TIME 150
// Delay from the start of the current layer going out until the next layer slides in
#define ANIMATION_OUT_IN_DELAY 100

#define LINE_APPEND_MARGIN 0
// We can add a new word to a line if there are at least this many characters free after
#define LINE_APPEND_LIMIT (LINE_LENGTH - LINE_APPEND_MARGIN)

static AppSync sync;
static uint8_t sync_buffer[64];

static int text_align = TEXT_ALIGN_CENTER;
static bool invert = false;
static Language lang = EN_US;
static bool displayToolbar = true;

static Window *window;

typedef struct {
	TextLayer *currentLayer;
	TextLayer *nextLayer;
	char lineStr1[BUFFER_SIZE];
	char lineStr2[BUFFER_SIZE];
	PropertyAnimation *animation1;
	PropertyAnimation *animation2;
} Line;

static Line lines[NUM_LINES];
static InverterLayer *inverter_layer;

typedef struct {
	TextLayer *dateLayer;
    GBitmap *bt_on;
    GBitmap *bt_off;
	BitmapLayer *blutoothLayer;
	TextLayer *batteryLayer;
} Toolbar;

static Toolbar toolbar;
static struct tm *t;

static int currentNLines;

// Animation handler
static void animationStoppedHandler(struct Animation *animation, bool finished, void *context)
{
	TextLayer *current = (TextLayer *)context;
	GRect rect = layer_get_frame((Layer *)current);
	rect.origin.x = 144;
	layer_set_frame((Layer *)current, rect);
}

// Animate line
static void makeAnimationsForLayer(Line *line, int delay)
{
	TextLayer *current = line->currentLayer;
	TextLayer *next = line->nextLayer;

	// Destroy old animations
	if (line->animation1 != NULL)
	{
		 property_animation_destroy(line->animation1);
	}
	if (line->animation2 != NULL)
	{
		 property_animation_destroy(line->animation2);
	}

	// Configure animation for current layer to move out
	GRect rect = layer_get_frame((Layer *)current);
	rect.origin.x =  -144;
	line->animation1 = property_animation_create_layer_frame((Layer *)current, NULL, &rect);
	animation_set_duration(&line->animation1->animation, ANIMATION_DURATION);
	animation_set_delay(&line->animation1->animation, delay);
	animation_set_curve(&line->animation1->animation, AnimationCurveEaseIn); // Accelerate

	// Configure animation for current layer to move in
	GRect rect2 = layer_get_frame((Layer *)next);
	rect2.origin.x = 0;
	line->animation2 = property_animation_create_layer_frame((Layer *)next, NULL, &rect2);
	animation_set_duration(&line->animation2->animation, ANIMATION_DURATION);
	animation_set_delay(&line->animation2->animation, delay + ANIMATION_OUT_IN_DELAY);
	animation_set_curve(&line->animation2->animation, AnimationCurveEaseOut); // Deaccelerate

	// Set a handler to rearrange layers after animation is finished
	animation_set_handlers(&line->animation2->animation, (AnimationHandlers) {
		.stopped = (AnimationStoppedHandler)animationStoppedHandler
	}, current);

	// Start the animations
	animation_schedule(&line->animation1->animation);
	animation_schedule(&line->animation2->animation);	
}

static void updateLayerText(TextLayer* layer, char* text)
{
	const char* layerText = text_layer_get_text(layer);
	strcpy((char*)layerText, text);
	// To mark layer dirty
	text_layer_set_text(layer, layerText);
    //layer_mark_dirty(&layer->layer);
}

// Update line
static void updateLineTo(Line *line, char *value, int delay)
{
	updateLayerText(line->nextLayer, value);
	makeAnimationsForLayer(line, delay);

	// Swap current/next layers
	TextLayer *tmp = line->nextLayer;
	line->nextLayer = line->currentLayer;
	line->currentLayer = tmp;
}

// Check to see if the current line needs to be updated
static bool needToUpdateLine(Line *line, char *nextValue)
{
	const char *currentStr = text_layer_get_text(line->currentLayer);

	if (strcmp(currentStr, nextValue) != 0) {
		return true;
	}
	return false;
}

static GTextAlignment lookup_text_alignment(int align_key)
{
	GTextAlignment alignment;
	switch (align_key)
	{
		case TEXT_ALIGN_LEFT:
			alignment = GTextAlignmentLeft;
			break;
		case TEXT_ALIGN_RIGHT:
			alignment = GTextAlignmentRight;
			break;
		default:
			alignment = GTextAlignmentCenter;
			break;
	}
	return alignment;
}

// Configure bold line of text
static void configureBoldLayer(TextLayer *textlayer)
{
	text_layer_set_font(textlayer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_text_color(textlayer, GColorWhite);
	text_layer_set_background_color(textlayer, GColorClear);
	text_layer_set_text_alignment(textlayer, lookup_text_alignment(text_align));
}

// Configure light line of text
static void configureLightLayer(TextLayer *textlayer)
{
	text_layer_set_font(textlayer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
	text_layer_set_text_color(textlayer, GColorWhite);
	text_layer_set_background_color(textlayer, GColorClear);
	text_layer_set_text_alignment(textlayer, lookup_text_alignment(text_align));
}

// Configure the layers for the given text
static int configureLayersForText(char text[NUM_LINES][BUFFER_SIZE], char format[])
{
	int numLines = 0;

	// Set bold layer.
	int i;
	for (i = 0; i < NUM_LINES; i++) {
		if (strlen(text[i]) > 0) {
			if (format[i] == 'b')
			{
				configureBoldLayer(lines[i].nextLayer);
			}
			else
			{
				configureLightLayer(lines[i].nextLayer);
			}
		}
		else
		{
			break;
		}
	}
	numLines = i;

	// Calculate y position of top Line
	int ypos = (168 - numLines * ROW_HEIGHT) / 2 - TOP_MARGIN;

	// Set y positions for the lines
	for (int i = 0; i < numLines; i++)
	{
		layer_set_frame((Layer *)lines[i].nextLayer, GRect(144, ypos, 144, 50));
		ypos += ROW_HEIGHT;
	}

	return numLines;
}

static void time_to_lines(int hours, int minutes, int seconds, char lines[NUM_LINES][BUFFER_SIZE], char format[])
{
	int length = NUM_LINES * BUFFER_SIZE + 1;
	char timeStr[length];
	time_to_words(lang, hours, minutes, seconds, timeStr, length);
	
	// Empty all lines
	for (int i = 0; i < NUM_LINES; i++)
	{
		lines[i][0] = '\0';
	}

	char *start = timeStr;
	char *end = strstr(start, " ");
	int l = 0;
	while (end != NULL && l < NUM_LINES) {
		// Check word for bold prefix
		if (*start == '*' && end - start > 1)
		{
			// Mark line bold and move start to the first character of the word
			format[l] = 'b';
			start++;
		}
		else
		{
			// Mark line normal
			format[l] = ' ';
		}

		// Can we add another word to the line?
		if (format[l] == ' ' && *(end + 1) != '*'    // are both lines formatted normal?
			&& end - start < LINE_APPEND_LIMIT - 1)  // is the first word is short enough?
		{
			// See if next word fits
			char *try = strstr(end + 1, " ");
			if (try != NULL && try - start <= LINE_APPEND_LIMIT)
			{
				end = try;
			}
		}

		// copy to line
		*end = '\0';
		strcpy(lines[l++], start);

		// Look for next word
		start = end + 1;
		end = strstr(start, " ");
	}
	
}

// Update screen based on new time
static void display_time(struct tm *t)
{
	// The current time text will be stored in the following strings
	char textLine[NUM_LINES][BUFFER_SIZE];
	char format[NUM_LINES];

	time_to_lines(t->tm_hour, t->tm_min, t->tm_sec, textLine, format);
	
	int nextNLines = configureLayersForText(textLine, format);

	int delay = 0;
	for (int i = 0; i < NUM_LINES; i++) {
		if (nextNLines != currentNLines || needToUpdateLine(&lines[i], textLine[i])) {
			updateLineTo(&lines[i], textLine[i], delay);
			delay += ANIMATION_STAGGER_TIME;
		}
	}
	
	currentNLines = nextNLines;
}

// Update screen based on new time
static void refresh_toolbar(struct tm *t, BatteryChargeState *battState)
{
    static char dateBuf[BUFFER_SIZE_DATE];
    static char battBuf[BUFFER_SIZE_BATT];
    static char btBuf[BUFFER_SIZE_BT];
    BatteryChargeState battStatePeeked;
    bool btConnected = bluetooth_connection_service_peek();

    if(displayToolbar == false) {
        return;
    }
    if(t != NULL) {
        snprintf(dateBuf, BUFFER_SIZE_DATE, "%d. %s", t->tm_mday, get_month_text(lang, t->tm_mon));
    	text_layer_set_text(toolbar.dateLayer, dateBuf);
    }

    if(btConnected == true) {
        bitmap_layer_set_bitmap(toolbar.blutoothLayer, toolbar.bt_on);
    } else {
        bitmap_layer_set_bitmap(toolbar.blutoothLayer, toolbar.bt_off);
    }

    if(battState == NULL) {
        battStatePeeked = battery_state_service_peek();
        battState = &battStatePeeked;
    }

    if(battState->is_plugged == true) {
        snprintf(battBuf, BUFFER_SIZE_BATT, "CHRG");
    } else {
        snprintf(battBuf, BUFFER_SIZE_BATT, "%d%%", battState->charge_percent);
    }
	text_layer_set_text(toolbar.batteryLayer, battBuf);
}

static void initLineForStart(Line* line)
{
	// Switch current and next layer
	TextLayer* tmp  = line->currentLayer;
	line->currentLayer = line->nextLayer;
	line->nextLayer = tmp;

	// Move current layer to screen;
	GRect rect = layer_get_frame((Layer *)line->currentLayer);
	rect.origin.x = 0;
	layer_set_frame((Layer *)line->currentLayer, rect);
}

// Update screen without animation first time we start the watchface
static void display_initial_time(struct tm *t)
{
	// The current time text will be stored in the following strings
	char textLine[NUM_LINES][BUFFER_SIZE];
	char format[NUM_LINES];

	time_to_lines(t->tm_hour, t->tm_min, t->tm_sec, textLine, format);

	// This configures the nextLayer for each line
	currentNLines = configureLayersForText(textLine, format);

	// Set the text and configure layers to the start position
	for (int i = 0; i < currentNLines; i++)
	{
		updateLayerText(lines[i].nextLayer, textLine[i]);
		// This call switches current- and nextLayer
		initLineForStart(&lines[i]);
	}	
}

// Time handler called every minute by the system
static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed)
{
	t = tick_time;
	display_time(tick_time);
	/* No need to do this on every minute - but eaier */
	refresh_toolbar(tick_time, NULL);
}

static void handle_bt_state(bool connected)
{
	refresh_toolbar(NULL, NULL);
}


static void handle_battery_state(BatteryChargeState chargeState)
{
	refresh_toolbar(NULL, &chargeState);
}

/**
 * Debug methods. For quickly debugging enable debug macro on top to transform the watchface into
 * a standard app and you will be able to change the time with the up and down buttons
 */
#if DEBUG

static void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	(void)recognizer;

	t->tm_min += 5;
	if (t->tm_min >= 60) {
		t->tm_min = 0;
		t->tm_hour += 1;
		
		if (t->tm_hour >= 24) {
			t->tm_hour = 0;
		}
	}
	display_time(t);
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
	(void)recognizer;

	t->tm_min -= 5;
	if (t->tm_min < 0) {
		t->tm_min = 55;
		t->tm_hour -= 1;
		
		if (t->tm_hour < 0) {
			t->tm_hour = 23;
		}
	}
	display_time(t);
}

static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

#endif

static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context)
{
	APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
	GTextAlignment alignment;
	switch (key) {
		case TEXT_ALIGN_KEY:
			text_align = new_tuple->value->uint8;
			persist_write_int(TEXT_ALIGN_KEY, text_align);
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Set text alignment: %u", text_align);

			alignment = lookup_text_alignment(text_align);
			for (int i = 0; i < NUM_LINES; i++)
			{
				text_layer_set_text_alignment(lines[i].currentLayer, alignment);
				text_layer_set_text_alignment(lines[i].nextLayer, alignment);
				layer_mark_dirty(text_layer_get_layer(lines[i].currentLayer));
				layer_mark_dirty(text_layer_get_layer(lines[i].nextLayer));
			}
			break;
		case INVERT_KEY:
			invert = new_tuple->value->uint8 == 1;
			persist_write_bool(INVERT_KEY, invert);
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Set invert: %u", invert ? 1 : 0);

			layer_set_hidden(inverter_layer_get_layer(inverter_layer), !invert);
			layer_mark_dirty(inverter_layer_get_layer(inverter_layer));
			break;
		case LANGUAGE_KEY:
			lang = (Language) new_tuple->value->uint8;
			persist_write_int(LANGUAGE_KEY, lang);
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Set language: %u", lang);

			if (t)
			{
				display_time(t);
			}
            break;
		case DISPLAY_TOOLBAR_KEY:
			displayToolbar = new_tuple->value->uint8 == 1;
			persist_write_bool(DISPLAY_TOOLBAR_KEY, displayToolbar);
			APP_LOG(APP_LOG_LEVEL_DEBUG, "Set display toolbar: %u", displayToolbar ? 1 : 0);

            refresh_toolbar(NULL, NULL);
			break;
	}
}

static void init_line(Line* line)
{
	// Create layers with dummy position to the right of the screen
	line->currentLayer = text_layer_create(GRect(144, 0, 144, 50));
	line->nextLayer = text_layer_create(GRect(144, 0, 144, 50));

	// Configure a style
	configureLightLayer(line->currentLayer);
	configureLightLayer(line->nextLayer);

	// Set the text buffers
	line->lineStr1[0] = '\0';
	line->lineStr2[0] = '\0';
	text_layer_set_text(line->currentLayer, line->lineStr1);
	text_layer_set_text(line->nextLayer, line->lineStr2);

	// Initially there are no animations
	line->animation1 = NULL;
	line->animation2 = NULL;
}

static void destroy_line(Line* line)
{
	// Free layers
	text_layer_destroy(line->currentLayer);
	text_layer_destroy(line->nextLayer);
}

static void configureToolbarText(TextLayer *textLayer, GTextAlignment alignment)
{
	text_layer_set_font(textLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_text_color(textLayer, (invert == true) ? GColorBlack : GColorWhite);
	text_layer_set_background_color(textLayer, GColorClear);
	text_layer_set_text_alignment(textLayer, alignment);
}

static void init_toolbar(Toolbar* tb, GRect windowBounds)
{
	// Create layers with dummy position to the right of the screen
	tb->dateLayer = text_layer_create(GRect(0, windowBounds.size.h - TOOLBAR_HEIGHT, windowBounds.size.w - (TOOLBAR_BATT_WIDTH + TOOLBAR_BT_WIDTH), TOOLBAR_HEIGHT));
    configureToolbarText(tb->dateLayer, GTextAlignmentLeft);

    tb->bt_on = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_ON);
    tb->bt_off = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BT_OFF);
	tb->blutoothLayer = bitmap_layer_create(GRect(windowBounds.size.w - (TOOLBAR_BATT_WIDTH + TOOLBAR_BT_WIDTH), windowBounds.size.h - TOOLBAR_HEIGHT, TOOLBAR_BT_WIDTH, TOOLBAR_HEIGHT));

	tb->batteryLayer = text_layer_create(GRect(windowBounds.size.w - TOOLBAR_BATT_WIDTH, windowBounds.size.h - TOOLBAR_HEIGHT, TOOLBAR_BATT_WIDTH, TOOLBAR_HEIGHT));
    configureToolbarText(tb->batteryLayer, GTextAlignmentRight);
}

static void destroy_toolbar(Toolbar* tb)
{
	// Free layer
	text_layer_destroy(tb->dateLayer);
	text_layer_destroy(tb->batteryLayer);

	//Destroy GBitmaps
    gbitmap_destroy(tb->bt_on);
    gbitmap_destroy(tb->bt_off);

    //Destroy BitmapLayers
    bitmap_layer_destroy(tb->blutoothLayer);
}

static void window_load(Window *window)
{
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);

	// Init and load lines
	for (int i = 0; i < NUM_LINES; i++)
	{
		init_line(&lines[i]);
		layer_add_child(window_layer, (Layer *)lines[i].currentLayer);
		layer_add_child(window_layer, (Layer *)lines[i].nextLayer);
	}

    init_toolbar(&toolbar, bounds);
    layer_add_child(window_layer, (Layer *)toolbar.dateLayer);
    layer_add_child(window_layer, (Layer *)toolbar.blutoothLayer);
    layer_add_child(window_layer, (Layer *)toolbar.batteryLayer);

	inverter_layer = inverter_layer_create(bounds);
	layer_set_hidden(inverter_layer_get_layer(inverter_layer), !invert);
	layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));

	// Configure time on init
	time_t raw_time;

	time(&raw_time);
	t = localtime(&raw_time);
	display_initial_time(t);
	refresh_toolbar(t, NULL);

	Tuplet initial_values[] = {
		TupletInteger(TEXT_ALIGN_KEY,      (uint8_t) text_align),
		TupletInteger(INVERT_KEY,          (uint8_t) invert ? 1 : 0),
		TupletInteger(LANGUAGE_KEY,        (uint8_t) lang),
		TupletInteger(DISPLAY_TOOLBAR_KEY, (uint8_t) displayToolbar ? 1 : 0),
	};

	app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
			sync_tuple_changed_callback, sync_error_callback, NULL);
}

static void window_unload(Window *window)
{
	app_sync_deinit(&sync);

	// Free layers
	inverter_layer_destroy(inverter_layer);
	for (int i = 0; i < NUM_LINES; i++)
	{
		destroy_line(&lines[i]);
	}
	destroy_toolbar(&toolbar);
}

static void handle_init() {
	// Load settings from persistent storage
	if (persist_exists(TEXT_ALIGN_KEY))
	{
		text_align = persist_read_int(TEXT_ALIGN_KEY);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Read text alignment from store: %u", text_align);
	}
	if (persist_exists(INVERT_KEY))
	{
		invert = persist_read_bool(INVERT_KEY);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Read invert from store: %u", invert ? 1 : 0);
	}
	if (persist_exists(LANGUAGE_KEY))
	{
		lang = (Language) persist_read_int(LANGUAGE_KEY);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Read language from store: %u", lang);
	}
	if (persist_exists(DISPLAY_TOOLBAR_KEY))
	{
		displayToolbar = persist_read_bool(DISPLAY_TOOLBAR_KEY);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Read display toolbar from store: %u", displayToolbar ? 1 : 0);
	}

	window = window_create();
	window_set_background_color(window, GColorBlack);
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload
	});

	// Initialize message queue
	const int inbound_size = 64;
	const int outbound_size = 64;
	app_message_open(inbound_size, outbound_size);

	const bool animated = true;
	window_stack_push(window, animated);

    // Subscribe to BT updates
    bluetooth_connection_service_subscribe(handle_bt_state);

    // Subscribe to battery updates
    battery_state_service_subscribe(handle_battery_state);

	// Subscribe to minute ticks
	tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);

#if DEBUG
	// Button functionality
	window_set_click_config_provider(window, (ClickConfigProvider) click_config_provider);
#endif
}

static void handle_deinit()
{
	// Free window
	window_destroy(window);
}

int main(void)
{
	handle_init();
	app_event_loop();
	handle_deinit();
}

