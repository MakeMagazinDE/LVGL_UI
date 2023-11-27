#include <TFT_eSPI.h>
#include <Adafruit_FT6206.h>
#include <lvgl.h>

#define screenWidth 240
#define screenHeight 320
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);
Adafruit_FT6206 touch = Adafruit_FT6206();

static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf[screenWidth * 10];

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p){
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
	
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t*)&color_p->full, w * h, true);
    tft.endWrite();
	
    lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t * drv, lv_indev_data_t*data){
  TS_Point touchPoint = touch.getPoint();
  if(touchPoint.x > 0 || touchPoint.y > 0){
    data->state = LV_INDEV_STATE_PR;
    data->point.x = 240 - touchPoint.x;
    data->point.y = 320 - touchPoint.y;
  }
  else {
    data->state = LV_INDEV_STATE_REL;
  }
}

void setup() {
  lv_init();

  tft.begin();
  
  Wire.begin(19, 22);
  touch.begin();
  
  lv_disp_draw_buf_init(&disp_buf, buf, NULL, screenWidth * 10);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);

  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x464B55), LV_PART_MAIN);

  lv_obj_t* box = lv_obj_create(lv_scr_act());
  lv_obj_set_width(box, 200);
  lv_obj_set_height(box, 280);
  lv_obj_set_align(box, LV_ALIGN_CENTER);
  lv_obj_set_style_radius(box, 25, LV_PART_MAIN);
  lv_obj_set_style_bg_color(box, lv_color_hex(0x646464), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_color(box, lv_color_hex(0x3C414B), LV_PART_MAIN);
  lv_obj_set_style_bg_grad_dir(box, LV_GRAD_DIR_VER, LV_PART_MAIN);
  lv_obj_set_style_border_color(box, lv_color_hex(0x2D323C), LV_PART_MAIN);
  lv_obj_set_style_shadow_width(box, 40, LV_PART_MAIN);
  lv_obj_set_style_shadow_spread(box, 0, LV_PART_MAIN);
  lv_obj_set_style_shadow_ofs_y(box, 15, LV_PART_MAIN);

  lv_obj_t* slider = lv_slider_create(box);
  lv_slider_set_value(slider, 75, LV_ANIM_OFF);
  lv_obj_set_width(slider, 142);
  lv_obj_set_height(slider, 13);
  lv_obj_set_x(slider, 0);
  lv_obj_set_y(slider, -21);
  lv_obj_set_align(slider, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_style_bg_color(slider, lv_color_hex(0x0F1215), LV_PART_MAIN);
  lv_obj_set_style_outline_color(slider, lv_color_hex(0x1E242D), LV_PART_MAIN);
  lv_obj_set_style_outline_width(slider, 6, LV_PART_MAIN);

  lv_obj_set_style_bg_color(slider, lv_color_hex(0xC5C5C5), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_color(slider, lv_color_hex(0xE9BB00), LV_PART_INDICATOR);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR);

  lv_obj_set_style_bg_color(slider, lv_color_hex(0x5F6464), LV_PART_KNOB);
  lv_obj_set_style_bg_grad_color(slider, lv_color_hex(0x3C424B), LV_PART_KNOB);
  lv_obj_set_style_bg_grad_dir(slider, LV_GRAD_DIR_VER, LV_PART_KNOB);
  lv_obj_set_style_outline_color(slider, lv_color_hex(0x373C46), LV_PART_KNOB);
  lv_obj_set_style_outline_width(slider, 2, LV_PART_KNOB);
  lv_obj_set_style_shadow_color(slider, lv_color_hex(0x000000), LV_PART_KNOB);
  lv_obj_set_style_shadow_width(slider, 10, LV_PART_KNOB);
  lv_obj_set_style_shadow_spread(slider, 3, LV_PART_KNOB);

  
  LV_IMG_DECLARE(lightbulb);
  lv_obj_t* bulbIcon = lv_img_create(box);
  lv_img_set_src(bulbIcon, &lightbulb);
  lv_obj_set_style_img_recolor(bulbIcon, lv_color_hex(0x1E242D), LV_PART_MAIN);
  lv_obj_set_style_img_recolor_opa(bulbIcon, LV_OPA_COVER, LV_PART_MAIN);
  lv_obj_align(bulbIcon, LV_ALIGN_CENTER, 0, -22);

}

void loop() {
  lv_timer_handler();
  delay(5);
}