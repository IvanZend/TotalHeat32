/*
 * total_device.cpp
 *
 *  Created on: Feb 9, 2022
 *      Author: TZprog2
 */

#include <total_device.hpp>
#include <iostream>
#include <cstring>

#include <vector>


#ifdef __cplusplus
extern "C"
{
#endif


using namespace std;


extern LTDC_HandleTypeDef hltdc;
extern SDRAM_HandleTypeDef hsdram1;

void TFT_DrawBitmap_d(uint32_t Xpos, uint32_t Ypos, const uint8_t *bitmap, uint16_t width, uint16_t height,  uint8_t layer){
	 uint32_t y,x,p=Xpos,c=0; //len=strlen((char*)bitmap);
  /* Set the address */
  uint32_t  address = hltdc.LayerCfg[layer].FBStartAdress +  Ypos*X_SIZE*4;
  /* Bypass the bitmap header */
  for(y=0;y<height;y++){
    //for(x=1;x<width+1;x++){
		for(x=0;x<width;x++){
		c=0;
		c=0x00000000;
		c |= (bitmap[((y*width)+x)*4+3]);
		c |= (bitmap[((y*width)+x)*4+2]) << 8;
		c |= (bitmap[((y*width)+x)*4+1]) << 16;
		c |= (bitmap[((y*width)+x)*4+0]) << 24;

      *(uint32_t*) (address + (y+x+p)*4) = c;//TTZ_COL_ORAN;
      if((y+x)>=(X_SIZE*Y_SIZE)) return;
    }
	p=p+X_SIZE-1;
  }

  vector<int> constant_images;
  return;
}



void TFT_FillScreen(uint32_t color)
{
 uint32_t i;
 uint32_t n = hltdc.LayerCfg[0].ImageHeight*hltdc.LayerCfg[0].ImageWidth;
 for(i=0;i<n;i++)
 {
  *(uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (i*4)) = color;
 }
}


#define DISPLAY_WIDTH                   X_SIZE
#define DISPLAY_HEIGHT                  Y_SIZE
#define BOUNCE_MAX_SAMPLES              4
#define CALIBRATION_OFFSET_X            0 //108
#define CALIBRATION_OFFSET_Y            0 //-220
#define PROFILE_WORD_COORD_X            23
#define PROFILE_WORD_COORD_Y            56
#define I2C_TRANSMISSION_INTERVAL_MS    20
#define TMP_BUF_REGISTER_SIZE           2
#define TOUCH_GLITCH_COUNTER            4
#define I2C_ADDRESS_TOUCH               0x14
#define I2C_POINT_1_X_LOW_BYTE          0x8150
#define I2C_POINT_1_X_HIGH_BYTE         0x8151
#define I2C_POINT_1_Y_LOW_BYTE          0x8152
#define I2C_POINT_1_Y_HIGH_BYTE         0x8153
#define I2C_POINT_1_SIZE_W              0x8154
#define I2C_POINT_1_SIZE_H              0x8155
#define I2C_TOUCH_BUFF_STATUS           0x814E
#define I2C_TOUCH_REGISTER_COMMAND      0x8040
#define LAYER_PAGE_0		            0
#define LAYER_PAGE_1		            1
#define LAYER_PAGE_2		            2
#define LAYER_PAGE_3		            3
#define LAYER_PAGE_4		            4
#define IMG_BACKGR_IN_VECT              10
#define TIME_FIGURE_Y_OFFSET            21
#define FONT_33_GAP_PIX                 4
#define FONT_33_NMBR_WIDTH              24
#define FONT_33_NMBR_HEIGHT             33
#define VALUES_NMBRS_COORD_Y            122
#define TRIANGLE_UP_COORD_Y             191
#define TRIANGLE_DOWN_COORD_Y           308
#define TRIANGLE_HIDDEN_BTN_AREA        11
#define EDIT_NMBR_X                     12
#define EDIT_NMBR_Y                     23
#define EDIT_TIME_OFFSET_X              8
#define EDIT_BACKGR_Y                   229
#define HOUR_LTTR_X                     14
#define HOUR_LTTR_Y                     62
#define MINUTE_LTTR_X                   HOUR_LTTR_X
#define MINUTE_LTTR_Y                   HOUR_LTTR_Y
#define MS_PER_SEC                      1000
#define MAX_NMBR_IN_STRING              3
#define I2C_JITTER_FILTER_ADDR          0x8040
#define JITTER_FILTER_VALUE             0x0b
#define MAX_HUNDRED_CELSIUS             4
#define SHOW_TEN_TEMPER                 true


enum Img_vect_list
{
    CONSTANT_IMG,
    CHANGEABLE_IMG,
    BTN_PRESSED_IMG,
    BTN_RELEASED_IMG,
    BTN_BLOCKED_IMG
};

enum Display_page
{
    START_PAGE
};

enum Alignment
{
    ALIGN_LEFT,
    ALIGN_RIGHT
};

enum Buttons_list
{
    NO_BUTTON,
    VACUUM_START_STOP,
    VACUUM_CONNECT_TO_TMER,
    TEMPER_START_STOP,
    TEMPER_CONNECT_TO_TIMER,
    TIMER_START,
    TIMER_STOP,
    TIMER_PAUSE,
    VACUUM_DECREASE_0,
    VACUUM_INCREASE_0,
    TEMPER_DECREASE_0,
    TEMPER_DECREASE_1,
    TEMPER_DECREASE_2,
    TEMPER_INCREASE_0,
    TEMPER_INCREASE_1,
    TEMPER_INCREASE_2,
    TIMER_DECREASE_0,
    TIMER_DECREASE_1,
    TIMER_DECREASE_2,
    TIMER_DECREASE_3,
    TIMER_INCREASE_0,
    TIMER_INCREASE_1,
    TIMER_INCREASE_2,
    TIMER_INCREASE_3
};

typedef struct
{
const uint8_t *data;
uint16_t width;
uint16_t height;
uint8_t dataSize;
} tImage;

typedef struct
{
    uint16_t img_coord_x, img_coord_y;      // координаты изображения внутри виджета
    tImage image_struct;
} ImageObj;


#include "Images/img_tile_background.h"
#include "Images/img_frame_rounding_down_left.h"
#include "Images/img_frame_rounding_down_right.h"
#include "Images/img_frame_rounding_top_left.h"
#include "Images/img_frame_rounding_top_right.h"
#include "Images/img_vertic_frame.h"
#include "Images/img_horiz_frame.h"
#include "Images/img_backgr_nmbr_time.h"
#include "Images/img_backgr_nmbr_vac_temper.h"
#include "Images/img_bar_backgr.h"
#include "Images/img_btn_timer_pause_rlsd.h"
#include "Images/img_btn_timer_start_rlsd.h"
#include "Images/img_btn_timer_stop_rlsd.h"
#include "Images/img_btn_triangle_down.h"
#include "Images/img_btn_triangle_up.h"
#include "Images/img_btn_vac_temper_start_rlsd.h"
#include "Images/img_btn_vac_temper_cnct_rlsd.h"
#include "Images/img_colon.h"
#include "Images/img_lttr_hour.h"
#include "Images/img_lttr_minute.h"
#include "Images/img_pointing_line.h"
#include "Images/img_title_atm.h"
#include "Images/img_title_temper.h"
#include "Images/img_title_time.h"
#include "Images/img_title_vac.h"
#include "Images/img_title_deg_celsius.h"
#include "Images/img_btn_timer_pause_blocked.h"
#include "Images/img_btn_timer_start_blocked.h"
#include "Images/img_btn_timer_stop_blocked.h"
#include "Images/img_btn_vac_temper_cnct_prsd.h"
#include "Images/img_btn_timer_pause_prsd.h"
#include "Images/font_33/numbers_33_0.h"
#include "Images/font_33/numbers_33_1.h"
#include "Images/font_33/numbers_33_2.h"
#include "Images/font_33/numbers_33_3.h"
#include "Images/font_33/numbers_33_4.h"
#include "Images/font_33/numbers_33_5.h"
#include "Images/font_33/numbers_33_6.h"
#include "Images/font_33/numbers_33_7.h"
#include "Images/font_33/numbers_33_8.h"
#include "Images/font_33/numbers_33_9.h"
#include "Images/font_33/numbers_33_backgr.h"
#include "Images/img_btn_timer_stop_prsd.h"
#include "Images/img_no_colon.h"
#include "Images/img_33_dot.h"
#include "Images/img_backgr_title_vac.h"
#include "Images/img_pressure_sensor_icon_crossed.h"
#include "Images/img_pressure_sensor_icon_enabled.h"
#include "Images/img_btn_vac_temper_start_blocked.h"

void test_output(void)
{
	HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(LAYER_PAGE_0), 0);

	TFT_DrawBitmap_d(0, 0, img_btn_timer_pause_rlsd.data, img_btn_timer_pause_rlsd.width, img_btn_timer_pause_rlsd.height, LAYER_PAGE_0);

	/*
	TFT_DrawBitmap_d(0, 0, img_btn_timer_pause_rlsd.data, img_btn_timer_pause_rlsd.width, img_btn_timer_pause_rlsd.height, LAYER_PAGE_1);
	TFT_DrawBitmap_d(0, 0, img_btn_timer_pause_rlsd.data, img_btn_timer_pause_rlsd.width, img_btn_timer_pause_rlsd.height, LAYER_PAGE_2);
	TFT_DrawBitmap_d(0, 0, img_btn_timer_pause_rlsd.data, img_btn_timer_pause_rlsd.width, img_btn_timer_pause_rlsd.height, LAYER_PAGE_3);
	TFT_DrawBitmap_d(0, 0, img_btn_timer_pause_rlsd.data, img_btn_timer_pause_rlsd.width, img_btn_timer_pause_rlsd.height, LAYER_PAGE_4);
	*/
}

// класс виджета
class Widget
{
    public:
    Buttons_list button_name;               // имя кнопки виджета (может быть NO_BUTTON)
    uint16_t wgt_coord_x, wgt_coord_y, wgt_width, wgt_height;   // координаты и габариты виджета
    vector<ImageObj> constant_images;       // постоянные изображения
    vector<ImageObj> changeable_images;     // изменяемые изображения
    vector<ImageObj> pressed_btn_images;    // изображения при нажатой кнопке
    vector<ImageObj> released_btn_images;   // изображения при отпущенной кнопке
    vector<ImageObj> blocked_btn_images;    // изображения при заблокированной кнопке
    bool button_is_pressed;                 // флаг нажатия кнопки
    bool btn_locked;                        // флаг блокировки кнопки
    uint8_t current_layer;
    bool widget_btn_is_active;

    Widget(Buttons_list btn_name);  // конструктор с нулевыми координатами
    Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y);  // конструктор без габаритов
    Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y, uint16_t wgt_wdth, uint16_t wgt_hght);    // конструктор с габаритами


    void add_img_to_wgt(Img_vect_list vect_to_add_type, tImage add_image_generated);    // добавляем в виджет изображение с координатами 0, 0
    void add_img_to_wgt(Img_vect_list vect_to_add_type, tImage add_image_generated, uint16_t add_img_coord_x, uint16_t add_img_coord_y);    // добавляем изображение в виджет
    void add_obj_to_wgt(Img_vect_list obj_vect_type, ImageObj img_obj_to_add);
    void draw_img_vector(vector<ImageObj> img_vector_to_draw, uint16_t parent_wgt_coord_x, uint16_t parent_wgt_coord_y);    // отрисовываем вектор изображений

    void set_rectangle_page_0(void);
    void tile_area(tImage image_to_tile, uint16_t area_width, uint16_t area_hight);
    void change_image_in_widget(tImage image_to_output);
    void change_image_in_widget(tImage image_to_output, uint16_t img_out_coord_x, uint16_t img_out_coord_y);            // изменить изображение изменить изображение в виджете
    void change_value_in_wgt(Alignment numbers_align, uint8_t font_space, vector<tImage>& img_font, uint32_t value_to_displ);   // изменить числовое значение в виджете
    uint16_t string_align_right_x(vector<tImage>& img_nmbrs_vect, uint8_t fnt_space, vector<uint8_t>& value_to_align);  // выровнять строку по правой границе виджета
    void lock_button(void);                 // заблокировать кнопку
    void unlock_button(void);               // разблокировать кнопку
    /*
    uint16_t img_center_x(tImage img_to_center);                                // получить координату для размещения изображения в центре виджета
    void temper_input_add_number(uint8_t num_to_enter, uint16_t &variable_to_change);       // добавить разряд к вводимому значению
    void temper_input_backspace(uint16_t &variable_to_change);          // удалить младший разряд из вводимого значения
    void temper_input_reset(uint16_t &variable_to_change);              // сбросить вводимое значение
    void temper_input_enter(uint16_t changed_variable, uint16_t &variable_to_write, uint16_t wgt_to_output_numbr);  // ввод из редактируемого значения в заданное значение

    */

};

// конструктор виджета (с нулевыми координатами)
Widget::Widget(Buttons_list btn_name)
{
    button_name = btn_name;
    wgt_coord_x = 0;
    wgt_coord_y = 0;
    wgt_width = 0;
    wgt_height = 0;
    button_is_pressed = false;
    btn_locked = false;
    widget_btn_is_active = 0;
    current_layer = LAYER_PAGE_0;
}

// конструктор виджета (без габаритов)
Widget::Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y)
{
    button_name = btn_name;
    wgt_coord_x = wgt_x;
    wgt_coord_y = wgt_y;
    wgt_width = 0;
    wgt_height = 0;
    button_is_pressed = false;
    btn_locked = false;
    widget_btn_is_active = 0;
    current_layer = LAYER_PAGE_0;
}

// конструктор виджета (с габаритами)
Widget::Widget(Buttons_list btn_name, uint16_t wgt_x, uint16_t wgt_y, uint16_t wgt_wdth, uint16_t wgt_hght)
{
    button_name = btn_name;
    wgt_coord_x = wgt_x;
    wgt_coord_y = wgt_y;
    wgt_width = wgt_wdth;
    wgt_height = wgt_hght;
    button_is_pressed = false;
    btn_locked = false;
    widget_btn_is_active = 0;
    current_layer = LAYER_PAGE_0;
}

void Widget::add_img_to_wgt(Img_vect_list vect_to_add_type, tImage add_image_generated)
{
    ImageObj tmp_img_obj_1;
    tmp_img_obj_1.img_coord_x = 0;
    tmp_img_obj_1.img_coord_y = 0;
    tmp_img_obj_1.image_struct = add_image_generated;

    add_obj_to_wgt(vect_to_add_type, tmp_img_obj_1);
}

// добавить изображение в вектор внутри виджета
void Widget::add_img_to_wgt(Img_vect_list vect_to_add_type, tImage add_image_generated, uint16_t add_img_coord_x, uint16_t add_img_coord_y)
{
    ImageObj tmp_img_obj_2;
    tmp_img_obj_2.img_coord_x = add_img_coord_x;
    tmp_img_obj_2.img_coord_y = add_img_coord_y;
    tmp_img_obj_2.image_struct = add_image_generated;

    add_obj_to_wgt(vect_to_add_type, tmp_img_obj_2);
}

void Widget::add_obj_to_wgt(Img_vect_list obj_vect_type, ImageObj img_obj_to_add)
{
    switch (obj_vect_type)
    {
    case CONSTANT_IMG:                                      // изображение не меняется на протяжении всей программы
    {
        constant_images.push_back(img_obj_to_add);
        break;
    }
    case CHANGEABLE_IMG:                                    // изображение меняется при вызове специальной функции
    {
        changeable_images.push_back(img_obj_to_add);
        break;
    }
    case BTN_PRESSED_IMG:                                   // изображение при нажатии кнопки
    {
        pressed_btn_images.push_back(img_obj_to_add);
        break;
    }
    case BTN_RELEASED_IMG:                                  // изображение при отпущенной кнопке
    {
        released_btn_images.push_back(img_obj_to_add);
        break;
    }
    case BTN_BLOCKED_IMG:                                   // изображение при заблокированной кнопке
        blocked_btn_images.push_back(img_obj_to_add);
        break;
    }
}

// отрисовываем изображения, перечисленные в векторе
void Widget::draw_img_vector(vector<ImageObj> img_vector_to_draw, uint16_t parent_wgt_coord_x, uint16_t parent_wgt_coord_y)
{
    for(vector<ImageObj>::size_type ii = 0; ii != img_vector_to_draw.size(); ii++)  // проходимся по вектору изображений
    {
    TFT_DrawBitmap_d(img_vector_to_draw[ii].img_coord_x + parent_wgt_coord_x, \
                    img_vector_to_draw[ii].img_coord_y + parent_wgt_coord_y, \
                    img_vector_to_draw[ii].image_struct.data, \
                    img_vector_to_draw[ii].image_struct.width, \
                    img_vector_to_draw[ii].image_struct.height, \
                    current_layer);                                                    // вызываем встроенную команду marlin
    }
}

uint16_t Widget::string_align_right_x(vector<tImage>& img_nmbrs_vect, uint8_t fnt_space, vector<uint8_t>& value_to_align)
{
    uint16_t string_summ_size = 0;                              // суммарная ширина строки
    for (vector<uint8_t>::size_type i = 0; i < value_to_align.size(); i++)             // проходимся по всем символам строки
    {
        string_summ_size += (img_nmbrs_vect[value_to_align[i]].width + fnt_space);      // прибавляем к суммарной ширине ширину символа и пробел между символами
    }
    if (string_summ_size <= wgt_width)                                                  // если не превышена ширина виджета
    {
        string_summ_size = wgt_width - string_summ_size;                                // получаем координату начала строки в виджете
    }
    else
    {
        string_summ_size = 0;                                                           // иначе обнуляем ширину строки
    }
    return string_summ_size;                                                            // возвращаем ширину строки
}

void Widget::lock_button(void)
{
    button_is_pressed = false;                                          // убираем флаг нажатия
    draw_img_vector(blocked_btn_images, wgt_coord_x, wgt_coord_y);      // меняем изображение кнопки
    btn_locked = true;                                                  // включаем флаг блокировки
}

// разблокируем кнопку
void Widget::unlock_button(void)
{
    button_is_pressed = false;                                          // убираем флаг нажатия
    draw_img_vector(released_btn_images, wgt_coord_x, wgt_coord_y);     // меняем изображение кнопки
    btn_locked = false;                                                 // выключаем флаг блокировки
}

class Touch_GT5688
{
    public:

    Touch_GT5688(void);

    bool power_on_init_commpleted;
    uint8_t coord_glitch_filter;
    void convert_16_bit_addr_to_8_bit_register(uint16_t addr_to_convert, uint8_t* buff_to_fill);
    uint8_t i2c_write_read_register(uint16_t register_address);
    void i2c_write_addr_write_register(uint16_t register_address, uint8_t register_data);
    bool touch_buffer_is_ready(void);
};

typedef struct
{
    uint16_t touch_coord_x, touch_coord_y;
} TouchPoint;



Touch_GT5688::Touch_GT5688(void)
{
    power_on_init_commpleted = 0;
    coord_glitch_filter = 0;
}

void Touch_GT5688::convert_16_bit_addr_to_8_bit_register(uint16_t addr_to_convert, uint8_t* buff_to_fill)
{
    buff_to_fill[0] = addr_to_convert >> 8;
    buff_to_fill[1] = addr_to_convert & 0xFF;
}

uint8_t Touch_GT5688::i2c_write_read_register(uint16_t register_address)
{
    uint8_t tmp_buf_register[TMP_BUF_REGISTER_SIZE];
    //Adafruit_I2CDevice i2c_tmp_instance_touch = Adafruit_I2CDevice(I2C_ADDRESS_TOUCH, &Wire_customs);

    convert_16_bit_addr_to_8_bit_register(register_address, tmp_buf_register);
    //i2c_tmp_instance_touch.write(tmp_buf_register, TMP_BUF_REGISTER_SIZE, true);

    //uint8_t tmp_buf_value[1];
    //i2c_tmp_instance_touch.read(tmp_buf_value, 1, false);

    //return tmp_buf_value[0];
    return 0;
}

void Touch_GT5688::i2c_write_addr_write_register(uint16_t register_address, uint8_t register_data)
{
    uint8_t tmp_buf_register[TMP_BUF_REGISTER_SIZE + 1];
    //Adafruit_I2CDevice i2c_tmp_instance_touch = Adafruit_I2CDevice(I2C_ADDRESS_TOUCH, &Wire_customs);

    convert_16_bit_addr_to_8_bit_register(register_address, tmp_buf_register);
    tmp_buf_register[2] = register_data;
    //i2c_tmp_instance_touch.write(tmp_buf_register, (TMP_BUF_REGISTER_SIZE + 1), true);
}

bool Touch_GT5688::touch_buffer_is_ready(void)
{
    uint8_t status_register = i2c_write_read_register(I2C_TOUCH_BUFF_STATUS);

    return (status_register & 0x80);
}


// класс дисплея (единственный)
class TotalDisplay : public Touch_GT5688
{
    public:

    TotalDisplay(void);                 // конструктор

    Display_page display_page;          // текущая страница интерфейса
    uint16_t display_width, display_height;

    bool display_initialized;
    bool draw_all_completed;
    uint32_t bounce_sample_counter;     // счётчик дребезга
    Buttons_list bounce_btn_buff;       // буфер кнопки, для которой считаем дребезг
    Buttons_list previous_button;       // буфер предыдущей нажатой кнопки
    vector<Widget> start_page;  // страницы
    Display_page current_page_name;
    bool touch_was_released;
    vector<tImage> numbers_33_font_vector;

    void init_widgets_content(void);
    void draw_all_widgets(vector<Widget>& widget_vector_to_draw);
    void tile_background(tImage image_to_tile, uint8_t layer_to_tile); // замостить область повторяющейся картинкой
    void init_widgets_size(vector<Widget>& wgt_vect_to_estimate_size);  // если размер виджета не был задан вручную, он автоматически равен размеру первой картинки - кнопки
    void handle_button_press(Buttons_list pressed_button, vector<Widget>& pressed_page, Display_page current_page_name_to_handle);  // обрабатываем нажатие кнопки
    void i2c_read_touch(void);
    Buttons_list identify_pressed_btn(uint16_t pressing_coord_x, uint16_t pressing_coord_y, vector<Widget> current_page);    // определяем, в какой кнопке лежат координаты нажатия
    void set_layer_for_widgets(vector<Widget>& wgt_wect_to_set_layer, uint8_t layer_to_set);
    void all_pages_write_to_memory(void);
    vector<Widget>& return_current_page_vect(Display_page page_name_to_parse);
    void set_all_widgets_active(vector<Widget>& vector_to_set_active, bool state_to_set);
    void init_fonts_img_vect(void);
    void release_all_buttons_in_vect(vector<Widget>& vect_to_release);
    void set_buttons_state(void);
    void init_displayed_values(void);   // инициализировать отображаемые числовые значения
    void output_time(uint16_t hours_to_out, uint16_t minutes_to_out);
    void convert_value_to_int_arr(vector<uint8_t>& value_int_vect,  uint32_t value_to_conv);            // конвертировать числовое значение в массив цифр
    void update_editable_time(void);
    void get_values_to_timer(void);
    int16_t convert_editable_time_to_int(int16_t time_digit_0, int16_t time_digit_1);
    void update_editable_temper(void);
    void update_editable_vacuum(void);
    void colon_blink(void);
    int16_t get_value_to_temper(void);
};

enum Process_stage
{
    PROCESS_STARTED,
    PROCESS_STOPPED,
    PROCESS_PAUSED
};

class TotalDevice : public TotalDisplay
{
    public:
    bool vacuum_connected_to_timer;
    bool temper_connected_to_timer;
    Process_stage current_stage;
    bool vacuum_started;
    bool temper_started;
    int16_t current_vacuum;
    int16_t previous_vacuum;
    int16_t previous_temper_chamber;
    int16_t current_hours;
    int16_t current_minutes;
    int16_t edit_vacuum_0;
    int16_t edit_vacuum_1;
    int16_t edit_temper_0;
    int16_t edit_temper_1;
    int16_t edit_temper_2;
    int16_t edit_minutes_0;
    int16_t edit_minutes_1;
    int16_t edit_hours_0;
    int16_t edit_hours_1;
    bool colon_is_active;
    int8_t seconds_counter;
    uint16_t previous_termocouple_heater;
    uint32_t time_before_start_vacuum;
    uint32_t time_before_stop_vacuum;
    uint32_t global_sec_counter;
    bool i2c_is_busy;
    int32_t tmp_bar_raw;
    int16_t required_temperature;

    TotalDevice();
    void start_process(void);
    void stop_process(void);
    void reset_timer(void);
    void pause_process(void);
    void unpause_process(void);
    void start_vacuum(void);
    void stop_vacuum(void);
    void start_temper(void);
    void stop_temper(void);
    void seconds_timer_handler(void);
    void minus_one_minute(void);
    void connect_vacuum_to_timer(void);
    void disconnect_vacuum_to_timer(void);
    void connect_temper_to_timer(void);
    void disconnect_temper_to_timer(void);
    void update_sensors_data(void);
    uint32_t calculate_seconds_before_start_vacuum(uint16_t all_process_time);
    uint16_t return_time_in_minutes(uint16_t hours_to_convert, uint16_t minutes_to_convert);
    void set_ten_heat(int16_t air_temper);
};

TotalDevice::TotalDevice(void)
{
    vacuum_connected_to_timer = true;
    temper_connected_to_timer = true;
    current_stage = PROCESS_STOPPED;
    vacuum_started = 0;
    temper_started = 0;
    current_vacuum = 0;
    previous_vacuum = 0;
    previous_temper_chamber = 0;
    current_hours = 0;
    current_minutes = 0;
    edit_vacuum_0 = 0;
    edit_vacuum_1 = 0;
    edit_temper_0 = 0;
    edit_temper_1 = 0;
    edit_temper_2 = 0;
    edit_minutes_0 = 0;
    edit_minutes_1 = 0;
    edit_hours_0 = 0;
    edit_hours_1 = 0;
    colon_is_active = true;
    seconds_counter = 0;
    previous_termocouple_heater = 0;
    time_before_start_vacuum = 0;
    time_before_stop_vacuum = 0;
    global_sec_counter = 0;
    i2c_is_busy = false;
    tmp_bar_raw = 0;
    required_temperature = 0;
}

TotalDevice total_device;

void TotalDisplay::convert_value_to_int_arr(vector<uint8_t>& value_int_vect,  uint32_t value_to_conv)
{
    uint16_t number_of_decimal_digits = 0;              // количество десячиных разрядов
    uint32_t dec_divider_buf_1 = 1;                     // десятка в степени, на неё делим
    uint32_t prev_digits_buf = 0;                       // буфер старших разрядов, его вычитаем

    if (value_to_conv > 0)                              // если наше значение больше нуля
    {
        while (dec_divider_buf_1 <= value_to_conv)      // проходимся по всем разрядам
        {
            number_of_decimal_digits++;                 // инкрементируем количество разрядов
            dec_divider_buf_1 *= 10;                    // умножаем на 10 десятку в степени
        }
        dec_divider_buf_1 /= 10;                        // делим на 10 десятку в степени (чтобы был первый разряд)
    }
    else
    {
        number_of_decimal_digits = 1;                   // если значение равно нулю, количество разрядов равно 1
    }

    value_int_vect.resize(number_of_decimal_digits);    // меняем размер вектора для нужного количества разрядов
    for (uint16_t i = number_of_decimal_digits; i > 0; i--)     // проходимся по каждому десятичному разряду
    {
        value_int_vect[i - 1] = value_to_conv/(dec_divider_buf_1) - prev_digits_buf;    // делим текущий разряд на десятку в степени, затем вычитаем более старшие разряды
        dec_divider_buf_1 /= 10;                        // уменьшаем на разряд десятку в степени
        prev_digits_buf += (value_int_vect[i - 1]);     // прибавляем текущий разряд к буферу старших разрядов
        prev_digits_buf *= 10;                          // домножаем старшие разряды на 10
    }
}

void Widget::change_value_in_wgt(Alignment numbers_align, uint8_t font_space, vector<tImage>& img_font, uint32_t value_to_displ)
{
    vector<uint8_t> value_digits_array;                                 // создаём массив разрядов
    total_device.convert_value_to_int_arr(value_digits_array, value_to_displ);       // конвертируем число в массив разрядов
    int16_t init_coord_x = 0;                                           // начальная координата строки в виджете

    while (init_coord_x <= (wgt_width - (img_font[IMG_BACKGR_IN_VECT].width)))  // пока координата строки меньше ширины виджета за вычетом ширины одного символа
    {
        change_image_in_widget(img_font[IMG_BACKGR_IN_VECT], init_coord_x, 0);  // отрисовываем фон символа
        init_coord_x += img_font[IMG_BACKGR_IN_VECT].width;                     // прибавляем к начальной координате ширину фона символа
    }
    init_coord_x = wgt_width - img_font[IMG_BACKGR_IN_VECT].width + 4;          // начальная координата равна ширине виджета за вычетом ширины одного символа + 4 пикселя (вправо)
    //change_image_in_widget(img_font[IMG_BACKGR_IN_VECT], init_coord_x, 0);      // отрисовываем крайний правый фон символа
    change_image_in_widget(img_font[IMG_BACKGR_IN_VECT], 0, 0);      // отрисовываем крайний правый фон символа

    switch (numbers_align)
    {
    case ALIGN_LEFT:                // если выравнивание по левой стороне
    {
        init_coord_x = 0;           // начинаем с начала виджета
        break;
    }
    case ALIGN_RIGHT:               // если выравнивание по правой стороне
    {
        init_coord_x = string_align_right_x(img_font, font_space, value_digits_array);      // определяем координату строки в виджете
        break;
    }
    }
    uint16_t img_string_size_buf = 0;                                                       // буфер размера строки
    for (vector<uint8_t>::size_type i = value_digits_array.size(); i > 0; i--)              // проходимся по массиву разрядов
    {
        if (i <= MAX_NMBR_IN_STRING)
        {
            change_image_in_widget(img_font[value_digits_array[i - 1]], (init_coord_x + img_string_size_buf), 0);   // отрисовываем разряд
            img_string_size_buf += (img_font[value_digits_array[i - 1]].width + font_space);
        }
    }
}




TotalDisplay::TotalDisplay(void)
{
	display_initialized  = 0;
    draw_all_completed = 0;

    display_width = DISPLAY_WIDTH;
    display_height = DISPLAY_HEIGHT;
    bounce_sample_counter = 0;
    bounce_btn_buff = NO_BUTTON;
    previous_button = NO_BUTTON;            // предыдущая нажатая кнопка

    init_fonts_img_vect();
    init_widgets_content();

    current_page_name = START_PAGE;
}

void TotalDisplay::init_fonts_img_vect(void)
{
    numbers_33_font_vector.push_back(numbers_33_0);
    numbers_33_font_vector.push_back(numbers_33_1);
    numbers_33_font_vector.push_back(numbers_33_2);
    numbers_33_font_vector.push_back(numbers_33_3);
    numbers_33_font_vector.push_back(numbers_33_4);
    numbers_33_font_vector.push_back(numbers_33_5);
    numbers_33_font_vector.push_back(numbers_33_6);
    numbers_33_font_vector.push_back(numbers_33_7);
    numbers_33_font_vector.push_back(numbers_33_8);
    numbers_33_font_vector.push_back(numbers_33_9);
    numbers_33_font_vector.push_back(numbers_33_backgr);
}

void TotalDisplay::init_widgets_content(void)
{
    Widget background_frame_0 = {NO_BUTTON};
    Widget vacuum_bar = {NO_BUTTON, 55, 55};
    Widget temper_bar = {NO_BUTTON, 251, 55};
    Widget time_bar = {NO_BUTTON, 447, 55};
    Widget vacuum_edit_number_0 = {NO_BUTTON, 143, EDIT_BACKGR_Y};
    Widget vacuum_edit_number_1 = {NO_BUTTON, 82, EDIT_BACKGR_Y};  // !!! уточнить координаты "на глазок"
    Widget temper_edit_number_0 = {NO_BUTTON, 372, EDIT_BACKGR_Y};
    Widget temper_edit_number_1 = {NO_BUTTON, 311, EDIT_BACKGR_Y};
    Widget temper_edit_number_2 = {NO_BUTTON, 251, EDIT_BACKGR_Y};
    Widget time_edit_number_0 = {NO_BUTTON, 583, EDIT_BACKGR_Y};
    Widget time_edit_number_1 = {NO_BUTTON, 538, EDIT_BACKGR_Y};
    Widget time_edit_number_2 = {NO_BUTTON, 492, EDIT_BACKGR_Y};
    Widget time_edit_number_3 = {NO_BUTTON, 447, EDIT_BACKGR_Y};
    Widget pointing_line = {NO_BUTTON, 177, 432};
    Widget btn_vacuum_start_stop = {VACUUM_START_STOP, 55, 349};
    Widget btn_vacuum_connect_to_timer = {NO_BUTTON, 146, 349};
    Widget btn_vacuum_increase_0 = {NO_BUTTON, 155 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_UP_COORD_Y};
    Widget btn_vacuum_decrease_0 = {NO_BUTTON, 155 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_DOWN_COORD_Y};
    Widget btn_temper_start_stop = {TEMPER_START_STOP, 251, 349};
    Widget btn_temper_connect_to_timer = {NO_BUTTON, 342, 349};
    Widget btn_temper_increase_0 = {TEMPER_INCREASE_0, 385 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_UP_COORD_Y};
    Widget btn_temper_decrease_0 = {TEMPER_DECREASE_0, 385 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_DOWN_COORD_Y};
    Widget btn_temper_increase_1 = {TEMPER_INCREASE_1, 324 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_UP_COORD_Y};
    Widget btn_temper_decrease_1 = {TEMPER_DECREASE_1, 324 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_DOWN_COORD_Y};
    Widget btn_temper_increase_2 = {TEMPER_INCREASE_2, 264 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_UP_COORD_Y};
    Widget btn_temper_decrease_2 = {TEMPER_DECREASE_2, 264 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_DOWN_COORD_Y};
    Widget btn_timer_start = {TIMER_START, 645, 55};
    Widget btn_timer_stop = {TIMER_STOP, 538, 349};
    Widget btn_timer_pause = {TIMER_PAUSE, 447, 349};
    Widget btn_timer_increase_0 = {TIMER_INCREASE_0, 589 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_UP_COORD_Y};
    Widget btn_timer_decrease_0 = {TIMER_DECREASE_0, 589 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_DOWN_COORD_Y};
    Widget btn_timer_increase_1 = {TIMER_INCREASE_1, 544 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_UP_COORD_Y};
    Widget btn_timer_decrease_1 = {TIMER_DECREASE_1, 544 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_DOWN_COORD_Y};
    Widget btn_timer_increase_2 = {TIMER_INCREASE_2, 498 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_UP_COORD_Y};
    Widget btn_timer_decrease_2 = {TIMER_DECREASE_2, 498 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_DOWN_COORD_Y};
    Widget btn_timer_increase_3 = {TIMER_INCREASE_3, 453 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_UP_COORD_Y};
    Widget btn_timer_decrease_3 = {TIMER_DECREASE_3, 453 - TRIANGLE_HIDDEN_BTN_AREA, TRIANGLE_DOWN_COORD_Y};
    //Widget out_value_vacuum = {NO_BUTTON, 132, VALUES_NMBRS_COORD_Y, (FONT_33_NMBR_WIDTH * 1) + (FONT_33_GAP_PIX * 1), FONT_33_NMBR_HEIGHT};
    Widget out_value_vacuum = {NO_BUTTON, 88, VALUES_NMBRS_COORD_Y, \
                                (FONT_33_NMBR_WIDTH * 3) + (FONT_33_GAP_PIX * 3), FONT_33_NMBR_HEIGHT};
    Widget out_vacuum_integer = {NO_BUTTON, 88, VALUES_NMBRS_COORD_Y};
    Widget out_value_temper_chamber = {NO_BUTTON, 269, VALUES_NMBRS_COORD_Y, \
                                        (FONT_33_NMBR_WIDTH * 3) + (FONT_33_GAP_PIX * 3), FONT_33_NMBR_HEIGHT};
    Widget time_figure_0 = {NO_BUTTON, 568, VALUES_NMBRS_COORD_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget time_figure_1 = {NO_BUTTON, 540, VALUES_NMBRS_COORD_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget time_figure_2 = {NO_BUTTON, 499, VALUES_NMBRS_COORD_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget time_figure_3 = {NO_BUTTON, 471, VALUES_NMBRS_COORD_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget time_colon = {NO_BUTTON, 527, 128, 8, 25};
    Widget edit_colon = {NO_BUTTON, 527, 255};
    Widget edti_nmbr_vacuum_0 = {NO_BUTTON, 143 + EDIT_NMBR_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget edti_nmbr_vacuum_1 = {NO_BUTTON, 82 + EDIT_NMBR_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget edti_nmbr_temper_0 = {NO_BUTTON, 372 + EDIT_NMBR_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget edti_nmbr_temper_1 = {NO_BUTTON, 311 + EDIT_NMBR_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget edti_nmbr_temper_2 = {NO_BUTTON, 251 + EDIT_NMBR_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget edti_nmbr_timer_0 = {NO_BUTTON, 583 + EDIT_NMBR_X - EDIT_TIME_OFFSET_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget edti_nmbr_timer_1 = {NO_BUTTON, 538 + EDIT_NMBR_X - EDIT_TIME_OFFSET_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget edti_nmbr_timer_2 = {NO_BUTTON, 492 + EDIT_NMBR_X - EDIT_TIME_OFFSET_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget edti_nmbr_timer_3 = {NO_BUTTON, 447 + EDIT_NMBR_X - EDIT_TIME_OFFSET_X, EDIT_BACKGR_Y + EDIT_NMBR_Y, FONT_33_NMBR_WIDTH, FONT_33_NMBR_HEIGHT};
    Widget out_value_temper_heater = {NO_BUTTON, 240, 10, (FONT_33_NMBR_WIDTH * 3) + (FONT_33_GAP_PIX * 3), FONT_33_NMBR_HEIGHT};

    background_frame_0.tile_area(img_tile_background, display_width, display_height);
    background_frame_0.set_rectangle_page_0();
    vacuum_bar.add_img_to_wgt(CONSTANT_IMG, img_bar_backgr);
    vacuum_bar.add_img_to_wgt(CONSTANT_IMG, img_title_vac, 56, 15);
    vacuum_bar.add_img_to_wgt(CHANGEABLE_IMG, img_pressure_sensor_icon_crossed, 54, 52);
    //vacuum_bar.add_img_to_wgt(CONSTANT_IMG, img_title_atm, 110, 90);
    temper_bar.add_img_to_wgt(CONSTANT_IMG, img_bar_backgr);
    temper_bar.add_img_to_wgt(CONSTANT_IMG, img_title_temper, 37, 15);
    temper_bar.add_img_to_wgt(CONSTANT_IMG, img_title_deg_celsius, 106, 68);
    time_bar.add_img_to_wgt(CONSTANT_IMG, img_bar_backgr);
    time_bar.add_img_to_wgt(CONSTANT_IMG, img_title_time, 36, 3);
    //time_bar.add_img_to_wgt(CHANGEABLE_IMG, img_colon, 80, 76);
    time_colon.add_img_to_wgt(CHANGEABLE_IMG, img_colon);
    edit_colon.add_img_to_wgt(CHANGEABLE_IMG, img_colon);
    //vacuum_edit_number_0.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_vac_temper);
    //vacuum_edit_number_1.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_vac_temper);
    //vacuum_edit_number_1.add_img_to_wgt(CONSTANT_IMG, img_33_dot, 42, 48);
    temper_edit_number_0.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_vac_temper);
    temper_edit_number_1.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_vac_temper);
    temper_edit_number_2.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_vac_temper);
    time_edit_number_0.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_time);
    time_edit_number_0.add_img_to_wgt(CONSTANT_IMG, img_lttr_minute, MINUTE_LTTR_X, MINUTE_LTTR_Y);
    time_edit_number_1.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_time);
    time_edit_number_1.add_img_to_wgt(CONSTANT_IMG, img_lttr_minute, MINUTE_LTTR_X, MINUTE_LTTR_Y);
    time_edit_number_2.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_time);
    time_edit_number_2.add_img_to_wgt(CONSTANT_IMG, img_lttr_hour, HOUR_LTTR_X, HOUR_LTTR_Y);
    time_edit_number_3.add_img_to_wgt(CONSTANT_IMG, img_backgr_nmbr_time);
    time_edit_number_3.add_img_to_wgt(CONSTANT_IMG, img_lttr_hour, HOUR_LTTR_X, HOUR_LTTR_Y);
    //pointing_line.add_img_to_wgt(CONSTANT_IMG, img_pointing_line);
    btn_vacuum_start_stop.add_img_to_wgt(CHANGEABLE_IMG, img_btn_vac_temper_start_rlsd);
    btn_vacuum_start_stop.add_img_to_wgt(BTN_BLOCKED_IMG, img_btn_vac_temper_start_blocked);
    //btn_vacuum_connect_to_timer.add_img_to_wgt(CHANGEABLE_IMG, img_btn_vac_temper_cnct_rlsd);
    //btn_vacuum_increase_0.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_up);
    //btn_vacuum_decrease_0.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_down);
    btn_temper_start_stop.add_img_to_wgt(CHANGEABLE_IMG, img_btn_vac_temper_start_rlsd);
    btn_temper_start_stop.add_img_to_wgt(BTN_BLOCKED_IMG, img_btn_vac_temper_start_blocked);
    //btn_temper_connect_to_timer.add_img_to_wgt(CHANGEABLE_IMG, img_btn_vac_temper_cnct_rlsd);
    btn_temper_increase_0.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_up);
    btn_temper_decrease_0.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_down);
    btn_temper_increase_1.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_up);
    btn_temper_decrease_1.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_down);
    btn_temper_increase_2.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_up);
    btn_temper_decrease_2.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_down);
    btn_timer_start.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_timer_start_rlsd);
    btn_timer_start.add_img_to_wgt(BTN_BLOCKED_IMG, img_btn_timer_start_blocked);
    btn_timer_stop.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_timer_stop_rlsd);
    btn_timer_stop.add_img_to_wgt(BTN_BLOCKED_IMG, img_btn_timer_stop_blocked);
    btn_timer_pause.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_timer_pause_rlsd);
    btn_timer_pause.add_img_to_wgt(BTN_BLOCKED_IMG, img_btn_timer_pause_blocked);
    btn_timer_increase_0.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_up);
    btn_timer_decrease_0.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_down);
    btn_timer_increase_1.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_up);
    btn_timer_decrease_1.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_down);
    btn_timer_increase_2.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_up);
    btn_timer_decrease_2.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_down);
    btn_timer_increase_3.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_up);
    btn_timer_decrease_3.add_img_to_wgt(BTN_RELEASED_IMG, img_btn_triangle_down);
    //out_vacuum_integer.add_img_to_wgt(CONSTANT_IMG, numbers_33_0);
    //out_vacuum_integer.add_img_to_wgt(CONSTANT_IMG, img_33_dot, 29, 25);

    start_page.push_back(background_frame_0);               // 0
    start_page.push_back(vacuum_bar);                       // 1
    start_page.push_back(temper_bar);                       // 2
    start_page.push_back(time_bar);                         // 3
    start_page.push_back(vacuum_edit_number_0);             // 4
    start_page.push_back(vacuum_edit_number_1);             // 5
    start_page.push_back(temper_edit_number_0);             // 6
    start_page.push_back(temper_edit_number_1);             // 7
    start_page.push_back(temper_edit_number_2);             // 8
    start_page.push_back(time_edit_number_0);               // 9
    start_page.push_back(time_edit_number_1);               // 10
    start_page.push_back(time_edit_number_2);               // 11
    start_page.push_back(time_edit_number_3);               // 12
    start_page.push_back(pointing_line);                    // 13
    start_page.push_back(btn_vacuum_start_stop);            // 14
    start_page.push_back(btn_vacuum_connect_to_timer);      // 15
    start_page.push_back(btn_temper_increase_0);            // 16
    start_page.push_back(btn_temper_decrease_0);            // 17
    start_page.push_back(btn_temper_increase_1);            // 18
    start_page.push_back(btn_temper_decrease_1);            // 19
    start_page.push_back(btn_temper_increase_2);            // 20
    start_page.push_back(btn_temper_decrease_2);            // 21
    start_page.push_back(btn_timer_start);                  // 22
    start_page.push_back(btn_timer_stop);                   // 23
    start_page.push_back(btn_timer_pause);                  // 24
    start_page.push_back(btn_timer_increase_0);             // 25
    start_page.push_back(btn_timer_decrease_0);             // 26
    start_page.push_back(btn_timer_increase_1);             // 27
    start_page.push_back(btn_timer_decrease_1);             // 28
    start_page.push_back(btn_timer_increase_2);             // 29
    start_page.push_back(btn_timer_decrease_2);             // 30
    start_page.push_back(btn_timer_increase_3);             // 31
    start_page.push_back(btn_timer_decrease_3);             // 32
    start_page.push_back(btn_vacuum_increase_0);            // 33
    start_page.push_back(btn_vacuum_decrease_0);            // 34
    start_page.push_back(btn_temper_start_stop);            // 35
    start_page.push_back(btn_temper_connect_to_timer);      // 36
    start_page.push_back(out_value_vacuum);                 // 37
    start_page.push_back(out_value_temper_chamber);         // 38
    start_page.push_back(time_figure_0);                    // 39
    start_page.push_back(time_figure_1);                    // 40
    start_page.push_back(time_figure_2);                    // 41
    start_page.push_back(time_figure_3);                    // 42
    start_page.push_back(time_colon);                       // 43
    start_page.push_back(edti_nmbr_vacuum_0);               // 44
    start_page.push_back(edti_nmbr_vacuum_1);               // 45
    start_page.push_back(edti_nmbr_temper_0);               // 46
    start_page.push_back(edti_nmbr_temper_1);               // 47
    start_page.push_back(edti_nmbr_temper_2);               // 48
    start_page.push_back(edti_nmbr_timer_0);                // 49
    start_page.push_back(edti_nmbr_timer_1);                // 50
    start_page.push_back(edti_nmbr_timer_2);                // 51
    start_page.push_back(edti_nmbr_timer_3);                // 52
    start_page.push_back(edit_colon);                       // 53
    start_page.push_back(out_vacuum_integer);               // 54
    if (SHOW_TEN_TEMPER)
    {
        start_page.push_back(out_value_temper_heater);       // 55
    }

    set_all_widgets_active(start_page, true);
    set_layer_for_widgets(start_page, LAYER_PAGE_0);
}


// если размер виджета не задан вручную, автоматически берём размер первого изображения в виджете
void TotalDisplay::init_widgets_size(vector<Widget>& wgt_vect_to_estimate_size)
{
    for (vector<Widget>::size_type i = 0; i != wgt_vect_to_estimate_size.size(); i++)                  // проходимся по вектору виджетов
    {
        if ((wgt_vect_to_estimate_size[i].wgt_width == 0) && (wgt_vect_to_estimate_size[i].wgt_height == 0))      // если размер виджета равен нулю
        {
            if (wgt_vect_to_estimate_size[i].released_btn_images.size() != 0)                          // если виджет является кнопкой
            {
                wgt_vect_to_estimate_size[i].wgt_width = wgt_vect_to_estimate_size[i].released_btn_images[0].image_struct.width;      // берём размеры изображения кнопки
                wgt_vect_to_estimate_size[i].wgt_height = wgt_vect_to_estimate_size[i].released_btn_images[0].image_struct.height;
            }
            else if (wgt_vect_to_estimate_size[i].constant_images.size() != 0)                         // если виджет не является кнопкой
            {
                wgt_vect_to_estimate_size[i].wgt_width = wgt_vect_to_estimate_size[i].constant_images[0].image_struct.width;      // берём размеры первого константного изображения
                wgt_vect_to_estimate_size[i].wgt_height = wgt_vect_to_estimate_size[i].constant_images[0].image_struct.height;
            }
            else if (wgt_vect_to_estimate_size[i].changeable_images.size() != 0)                         // если виджет не является кнопкой
            {
                wgt_vect_to_estimate_size[i].wgt_width = wgt_vect_to_estimate_size[i].changeable_images[0].image_struct.width;      // берём размеры первого константного изображения
                wgt_vect_to_estimate_size[i].wgt_height = wgt_vect_to_estimate_size[i].changeable_images[0].image_struct.height;
            }
        }
    }
}


// начальная отрисовка всех виджетов
void TotalDisplay::draw_all_widgets(vector<Widget>& widget_vector_to_draw)
{
    for(vector<Widget>::size_type i = 0; i != widget_vector_to_draw.size(); i++)       // проходимся по вектору виджетов
    {
        // отрисовываем константные изображения
        widget_vector_to_draw[i].draw_img_vector(widget_vector_to_draw[i].constant_images, widget_vector_to_draw[i].wgt_coord_x, widget_vector_to_draw[i].wgt_coord_y);
        widget_vector_to_draw[i].draw_img_vector(widget_vector_to_draw[i].changeable_images, widget_vector_to_draw[i].wgt_coord_x, widget_vector_to_draw[i].wgt_coord_y);
        if(widget_vector_to_draw[i].button_name != NO_BUTTON)          // отрисовывам кнопки в ненажатом состоянии
        {
            widget_vector_to_draw[i].draw_img_vector(widget_vector_to_draw[i].released_btn_images, widget_vector_to_draw[i].wgt_coord_x, widget_vector_to_draw[i].wgt_coord_y);
        }
    }
}

void TotalDisplay::set_layer_for_widgets(vector<Widget>& wgt_wect_to_set_layer, uint8_t layer_to_set)
{
    for (vector<Widget>::size_type i = 0; i != wgt_wect_to_set_layer.size(); i++)
    {
        wgt_wect_to_set_layer[i].current_layer = layer_to_set;
    }
}

void TotalDisplay::set_all_widgets_active(vector<Widget>& vector_to_set_active, bool state_to_set)
{
    for (vector<Widget>::size_type i = 0; i != vector_to_set_active.size(); i++)
    {
        vector_to_set_active[i].widget_btn_is_active = state_to_set;
    }
}

void TotalDisplay::set_buttons_state(void)
{
    start_page[22].unlock_button();
    start_page[23].lock_button();
    start_page[24].lock_button();
    //total_device.connect_vacuum_to_timer();
    //total_device.connect_temper_to_timer();
}

void TotalDisplay::init_displayed_values(void)
{
    //start_page[37].change_value_in_wgt(ALIGN_RIGHT, FONT_33_GAP_PIX, numbers_33_font_vector, total_device.current_vacuum);
    start_page[38].change_value_in_wgt(ALIGN_RIGHT, FONT_33_GAP_PIX, numbers_33_font_vector, 0);//Temperature::OVEN_CHAMBER.celsius);

    if (SHOW_TEN_TEMPER)
    {
        start_page[55].change_value_in_wgt(ALIGN_RIGHT, FONT_33_GAP_PIX, numbers_33_font_vector, 0);//Temperature::OVEN_HEATER.celsius);
    }

    output_time(total_device.current_hours, total_device.current_minutes);

    // update_editable_vacuum();
    update_editable_temper();
    update_editable_time();
}

void TotalDisplay::all_pages_write_to_memory(void)
{
    init_widgets_size(start_page);
    draw_all_widgets(start_page);
    set_buttons_state();
    init_displayed_values();
    total_device.stop_temper();
}

bool colon_is_active;

void TotalDisplay::colon_blink(void)
{
    if (colon_is_active)
    {
        start_page[43].change_image_in_widget(img_no_colon);
        colon_is_active = false;
        //total_device.colon_is_active = false;

    }
    else
    {
        start_page[43].change_image_in_widget(img_colon);
        colon_is_active = true;
        //total_device.colon_is_active = true;

    }
}

// определяем нажатую кнопку
Buttons_list TotalDisplay::identify_pressed_btn(uint16_t pressing_coord_x, uint16_t pressing_coord_y, vector<Widget> current_page)
{
    pressing_coord_x += CALIBRATION_OFFSET_X;       // добавляем калибровочное смещение, ибо тачскрин и дисплей могут не совпадать
    pressing_coord_y += CALIBRATION_OFFSET_Y;

    for (vector<Widget>::size_type i = 0; i != current_page.size(); i++)          // проходимся по вектору виджетов
    {
        if (current_page[i].button_name != NO_BUTTON)                             // если данный вектор являетс кнопкой
        {
            if (current_page[i].widget_btn_is_active)
            {
                if ((WITHIN(pressing_coord_x, current_page[i].wgt_coord_x, \
                (current_page[i].wgt_coord_x + current_page[i].wgt_width))) && \
                    (WITHIN(pressing_coord_y, current_page[i].wgt_coord_y, \
                    (current_page[i].wgt_coord_y + current_page[i].wgt_height))))      // если текущие координаты нажатия находятся внутри кнопки
                {
                    return current_page[i].button_name;                             // возвращаем номер кнопки (определён через enum)
                }
            }
        }
    }

    return NO_BUTTON;
}

vector<Widget>& TotalDisplay::return_current_page_vect(Display_page page_name_to_parse)
{
    switch (page_name_to_parse)
    {
        case START_PAGE:
        {
            return start_page;
        }
        default:
        {
        	return start_page;
        }
    }
}

int16_t TotalDisplay::convert_editable_time_to_int(int16_t time_digit_0, int16_t time_digit_1)
{
    int16_t time_to_return = time_digit_0;
    time_to_return += time_digit_1*10;
    return time_to_return;
}

void TotalDisplay::get_values_to_timer(void)
{
    total_device.current_hours = convert_editable_time_to_int(total_device.edit_hours_0, total_device.edit_hours_1);
    total_device.current_minutes = convert_editable_time_to_int(total_device.edit_minutes_0, total_device.edit_minutes_1);
    output_time(total_device.current_hours, total_device.current_minutes);
}

uint16_t TotalDevice::return_time_in_minutes(uint16_t hours_to_convert, uint16_t minutes_to_convert)
{
    uint16_t time_to_return_min = minutes_to_convert;
    time_to_return_min += (hours_to_convert * 60);
    return time_to_return_min;
}

void TotalDevice::start_process(void)
{
    total_device.get_values_to_timer();
    if ((current_hours == 0) && (current_minutes == 0))
    {
        return;
    }
    else
    {
        total_device.start_page[22].lock_button();
        total_device.start_page[23].unlock_button();
        total_device.start_page[24].unlock_button();
        if (temper_connected_to_timer)
        {
            start_temper();
        }
        global_sec_counter = return_time_in_minutes(current_hours, current_minutes)*60;
        time_before_start_vacuum = (global_sec_counter * TIME_PERSENT_TO_VACUUM)/100;
        // time_before_start_vacuum = calculate_seconds_before_start_vacuum(return_time_in_minutes(current_hours, current_minutes));
        time_before_stop_vacuum = (global_sec_counter * TIME_PERCENT_TO_HEAT)/100;
        total_device.start_page[14].lock_button();
        total_device.start_page[35].lock_button();
        current_stage = PROCESS_STARTED;
    }
}

void TotalDisplay::output_time(uint16_t hours_to_out, uint16_t minutes_to_out)
{
    vector<uint8_t> digits_array_hours;                                 // создаём массив разрядов
    convert_value_to_int_arr(digits_array_hours, hours_to_out);         // конвертируем число в массив разрядов
    if (digits_array_hours.size() == 1)
    {
        digits_array_hours.push_back(0);
    }
    vector<uint8_t> digits_array_minutes;
    convert_value_to_int_arr(digits_array_minutes, minutes_to_out);
    if (digits_array_minutes.size() == 1)
    {
        digits_array_minutes.push_back(0);
    }
    start_page[39].change_value_in_wgt(ALIGN_LEFT, FONT_33_GAP_PIX, numbers_33_font_vector, digits_array_minutes[0]);
    start_page[40].change_value_in_wgt(ALIGN_LEFT, FONT_33_GAP_PIX, numbers_33_font_vector, digits_array_minutes[1]);
    start_page[41].change_value_in_wgt(ALIGN_LEFT, FONT_33_GAP_PIX, numbers_33_font_vector, digits_array_hours[0]);
    start_page[42].change_value_in_wgt(ALIGN_LEFT, FONT_33_GAP_PIX, numbers_33_font_vector, digits_array_hours[1]);
}


void TotalDevice::reset_timer(void)
{
    current_hours = 0;
    current_minutes = 0;
    seconds_counter = 0;
    total_device.output_time(total_device.current_hours, total_device.current_minutes);
}


void TotalDevice::stop_process(void)
{
    total_device.start_page[43].change_image_in_widget(img_colon);
    total_device.colon_is_active = true;
    reset_timer();
    total_device.start_page[22].unlock_button();
    total_device.start_page[23].lock_button();
    total_device.start_page[24].lock_button();
    if (vacuum_connected_to_timer)
    {
        stop_vacuum();
    }
    if (temper_connected_to_timer)
    {
        stop_temper();
    }
    global_sec_counter = 0;
    total_device.start_page[14].unlock_button();
    total_device.start_page[35].unlock_button();
    current_stage = PROCESS_STOPPED;
}


void TotalDevice::pause_process(void)
{
	total_device.start_page[24].change_image_in_widget(img_btn_timer_pause_prsd);
    current_stage = PROCESS_PAUSED;
}

void TotalDevice::unpause_process(void)
{
	total_device.start_page[24].change_image_in_widget(img_btn_timer_pause_rlsd);
    current_stage = PROCESS_STARTED;
}

void TotalDevice::start_vacuum(void)
{
    //WRITE(PB8, HIGH);
    if (total_device.start_page[14].btn_locked)
    {
        total_device.start_page[14].change_image_in_widget(img_btn_timer_stop_blocked);
    }
    else
    {
        total_device.start_page[14].change_image_in_widget(img_btn_timer_stop_prsd);
    }
    total_device.start_page[1].change_image_in_widget(img_pressure_sensor_icon_enabled, 54, 52);
    vacuum_started = true;
}

void TotalDevice::stop_vacuum(void)
{
    //WRITE(PB8, LOW);
    if (total_device.start_page[14].btn_locked)
    {
       total_device.start_page[14].change_image_in_widget(img_btn_vac_temper_start_blocked);
    }
    else
    {
        total_device.start_page[14].change_image_in_widget(img_btn_vac_temper_start_rlsd);
    }
    total_device.start_page[1].change_image_in_widget(img_pressure_sensor_icon_crossed, 54, 52);
    vacuum_started = false;
}

int16_t TotalDisplay::get_value_to_temper(void)
{
    return total_device.edit_temper_0 + total_device.edit_temper_1*10 + total_device.edit_temper_2*100;
}

void TotalDevice::set_ten_heat(int16_t air_temper)
{
    if (air_temper <= TEN_ADDITION_TRESHOLD)
    {
        //Temperature::OVEN_HEATER.target = air_temper + TEN_TEMPERATURE_ADDITION_0;
    }
    else
    {
        //Temperature::OVEN_HEATER.target = air_temper + TEN_TEMPERATURE_ADDITION_1;
    }
}

void TotalDevice::start_temper(void)
{
    total_device.required_temperature = total_device.get_value_to_temper();

    set_ten_heat(total_device.required_temperature);

    if (total_device.start_page[35].btn_locked)
    {
        total_device.start_page[35].change_image_in_widget(img_btn_timer_stop_blocked);
    }
    else
    {
        total_device.start_page[35].change_image_in_widget(img_btn_timer_stop_prsd);
    }
    temper_started = true;
}

void TotalDevice::stop_temper(void)
{
    //Temperature::OVEN_HEATER.target = NO_HEAT_DUMMY_VALUE;                                      // требуем минимальную температуру
    if (total_device.start_page[35].btn_locked)
    {
       total_device.start_page[35].change_image_in_widget(img_btn_vac_temper_start_blocked);
    }
    else
    {
        total_device.start_page[35].change_image_in_widget(img_btn_vac_temper_start_rlsd);
    }

    temper_started = false;
}

void TotalDisplay::update_editable_time(void)
{
    start_page[49].change_value_in_wgt(ALIGN_LEFT, 0, numbers_33_font_vector, total_device.edit_minutes_0);
    start_page[50].change_value_in_wgt(ALIGN_LEFT, 0, numbers_33_font_vector, total_device.edit_minutes_1);
    start_page[51].change_value_in_wgt(ALIGN_LEFT, 0, numbers_33_font_vector, total_device.edit_hours_0);
    start_page[52].change_value_in_wgt(ALIGN_LEFT, 0, numbers_33_font_vector, total_device.edit_hours_1);
}

void TotalDisplay::update_editable_temper(void)
{
    if (total_device.edit_temper_2 == MAX_HUNDRED_CELSIUS - 1)
    {
        total_device.edit_temper_0 = 0;
        total_device.edit_temper_1 = 0;
    }
    if (total_device.edit_temper_2 > MAX_HUNDRED_CELSIUS - 1)
    {
        total_device.edit_temper_0 = 0;
        total_device.edit_temper_1 = 0;
        total_device.edit_temper_2 = MAX_HUNDRED_CELSIUS - 1;
    }
    start_page[46].change_value_in_wgt(ALIGN_RIGHT, FONT_33_GAP_PIX, numbers_33_font_vector, total_device.edit_temper_0);
    start_page[47].change_value_in_wgt(ALIGN_RIGHT, FONT_33_GAP_PIX, numbers_33_font_vector, total_device.edit_temper_1);
    start_page[48].change_value_in_wgt(ALIGN_RIGHT, FONT_33_GAP_PIX, numbers_33_font_vector, total_device.edit_temper_2);

    if (total_device.temper_started)
    {
        total_device.required_temperature = total_device.get_value_to_temper();
    }
}

void TotalDisplay::update_editable_vacuum(void)
{
    start_page[44].change_value_in_wgt(ALIGN_RIGHT, FONT_33_GAP_PIX, numbers_33_font_vector, total_device.edit_vacuum_0);
    start_page[45].change_value_in_wgt(ALIGN_RIGHT, FONT_33_GAP_PIX, numbers_33_font_vector, total_device.edit_vacuum_1);
}


void TotalDisplay::handle_button_press(Buttons_list pressed_button, vector<Widget>& pressed_page, Display_page current_page_name_to_handle)
{
    for (vector<Widget>::size_type i = 0; i != pressed_page.size(); i++)                          // проходимся по вектору виджетов
    {
        if ((pressed_button == pressed_page[i].button_name) && (pressed_page[i].btn_locked))    // если данная кнопка заблокирована - выходим из функции
        {
            return;
        }
    }


    switch (current_page_name_to_handle)
    {
        case START_PAGE:
        {
            switch (pressed_button)
            {
                case TIMER_START:
                {
                    total_device.start_process();
                    break;
                }
                case TIMER_STOP:
                {
                    total_device.stop_process();
                    break;
                }
                case TIMER_PAUSE:
                {
                    if (total_device.current_stage == PROCESS_STARTED)
                    {
                        total_device.pause_process();
                    }
                    else if (total_device.current_stage == PROCESS_PAUSED)
                    {
                        total_device.unpause_process();
                    }
                    break;
                }
                case VACUUM_START_STOP:
                {
                    if (total_device.vacuum_started)
                    {
                        total_device.stop_vacuum();
                    }
                    else
                    {
                        total_device.start_vacuum();
                    }
                    break;
                }
                case VACUUM_CONNECT_TO_TMER:
                {
                    if (total_device.vacuum_connected_to_timer)
                    {
                        //total_device.disconnect_vacuum_to_timer();

                    }
                    else
                    {
                        //total_device.connect_vacuum_to_timer();
                    }
                    break;
                }
                case TEMPER_START_STOP:
                {
                    if (total_device.temper_started)
                    {
                        total_device.stop_temper();
                    }
                    else
                    {
                        total_device.start_temper();
                    }
                    break;
                }
                case TEMPER_CONNECT_TO_TIMER:
                {
                    if (total_device.temper_connected_to_timer)
                    {
                        //total_device.disconnect_temper_to_timer();
                    }
                    else
                    {
                        //total_device.connect_temper_to_timer();
                    }
                    break;
                }
                case TIMER_INCREASE_0:
                {
                    total_device.edit_minutes_0++;
                    if (total_device.edit_minutes_0 == 10)
                    {
                        total_device.edit_minutes_0 = 0;
                        total_device.edit_minutes_1++;
                        if (total_device.edit_minutes_1 == 6)
                        {
                            total_device.edit_minutes_1 = 0;
                            total_device.edit_hours_0++;
                            if (total_device.edit_hours_0 == 10)
                            {
                                total_device.edit_hours_0 = 0;
                                total_device.edit_hours_1++;
                                if (total_device.edit_hours_1 == 10)
                                {
                                    total_device.edit_hours_1 = 0;
                                }
                            }
                        }
                    }
                    update_editable_time();
                    break;
                }
                case TIMER_INCREASE_1:
                {
                    total_device.edit_minutes_1++;
                    if (total_device.edit_minutes_1 == 6)
                    {
                        total_device.edit_minutes_1 = 0;
                        total_device.edit_hours_0++;
                        if (total_device.edit_hours_0 == 10)
                        {
                            total_device.edit_hours_0 = 0;
                            total_device.edit_hours_1++;
                            if (total_device.edit_hours_1 == 10)
                            {
                                total_device.edit_hours_1 = 0;
                            }
                        }
                    }
                    update_editable_time();
                    break;
                }
                case TIMER_INCREASE_2:
                {
                    total_device.edit_hours_0++;
                    if (total_device.edit_hours_0 == 10)
                    {
                        total_device.edit_hours_0 = 0;
                        total_device.edit_hours_1++;
                        if (total_device.edit_hours_1 == 10)
                        {
                            total_device.edit_hours_1 = 0;
                        }
                    }
                    update_editable_time();
                    break;
                }
                case TIMER_INCREASE_3:
                {
                    total_device.edit_hours_1++;
                    if (total_device.edit_hours_1 == 10)
                    {
                        total_device.edit_hours_1 = 0;
                    }
                    update_editable_time();
                    break;
                }
                case TIMER_DECREASE_0:
                {
                    total_device.edit_minutes_0--;
                    if (total_device.edit_minutes_0 < 0)
                    {
                        total_device.edit_minutes_0 = 9;
                        total_device.edit_minutes_1--;
                        if (total_device.edit_minutes_1 < 0)
                        {
                            total_device.edit_minutes_1 = 5;
                            total_device.edit_hours_0--;
                            if (total_device.edit_hours_0 < 0)
                            {
                                total_device.edit_hours_0 = 9;
                                total_device.edit_hours_1--;
                                if (total_device.edit_hours_1 < 0)
                                {
                                    total_device.edit_hours_1 = 9;
                                }
                            }
                        }
                    }
                    update_editable_time();
                    break;
                }
                case TIMER_DECREASE_1:
                {
                    total_device.edit_minutes_1--;
                    if (total_device.edit_minutes_1 < 0)
                    {
                        total_device.edit_minutes_1 = 5;
                        total_device.edit_hours_0--;
                        if (total_device.edit_hours_0 < 0)
                        {
                            total_device.edit_hours_0 = 9;
                            total_device.edit_hours_1--;
                            if (total_device.edit_hours_1 < 0)
                            {
                                total_device.edit_hours_1 = 9;
                            }
                        }
                    }
                    update_editable_time();
                    break;
                }
                case TIMER_DECREASE_2:
                {
                    total_device.edit_hours_0--;
                    if (total_device.edit_hours_0 < 0)
                    {
                        total_device.edit_hours_0 = 9;
                        total_device.edit_hours_1--;
                        if (total_device.edit_hours_1 < 0)
                        {
                            total_device.edit_hours_1 = 9;
                        }
                    }
                    update_editable_time();
                    break;
                }
                case TIMER_DECREASE_3:
                {
                    total_device.edit_hours_1--;
                    if (total_device.edit_hours_1 < 0)
                    {
                        total_device.edit_hours_1 = 9;
                    }
                    update_editable_time();
                    break;
                }
                case TEMPER_INCREASE_0:
                {
                    total_device.edit_temper_0++;
                    if (total_device.edit_temper_0 == 10)
                    {
                        total_device.edit_temper_0 = 0;
                        total_device.edit_temper_1++;
                        if (total_device.edit_temper_1 == 10)
                        {
                            total_device.edit_temper_1 = 0;
                            total_device.edit_temper_2++;
                            if (total_device.edit_temper_2 == MAX_HUNDRED_CELSIUS)
                            {
                                total_device.edit_temper_2 = 0;
                            }
                        }
                    }
                    update_editable_temper();
                    break;
                }
                case TEMPER_INCREASE_1:
                {
                    total_device.edit_temper_1++;
                    if (total_device.edit_temper_1 == 10)
                    {
                        total_device.edit_temper_1 = 0;
                        total_device.edit_temper_2++;
                        if (total_device.edit_temper_2 == MAX_HUNDRED_CELSIUS)
                        {
                            total_device.edit_temper_2 = 0;
                        }
                    }
                    update_editable_temper();
                    break;
                }
                case TEMPER_INCREASE_2:
                {
                    total_device.edit_temper_2++;
                    if (total_device.edit_temper_2 == MAX_HUNDRED_CELSIUS)
                    {
                        total_device.edit_temper_2 = 0;
                    }
                    update_editable_temper();
                    break;
                }
                case TEMPER_DECREASE_0:
                {
                    total_device.edit_temper_0--;
                    if (total_device.edit_temper_0 < 0)
                    {
                        total_device.edit_temper_0 = 9;
                        total_device.edit_temper_1--;
                        if (total_device.edit_temper_1 < 0)
                        {
                            total_device.edit_temper_1 = 9;
                            total_device.edit_temper_2--;
                            if (total_device.edit_temper_2 < 0)
                            {
                                total_device.edit_temper_2 = MAX_HUNDRED_CELSIUS - 1;
                            }
                        }
                    }
                    update_editable_temper();
                    break;
                }
                case TEMPER_DECREASE_1:
                {
                    total_device.edit_temper_1--;
                    if (total_device.edit_temper_1 < 0)
                    {
                        total_device.edit_temper_1 = 9;
                        total_device.edit_temper_2--;
                        if (total_device.edit_temper_2 < 0)
                        {
                            total_device.edit_temper_2 = MAX_HUNDRED_CELSIUS - 1;
                        }
                    }
                    update_editable_temper();
                    break;
                }
                case TEMPER_DECREASE_2:
                {
                    total_device.edit_temper_2--;
                    if (total_device.edit_temper_2 < 0)
                    {
                        total_device.edit_temper_2 = MAX_HUNDRED_CELSIUS - 1;
                    }
                    update_editable_temper();
                    break;
                }
                case VACUUM_INCREASE_0:
                {
                    total_device.edit_vacuum_0++;
                    if (total_device.edit_vacuum_0 == 9)
                    {
                        total_device.edit_vacuum_0 = 0;
                    }
                    update_editable_vacuum();
                    break;
                }
                case VACUUM_DECREASE_0:
                {
                    total_device.edit_vacuum_0--;
                    if (total_device.edit_vacuum_0 < 0)
                    {
                        total_device.edit_vacuum_0 = 9;
                    }
                    update_editable_vacuum();
                    break;
                }
                case NO_BUTTON:
                {

                	break;
                }
            }
            break;
        }
    }
}


void TotalDisplay::release_all_buttons_in_vect(vector<Widget>& vect_to_release)
{
    for (vector<Widget>::size_type i = 0; i != vect_to_release.size(); i++)
    {
        if (vect_to_release[i].button_name != NO_BUTTON)
        {
            if (vect_to_release[i].button_is_pressed == true)
            {
                vect_to_release[i].button_is_pressed = false;
                vect_to_release[i].draw_img_vector(vect_to_release[i].released_btn_images, \
                                                    vect_to_release[i].wgt_coord_x, \
                                                    vect_to_release[i].wgt_coord_y);          // отрисовываем отпущенную кнопку
            }
        }
    }
}

void TotalDisplay::i2c_read_touch(void)
{
    if (touch_buffer_is_ready())
    {
        TouchPoint current_touch_point;
        current_touch_point.touch_coord_x = i2c_write_read_register(I2C_POINT_1_X_LOW_BYTE);
        current_touch_point.touch_coord_x += (i2c_write_read_register(I2C_POINT_1_X_HIGH_BYTE)) << 8;
        current_touch_point.touch_coord_y = i2c_write_read_register(I2C_POINT_1_Y_LOW_BYTE);
        current_touch_point.touch_coord_y += (i2c_write_read_register(I2C_POINT_1_Y_HIGH_BYTE)) << 8;
        //i2c_write_read_register(I2C_POINT_1_SIZE_W);
        //i2c_write_read_register(I2C_POINT_1_SIZE_H);
        i2c_write_addr_write_register(I2C_TOUCH_BUFF_STATUS, 0x00);
        if ((current_touch_point.touch_coord_x <= DISPLAY_WIDTH) && (current_touch_point.touch_coord_y <= DISPLAY_HEIGHT))
        {
            if (coord_glitch_filter == TOUCH_GLITCH_COUNTER)
            {
                Buttons_list tmp_btn_name = identify_pressed_btn(current_touch_point.touch_coord_x, current_touch_point.touch_coord_y, \
                                                                return_current_page_vect(current_page_name));
                handle_button_press(tmp_btn_name, return_current_page_vect(current_page_name), current_page_name);
            }
            coord_glitch_filter++;
            if (coord_glitch_filter > TOUCH_GLITCH_COUNTER)
            {
                coord_glitch_filter = TOUCH_GLITCH_COUNTER + 1;
            }
        }
    }
    else
    {
        coord_glitch_filter = 0;
        release_all_buttons_in_vect(return_current_page_vect(current_page_name));
        //release_all_buttons_in_vect(mutable_wgts);
    }
}

void Widget::set_rectangle_page_0(void)
{
    for (int i = 0; i < 43; i++)                                    // вертикальная правая
    {
        add_img_to_wgt(CONSTANT_IMG, img_vertic_frame, 780, (i*10) + 25);
    }
    for (int i = 0; i < 43; i++)                                    // вертикальная левая
    {
        add_img_to_wgt(CONSTANT_IMG, img_vertic_frame, 20, (i*10) + 25);
    }
    for (int i = 0; i < 75; i++)                                    // горизонтальная верхняя
    {
        add_img_to_wgt(CONSTANT_IMG, img_horiz_frame, (i*10) + 25, 20);
    }
    for (int i = 0; i < 75; i++)                                    // горизонтальная нижняя
    {
        add_img_to_wgt(CONSTANT_IMG, img_horiz_frame, (i*10) + 25, 460);
    }

    add_img_to_wgt(CONSTANT_IMG, img_frame_rounding_top_left, 20, 20);         // верхнее левое
    add_img_to_wgt(CONSTANT_IMG, img_frame_rounding_down_left, 20, 452);       // нижнее левое
    add_img_to_wgt(CONSTANT_IMG, img_frame_rounding_down_right, 772, 452);       // нижнее правое
    add_img_to_wgt(CONSTANT_IMG, img_frame_rounding_top_right, 772, 20);        // верхнее правое
}


// заполняем определённую область повторяющимся изображением
void Widget::tile_area(tImage image_to_tile, uint16_t area_width, uint16_t area_hight)
{
    uint16_t repetitions_x = area_width/image_to_tile.width;            // вычисляем количество повторений по X и Y
    uint16_t repetitions_y = area_width/image_to_tile.height;
    for (int i = 0; i < repetitions_y; i++)                            // проходимся по горизонтали
    {
        for (int ii = 0; ii < repetitions_x; ii++)                     // проходимся по вертикали
        {
            add_img_to_wgt(CONSTANT_IMG, image_to_tile, (image_to_tile.width * ii), (image_to_tile.height * i));    // отрисовываем фоновое изображение
        }
    }
}

// заменяем изображение в виджете
void Widget::change_image_in_widget(tImage image_to_output)
{
    changeable_images.clear();      // очищаем вектор изменяемых изображений в виджете
    add_img_to_wgt(CHANGEABLE_IMG, image_to_output);  // добавляем изображение в вектор изменяемых изображений
    draw_img_vector(changeable_images, wgt_coord_x, wgt_coord_y);                       // отрисовываем изменяемые изображения
}

// заменяем изображение в виджете
void Widget::change_image_in_widget(tImage image_to_output, uint16_t img_out_coord_x, uint16_t img_out_coord_y)
{
    changeable_images.clear();      // очищаем вектор изменяемых изображений в виджете
    add_img_to_wgt(CHANGEABLE_IMG, image_to_output, img_out_coord_x, img_out_coord_y);  // добавляем изображение в вектор изменяемых изображений
    draw_img_vector(changeable_images, wgt_coord_x, wgt_coord_y);                       // отрисовываем изменяемые изображения
}




void test_draw_all(void)
{
	total_device.all_pages_write_to_memory();
}


void sec_handler(void)
{
	if (total_device.display_initialized)
	{
		total_device.colon_blink();
	}

	if (!total_device.display_initialized)
	{
		init_layers();
		total_device.display_initialized = 1;
	}
}

uint32_t ms_ticks_previous = 0;
uint16_t ms_ticks_counter = 0;
uint16_t i2c_ms_counter = 0;

void handle_ms_tick(uint32_t ms_ticks_current)
{
	if (ms_ticks_current != ms_ticks_previous)
	{
		ms_ticks_previous = ms_ticks_current;
		ms_ticks_counter++;
		if (ms_ticks_counter == 1000)
		{
			ms_ticks_counter = 0;
			sec_handler();
		}

		i2c_ms_counter++;
		if(i2c_ms_counter == I2C_TRANSMISSION_INTERVAL_MS)
		{
			i2c_ms_counter = 0;
			total_device.i2c_read_touch();
		}
	}
}

#ifdef __cplusplus
}
#endif



