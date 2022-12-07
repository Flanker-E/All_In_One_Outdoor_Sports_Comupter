# All-in-one Outdoor Sports Comupter
## changelog
20220617 demo widges, run in FreeRTOS, with semaphor. Record before adding X-track page management and display port.  
20220624 touchpad on peak pages.  
20220626 sysinfo works. battery label update error.  
20220629 X-track pages, simulator, touchpad problem.  
20220928 sync using new machine.  
20221004 update hardware design.  
20221102 PCB v2.7 =>Fab  
20221110 case 3D modeling v1.3  
20221116 driver switch between LCD and EINK  
20221120 case 3D modeling v1.4, revision for PCB v2.7  
20221122 incorporate encoder and touch screen  
20221123 solved the switch between LCD and Eink  
20221127 adding settings page  
20221128 design settings items' icon  
20221202 fix encoder click logic  
20221205 poster stuff, tf card and map load  
20221206 time issure, positioning offset, route recording enable  
20221207 available route read and selection UI  

## Credit to 
Based on [X-Track](https://github.com/FASTSHIFT/X-TRACK), [Peak](https://github.com/peng-zhihui/Peak) and [Peak-T2](https://gitee.com/forairaaaaa/peak-t2/tree/master/1.Hardware#https://gitee.com/link?target=https%3A%2F%2Foshwhub.com%2Feedadada%2Ffed36bbc19da4527b237982f9cd09f99)

## Firmware compile using PIO
Open project with Vscode, wait for PlatformIO to configure the project.  

### configure lvgl/lv_conf.h  
copy from lv_conf_template.h and change name to lv_conf.h  
line 15 change to -> #if 1 
line 52 change to -> #define LV_MEM_SIZE (38U * 1024U)          /*[bytes]*/  
line 88 change to -> #define LV_TICK_CUSTOM 1  
line 114 change to -> #define LV_SHADOW_CACHE_SIZE 50  
line 174 change to -> #define LV_USE_LOG 1  
line 210 change to -> #define LV_USE_ASSERT_STYLE 1  
line 245 change to -> #define LV_SPRINTF_USE_FLOAT 1  
line 306 change to -> #define LV_FONT_MONTSERRAT_10 1  
line 314 change to -> #define LV_FONT_MONTSERRAT_26 1  
line 348 change to -> #define LV_FONT_FMT_TXT_LARGE 1  
line 351 change to -> #define LV_USE_FONT_COMPRESSED 1  
line 526 change to -> #define LV_USE_THEME_BASIC 0  
line 529 change to -> #define LV_USE_THEME_MONO 0  

### using 7789V(PCB):  
#### configure TFT_eSPI/User_Setup.h  
uncomment line 45  
comment line 168-170  
comment line 205-210 and 223  
#### configure TFT_eSPI/User_Setup_select.h  
uncomment line 72 setup24_st7789.h  
#### configure TFT_eSPI/User_Setups/Setup24_ST7789.h  
uncomment line 10, enable TFT_RGB  
uncomment line 22-27, change TFT_CS to 9  
comment line 30-33  

### using IL9341(quick proto):   
#### configure TFT_eSPI/User_Setup.h  
comment line 168-170  
uncomment line 205-210 and 223  


## How to get simulator work on Windows
Clone project.  
Open project with Vscode, wait for PlatformIO to configure the project.  
Download Visual Studio 2019.(can't work with 2022)  
Install module: C++ desktop(click on MSVC v142-VS 2019) and .NET desktop.  
Open the .sln file, select x86 Debug and run.  
Then the simulator should can work.  


## How to get simulator work on MacOS
todo 
some basic function don't work under Mac c++ env.

## Software Part Understanding
### Software Init Pipeline
Setup:  
* HAL_init;  
    * buzz: thread(tailored in this project)  
*	Port_Init //LCD  
    * Tft object setup, binding with LVGL  
    * Create FreeRTOS task  
*	App_init  
    * Init data buffer, lv clear, init resourse, create apps, bind service  

Loop:  
*	HAL_update  

After that, the FreeRTOS will periodically call lv_handler, which gives lvgl heart beat.  
The software structure and lvgl outside of FreeRTOS will listen from input and periodic timers to do its job.  

### Software Structure (explaination on content)
#### FreeRTOS
* Task1: lvgl update  
* Task2: pending: HAL update 
* Task3: pending: switch between LCD and Eink 
plan : using state machine to control the shift between normal mode and low power mode.  
#### Data Buffer:
Namespace DataProc:  

class:  
Account:   
all account connect to one data center. Using an id to distinguish.   
Having a general pingpong buffer and a specified user data pointer.  
Using commit to get data and use publish to notify all subscriber with new data.  
Can data can be pull from subscriber.  
Timer’s utility? Cmd utility?  

DataCenter:  
Provide find specific account utility.  
Store accounts using account pool, which is a vector.  

Pingpong buffer:  

    void* buffer[2];
        volatile uint8_t writeIndex;
        volatile uint8_t readIndex;
        volatile uint8_t readAvaliable[2];
          
readidx=0 or 1. After read, set avai to 0.  
Write: can not be the same with read. Write to another slot and set its availability to 1.  

#### App
AppFactory: public inherited from PageFactory  
Provide createpage check function. Check whether “create a page named xx” is legal. Then return the page object  

PageManager: manage pages. Install, register, push&pop of page stack, page switch and the other stuff.  
Organized the page-switching logic!  
Install of page: init page, assign some information of this page.  
Register of page: assign PM’s this pointer, assign page name to itself (?), pushback in pagepool  

Page stack and page pool:  
Page stack is used to store the sequence of appearance of pages. Page pool is used to check whether a page is registered or not.  

The app will show after it is pushed into the stack by calling “SwitchTo”  
Switch to: detect and copy stash into the target page.  

The PM will raise StateUpdate to check the state and call corresponding functions in pages (state machine)  


App使用MVC框架  
[基本概念参考:](https://www.zhihu.com/question/27897315)  
[详细细节以及 IOS viewcontroler：](https://www.jianshu.com/p/caaa173071f3)  
[跟MVP的区别？](http://c.biancheng.net/view/7743.html) 然后会发现mvc的定义出现了分歧  


实现了页面调度（生命周期）的功能  
移植 通过push添加页面进池，pop弹出当前页面回到上一个页面  
确认硬件没有初始化导致systeminfo会引用空指针  
添加其他页面  

Page：接口功能，包括创建，显示，加载等  
PageView：功能实现，如创建，建组等  
PageModel：数据交互功能，如读取数据更新等  

onBtnClicked called by onEvent. onEvent is a callback function, which is attached to events by lv_obj_add_event_cb In onEvent

The onEvent will call different functions based on the event code.  

The information updates of map and dialplate are done by lv_timer  

#### Button and Lvgl
Encoder_Init provide: hardware interrupt when a pin changed, attach encoder with buttonEvent  
Bind encoder in the Encoder_Init only provide long press power off feature(disable encoder difference calc)  
Encoder_Update will call ButtonEvent’s EventMonitor, which judge the state of press. This can determin whether device should be power off.  

Lvgl determines press event using the encoder_read call-back function. It reads diff and state from encoder driver.  
#### Map
Using Microsoft Tiled map system: different scale, different image.   
Which uses scale number to calculate the crop of an image.  

Namespace page  
Class livemap :pagebase  

MapConv mapConv;  
TileConv tileConv;  

When things are being updated. Enter or outside from map?
Using lv_poly_line to record track line


Observation: the map will exit when button is pressed(before release)
#### Resource Pool
Import fonts, label images



