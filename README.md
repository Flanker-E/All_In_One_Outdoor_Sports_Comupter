# All-in-one Outdoor Sports Comupter
## changelog
20220617 demo widges, run in FreeRTOS, with semaphor. Record before adding X-track page management and display port.  
20220624 touchpad on peak pages.  
20220626 sysinfo works. battery label update error.  
20220629 X-track pages, simulator, touchpad problem.  
20220928 sync using new machine.  
20221004 update hardware design.  

## How to get simulator work on Windows
Download Visual Studio.  
Install module: C++ desktop(click on MSVC -VS 2019) and .NET desktop.  
Open the .sln file and run.  
Then the simulator should can work.  

configure lv_conf.h  
copy from lv_conf_template.h and change name  
line 15 change to -> #if 1 
line 52 change to -> #define LV_MEM_SIZE (38U * 1024U)          /*[bytes]*/  
line 88 change to -> #define LV_TICK_CUSTOM 1  
line 114 change to -> #define LV_SHADOW_CACHE_SIZE 50  
line 174 change to -> #define LV_USE_LOG 1  
line 210 change to -> #define LV_USE_ASSERT_STYLE         1  
line 306 change to -> #define LV_FONT_MONTSERRAT_10 1  
line 314 change to -> #define LV_FONT_MONTSERRAT_26 1  
line 348 change to -> #define LV_FONT_FMT_TXT_LARGE 1  
line 351 change to -> #define LV_USE_FONT_COMPRESSED 1  
line 526 change to -> #define LV_USE_THEME_BASIC 0  
line 529 change to -> #define LV_USE_THEME_MONO 0  

## How to get simulator work on MacOS
todo 
