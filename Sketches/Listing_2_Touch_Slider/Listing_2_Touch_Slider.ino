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

  lv_obj_t * slider = lv_slider_create(lv_scr_act());
  lv_obj_set_size(slider, 180, 10);
  lv_obj_align(slider, LV_ALIGN_CENTER, 0, 0);
}

void loop() {
  lv_timer_handler();
  delay(5);
}