#ifndef __CONFIG_H
#define __CONFIG_H

/*=========================
   Application configuration
 *=========================*/

#define CONFIG_SYSTEM_SAVE_FILE_PATH          "/SystemSave.json"
#define CONFIG_SYSTEM_SAVE_FILE_BACKUP_PATH   "/.SystemSaveBackup.json"
#define CONFIG_SYSTEM_LANGUAGE_DEFAULT        "en-GB"//{'e','n','-','G','B'}
#define CONFIG_SYSTEM_TIME_ZONE_DEFAULT      (-5) // GMT- 5, Ann Arbor
#define CONFIG_SYSTEM_SOUND_ENABLE_DEFAULT    false

#define CONFIG_WEIGHT_DEFAULT                 70 // kg

#define CONFIG_GPS_REFR_PERIOD                1000 // ms
#define CONFIG_GPS_LONGITUDE_DEFAULT          -83.7161   //Ann Arbor duder
#define CONFIG_GPS_LATITUDE_DEFAULT           42.2905
#define CONFIG_GPS_LONGTITUDE_OFFSET          -0.041925
#define CONFIG_GPS_LATITUDE_OFFSET            0.002818

#define CONFIG_MAP_DOUBLE_CLICKED_DELAY       700
#define CONFIG_MAP_LONG_PRESSED_TIME          1000

#define CONFIG_TRACK_FILTER_OFFSET_THRESHOLD  2 // pixel
#define CONFIG_TRACK_RECORD_FILE_DIR_NAME     "Track"

#define CONFIG_ROUTE_FILE_DIR_NAME            "/Route"

#define CONFIG_MAP_USE_WGS84_DEFAULT          false

//#define CONFIG_MAP_DIR_PATH                   {'/','M','A','P'}//"/MAP"
#define CONFIG_MAP_DIR_PATH_DEFAULT           "/MAP"

#define CONFIG_MAP_EXT_NAME_DEFAULT           "bin"

#define CONFIG_ARROW_THEME_DEFAULT            "default"

#define CONFIG_LIVE_MAP_LEVEL_DEFAULT         16
#define CONFIG_LIVE_MAP_VIEW_WIDTH            LV_HOR_RES
#define CONFIG_LIVE_MAP_VIEW_HEIGHT           LV_VER_RES

// color config
#define UMICH_COLOR_MAIZE                     lv_color_hex(0xffcb05)
#define UMICH_COLOR_BLUE                      lv_color_hex(0x00274c)
#define UMICH_COLOR_WHITE                     lv_color_hex(0xffffff)

// #define COLOR_BACKGROUND                      lv_color_black()
// #define COLOR_FOCUS                           lv_color_hex(0xff931e)
// #define COLOR_TEXT                            lv_color_white()
// #define COLOR_DIM_TEXT                        lv_color_hex(0xb3b3b3)
// #define COLOR_UNFOCUS                         lv_color_hex(0x666666)
// #define COLOR_PRESSED                         lv_color_hex(0xbbbbbb)

#define COLOR_BACKGROUND                      UMICH_COLOR_BLUE
#define COLOR_FOCUS                           UMICH_COLOR_MAIZE

// #define COLOR_BACKGROUND                      lv_color_black()
// #define COLOR_FOCUS                           UMICH_COLOR_BLUE
#define COLOR_TEXT                            UMICH_COLOR_WHITE
#define COLOR_DIM_TEXT                        lv_color_hex(0x939393)
#define COLOR_UNFOCUS                         COLOR_DIM_TEXT
#define COLOR_PRESSED                         COLOR_DIM_TEXT

/* Simulator */
#define CONFIG_TRACK_VIRTUAL_GPX_FILE_PATH    "S:/TRK_20210801_203324.gpx"

/*=========================
   Hardware Configuration
 *=========================*/

/* Sensors */
#define CONFIG_SENSOR_ENABLE        1

#if CONFIG_SENSOR_ENABLE
#  define CONFIG_SENSOR_IMU_ENABLE  1
#  define CONFIG_SENSOR_MAG_ENABLE  1
#endif

#define NULL_PIN                    PD0

/* Screen */
#define CONFIG_SCREEN_CS_PIN        9
#define CONFIG_SCREEN_DC_PIN        2
#define CONFIG_SCREEN_RST_PIN       4
#define CONFIG_SCREEN_SCK_PIN       18
#define CONFIG_SCREEN_MOSI_PIN      23
#define CONFIG_SCREEN_BLK_PIN       12

#define CONFIG_SCREEN_HOR_RES       240//240
#define CONFIG_SCREEN_VER_RES       320
#define CONFIG_SCREEN_BUFFER_SIZE   (CONFIG_SCREEN_HOR_RES * CONFIG_SCREEN_VER_RES /2)

/* Battery */
#define CONFIG_BAT_DET_PIN          37
#define CONFIG_BAT_CHG_DET_PIN      38

/* Buzzer */
#define CONFIG_BUZZ_PIN             25
#define CONFIG_BUZZ_CHANNEL         2
#define CONFIG_SOUND_ENABLE_DEFAULT false

/* GPS */
#define CONFIG_GPS_SERIAL           Serial2
#define CONFIG_GPS_USE_TRANSPARENT  0
#define CONFIG_GPS_BUF_OVERLOAD_CHK 0
#define CONFIG_GPS_TX_PIN           36
#define CONFIG_GPS_RX_PIN           19

/* IMU */
#define CONFIG_IMU_INT1_PIN         32
#define CONFIG_IMU_INT2_PIN         33

/* I2C */
#define CONFIG_MCU_SDA_PIN          32
#define CONFIG_MCU_SCL_PIN          33

/* Encoder */
#define CONFIG_ENCODER_B_PIN        34
#define CONFIG_ENCODER_A_PIN        35
#define CONFIG_ENCODER_PUSH_PIN     27

/* Power */
#define CONFIG_POWER_EN_PIN         21
#define CONFIG_IMU_GPS_PWR_PIN      25
#define CONFIG_SCREEN_PWR_PIN       5

/* Debug USART */
#define CONFIG_DEBUG_SERIAL         Serial
#define CONFIG_DEBUG_RX_PIN         PA10
#define CONFIG_DEBUG_TX_PIN         PA9

/* SD CARD */
#define CONFIG_SD_SPI               SPI_SD
#define CONFIG_SD_CD_PIN            -1
#define CONFIG_SD_MOSI_PIN          PB15
#define CONFIG_SD_MISO_PIN          PB14
#define CONFIG_SD_SCK_PIN           PB13
#define CONFIG_SD_CS_PIN            15
#define CONFIG_SD_DET_PIN           22

/* Stack Info */
#define CONFIG_USE_STACK_INFO       0

#endif
