/**
 * @file lv_port_indev_templ.c
 *
 */

/*Copy this file as "lv_port_indev.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#include "../Display.h"
#include "HAL/HAL.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void encoder_init(void);
static void encoder_read(lv_indev_drv_t* indev_drv, lv_indev_data_t* data);

/**********************
 *  STATIC VARIABLES
 **********************/

lv_indev_t* encoder_indev;
lv_indev_t* touch_indev;
static TouchPoint_t coordinate;
// static lv_indev_drv_t indev_drv;
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*Read the touchpad*/
#ifdef TOUCH_CS
static void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;

    bool touched = tft.getTouch( &touchX, &touchY);

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        Serial.printf("touched:x:%d,y:%d\n",touchX,touchY);
        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;

    }
}
#else
static void my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    // static lv_coord_t last_x = 0;
    // static lv_coord_t last_y = 0;

    // /*Save the pressed coordinates and the state*/
    // if(touchpad_is_pressed()) {
    //     touchpad_get_xy(&last_x, &last_y);
    //     data->state = LV_INDEV_STATE_PR;
    // } else {
    //     data->state = LV_INDEV_STATE_REL;
    // }

    // /*Set the last pressed coordinates*/
    // data->point.x = last_x;
    // data->point.y = last_y;


    /* Read touch point */
    HAL::Touch_GetPressPoint(&coordinate);
    // Serial.printf("x:%d, y:%d \r\n", coordinate.x, coordinate.y);
    if (coordinate.x == -1) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        data->point.x = coordinate.x;
        data->point.y = coordinate.y;
        data->state = LV_INDEV_STATE_PR;
        // Serial.printf("touched:x:%d,y:%d\n",coordinate.x, coordinate.y);
    }
}
#endif

void lv_port_indev_init(void)
{
    // test=1;
    static lv_indev_drv_t indev_drv_Touchpad;
    static lv_indev_drv_t indev_drv_Encoder;
    lv_group_t* group = lv_group_create();
    #ifdef TOUCH_CS
    
    lv_indev_drv_init( &indev_drv_Touchpad );
    indev_drv_Touchpad.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    touch_indev = lv_indev_drv_register( &indev_drv_Touchpad );
    Serial.println( "indev" );
    // lv_group_t* group = lv_group_create();
    lv_indev_set_group(touch_indev, group);
    lv_group_set_default(group);
    #else
    lv_indev_drv_init(&indev_drv_Touchpad);
    indev_drv_Touchpad.type = LV_INDEV_TYPE_POINTER;
    indev_drv_Touchpad.read_cb = my_touchpad_read;
    touch_indev = lv_indev_drv_register(&indev_drv_Touchpad);
    lv_indev_set_group(touch_indev, group);
    #endif
    // Serial.printf( "indevtest:%d\n",test );
    /*------------------
     * Encoder
     * -----------------*/

    /*Initialize your encoder if you have*/
    // encoder_init();

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv_Encoder);
    indev_drv_Encoder.type = LV_INDEV_TYPE_ENCODER;
    indev_drv_Encoder.read_cb = encoder_read;
    encoder_indev = lv_indev_drv_register(&indev_drv_Encoder);
    
    lv_indev_set_group(encoder_indev, group);
    lv_group_set_default(group);
    



    /* Later you should create group(s) with `lv_group_t * group = lv_group_create()`,
     * add objects to the group with `lv_group_add_obj(group, obj)`
     * and assign this input device to group to navigate in it:
     * `lv_indev_set_group(indev_encoder, group);` */
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

/*------------------
 * Encoder
 * -----------------*/

/* Initialize your keypad */
static void encoder_init(void)
{
    /*Do it in Init*/
}


/* Will be called by the library to read the encoder */
static void encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    // if(data->state!=LV_INDEV_STATE_PR)
    data->enc_diff = HAL::Encoder_GetDiff();
    data->state =  HAL::Encoder_GetIsPush() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    // if(data->state==LV_INDEV_STATE_REL)
    //     Serial.printf("encoder released");
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
