#include <pebble.h>
#include <pebble_fonts.h>

#define GColorDukeBlueARGB8 ((uint8_t)0b11000010)
#define JoLeeSucksBlue (GColor8){ .argb = 0b11000010}

#define CIRCLE_RADIUS 16
#define CIRCLE_LINE_THICKNESS 2
#define CIRCLE_PADDING 12 - CIRCLE_RADIUS
#define CELL_SIZE (2 * (CIRCLE_RADIUS + CIRCLE_PADDING))
#define SIDE_PADDING (144 - (5.5 * CELL_SIZE)) / 2
#define TOP_PADDING 10
#define CELLS_PER_ROW 4
#define CELLS_PER_COLUMN 6
#define HOURS_FIRST_DIGIT_ROW 0
#define HOURS_SECOND_DIGIT_ROW 1
#define MINUTES_FIRST_DIGIT_ROW 2
#define MINUTES_SECOND_DIGIT_ROW 3
#define SECONDS_FIRST_DIGIT_ROW 4
#define SECONDS_SECOND_DIGIT_ROW 5
#define DEFAULT_MAX_COLS 4
#define HOURS_FIRST_DIGIT_MAX_COLS 2
#define MINUTES_FIRST_DIGIT_MAX_COLS 3
#define SECONDS_FIRST_DIGIT_MAX_COLS 3  
  
static Window *window;
static TextLayer *day_layer, *charge_layer;
static Layer *main_layer;
static char day_info[13];
char mCharge[5];
static GBitmap *charge_bitmap;
static BitmapLayer *bitmap_layer;
static GBitmap *phone_bitmap;
static BitmapLayer *phone_layer;
bool connectionStatus = false;


static void draw_cell(GContext *ctx, GPoint center, bool filled) {
  graphics_context_set_fill_color(ctx, JoLeeSucksBlue);
    graphics_fill_rect(ctx, GRect(center.x, center.y, CIRCLE_RADIUS, CIRCLE_RADIUS), 2, GCornersAll);

  if (!filled) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_fill_rect(ctx, GRect(center.x+CIRCLE_LINE_THICKNESS,  center.y+CIRCLE_LINE_THICKNESS, CIRCLE_RADIUS-CIRCLE_LINE_THICKNESS*2, CIRCLE_RADIUS-CIRCLE_LINE_THICKNESS*2), 2, GCornersAll);
  }
}

static GPoint get_center_point_from_cell_location(unsigned short x, unsigned short y) {
  return GPoint(SIDE_PADDING + (CELL_SIZE / 2) + (CELL_SIZE * y), (CELL_SIZE / 2) + (CELL_SIZE * (4-x) + TOP_PADDING));
}

static void draw_cell_row_for_digit(GContext *ctx, unsigned short digit, unsigned short max_columns_to_display, unsigned short cell_row) {
  // Converts the supplied decimal digit into Binary Coded Decimal form and
  // then draws a row of cells on screen--'1' binary values are filled, '0' binary values are not filled.
  // `max_columns_to_display` restricts how many binary digits are shown in the row.
  for (int i = 0; i < max_columns_to_display; i++) {
    draw_cell(ctx, get_center_point_from_cell_location(i, cell_row), (digit >> i) & 0x1);
  }
}

static unsigned short get_display_hour(unsigned short hour) {
  if (clock_is_24h_style()) {
    return hour;
  }

  unsigned short display_hour = hour % 12;

  return display_hour ? display_hour : 12;
}

char* days_of_week[7] = {"Sun,", "Mon,", "Tues,", "Wed,", "Thu,", "Fri,", "Sat,"};
char* months_of_year[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

static char seconds_singles_buffer[30]; 
static char seconds_tens_buffer[30];

void oldUpdate(Layer *layer, GContext *ctx) {
  
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  unsigned short display_hour = get_display_hour(t->tm_hour);

  draw_cell_row_for_digit(ctx, display_hour / 10, HOURS_FIRST_DIGIT_MAX_COLS, HOURS_FIRST_DIGIT_ROW);
  draw_cell_row_for_digit(ctx, display_hour % 10, DEFAULT_MAX_COLS, HOURS_SECOND_DIGIT_ROW);

  draw_cell_row_for_digit(ctx, t->tm_min / 10, MINUTES_FIRST_DIGIT_MAX_COLS, MINUTES_FIRST_DIGIT_ROW);
  draw_cell_row_for_digit(ctx, t->tm_min % 10, DEFAULT_MAX_COLS, MINUTES_SECOND_DIGIT_ROW);

  graphics_draw_text(ctx, seconds_singles_buffer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS), GRect(50, 90, 144, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  graphics_draw_text(ctx, seconds_tens_buffer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS), GRect(50, 60, 144, 20), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  int secondsplace = t->tm_sec % 10;
  int tensplace = t->tm_sec / 10;
  
  snprintf(seconds_singles_buffer, sizeof(seconds_singles_buffer), "%d", secondsplace);
  snprintf(seconds_tens_buffer, sizeof(seconds_tens_buffer), "%d", tensplace);  


  
}
void update_proc(Layer *layer, GContext *ctx) {

  oldUpdate(layer, ctx);
    time_t time_value = time(NULL);
    struct tm* mTime = localtime(&time_value);
    
  
    int day_int = mTime->tm_mday;
    char* day_string = "01";
    day_string[1] = '0' + day_int%10;
    day_string[0] = '0' + day_int/10;
    memset(day_info, 0, sizeof(day_info));
    strcpy(day_info, days_of_week[mTime->tm_wday]);
    strcat(day_info, " ");
    strcat(day_info, months_of_year[mTime->tm_mon]);
    strcat(day_info, " ");
    strcat(day_info, day_string);
    text_layer_set_text(day_layer, day_info);
}

void handle_tick(struct tm *tick_time, TimeUnits units_changed)
{
    layer_mark_dirty(main_layer);    //keeping main
}

void battery_state_handler(BatteryChargeState charge)
{
    int chargeLevel = charge.charge_percent;
    memset(mCharge, 0, sizeof(mCharge));
    if(chargeLevel >= 100 || (charge.is_plugged && !charge.is_charging))
    {
        mCharge[0] = '1';
        mCharge[1] = '0';
        mCharge[2] = '0';
    }
    else if(chargeLevel <= 0)
    {
        mCharge[0] = '0';
    }
    else
    {
        mCharge[0] = '0' + chargeLevel/10%10;
        mCharge[1] = '0' + chargeLevel%10;
    }
    strcat(mCharge, "%");
    text_layer_set_text(charge_layer, mCharge);
    if(charge.is_charging)
    {
        if(!charge_bitmap)
        {
            charge_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CHARGE_WHITE);
            bitmap_layer_set_bitmap(bitmap_layer, charge_bitmap);
        }
    }
    else
    {
        if(charge_bitmap)
        {
            bitmap_layer_set_bitmap(bitmap_layer, NULL);
            gbitmap_destroy(charge_bitmap);
            charge_bitmap = NULL;
        }
    }
}

void bluetooth_state_handler(bool connected)
{
    if(!connected && connectionStatus)
    {
        vibes_double_pulse();
    }
    connectionStatus = connected;
    if(connected)
    {
        if(!phone_bitmap)
        {
            phone_bitmap = gbitmap_create_with_resource(RESOURCE_ID_PHONE_WHITE);
            bitmap_layer_set_bitmap(phone_layer, phone_bitmap);
        }
    }
    else
    {
        if(phone_bitmap)
        {
            bitmap_layer_set_bitmap(phone_layer, NULL);
            gbitmap_destroy(phone_bitmap);
            phone_bitmap = NULL;
        }
    }
}

static void window_load(Window *window) {
  
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    day_layer = text_layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, 20 } });
    main_layer = layer_create((GRect) { .origin = { 0, 0 }, .size = { bounds.size.w, bounds.size.h } });
    charge_layer = text_layer_create((GRect) { .origin = {0, 0 }, .size = { 30, 20} });
    bitmap_layer = bitmap_layer_create((GRect) { .origin = {25, 0}, .size={12, 20}});
    bitmap_layer_set_background_color(bitmap_layer, GColorClear);
    bitmap_layer_set_compositing_mode(bitmap_layer, GCompOpAssignInverted);
    
    phone_layer = bitmap_layer_create((GRect) { .origin = {bounds.size.w - 12, 0}, .size={12, 20}});
    bitmap_layer_set_background_color(phone_layer, GColorClear);
    bitmap_layer_set_compositing_mode(phone_layer, GCompOpAssignInverted);
    
    text_layer_set_text(day_layer, "");
    text_layer_set_background_color(day_layer, GColorBlack);
    text_layer_set_text_color(day_layer, GColorWhite);
    text_layer_set_background_color(charge_layer, GColorBlack);
    text_layer_set_text_color(charge_layer, GColorWhite);
    text_layer_set_text_alignment(charge_layer, GTextAlignmentLeft);
    text_layer_set_text(charge_layer, "");
    text_layer_set_text_alignment(day_layer, GTextAlignmentCenter);
    
    layer_set_update_proc(main_layer, update_proc);
    layer_add_child(window_layer, main_layer);
    layer_add_child(window_layer, text_layer_get_layer(day_layer));
    layer_add_child(window_layer, text_layer_get_layer(charge_layer));
    layer_add_child(window_layer, bitmap_layer_get_layer(bitmap_layer));
    layer_add_child(window_layer, bitmap_layer_get_layer(phone_layer));
    
    tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
    battery_state_service_subscribe(battery_state_handler);
    bluetooth_connection_service_subscribe(bluetooth_state_handler);
    battery_state_handler(battery_state_service_peek());
    bluetooth_state_handler(bluetooth_connection_service_peek());
}

static void window_unload(Window *window) {
    text_layer_destroy(day_layer);
    text_layer_destroy(charge_layer);
    layer_destroy(main_layer);
    bitmap_layer_destroy(bitmap_layer);
    bitmap_layer_destroy(phone_layer);
    battery_state_service_unsubscribe();
    bluetooth_connection_service_unsubscribe();
    if(charge_bitmap)
    {
        gbitmap_destroy(charge_bitmap);
        charge_bitmap = NULL;
    }
    if(phone_bitmap)
    {
        gbitmap_destroy(phone_bitmap);
        phone_bitmap = NULL;
    }
}

//----------------------jif binary


//---------------------imported code

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
    window_set_background_color(window, GColorBlack);
    window_set_fullscreen(window, true);
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  app_event_loop();
  deinit();
}
