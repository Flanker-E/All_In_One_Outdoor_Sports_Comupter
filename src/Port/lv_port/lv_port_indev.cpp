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
#endif

void lv_port_indev_init(void)
{
    // test=1;
    static lv_indev_drv_t indev_drv;
    #ifdef TOUCH_CS
    
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    touch_indev = lv_indev_drv_register( &indev_drv );
    Serial.println( "indev" );
    lv_group_t* group = lv_group_create();
    lv_indev_set_group(touch_indev, group);
    lv_group_set_default(group);
    #endif
    // Serial.printf( "indevtest:%d\n",test );
    /*------------------
     * Encoder
     * -----------------*/

    /*Initialize your encoder if you have*/
    // encoder_init();

    /*Register a encoder input device*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = encoder_read;
    encoder_indev = lv_indev_drv_register(&indev_drv);
    lv_group_t* group = lv_group_create();
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
    data->enc_diff = HAL::Encoder_GetDiff();
    data->state =  HAL::Encoder_GetIsPush() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
