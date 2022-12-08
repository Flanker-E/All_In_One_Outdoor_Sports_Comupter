#include "HAL/HAL.h"
#include "App/Utils/ButtonEvent/ButtonEvent.h"
// #include "App/Accounts/Account_Master.h"
#include "App/Common/DataProc/DataProc.h"

static ButtonEvent EncoderPush(5000,500,700);

static bool EncoderEnable = true;
static volatile int16_t EncoderDiff = 0;
static bool EncoderDiffDisable = false;

Account* actEncoder;

extern volatile bool isNormalMode;
extern volatile bool einkNeedUpdate;
// extern volatile bool isLcdOff;

static void Encoder_IrqHandler()
{
    // Serial.println("Encoder irq handle");
    if (!EncoderEnable || EncoderDiffDisable)
    {
        return;
    }
    // Serial.println("count");
    static volatile int count, countLast;
    static volatile uint8_t a0, b0;
    static volatile uint8_t ab0;
    uint8_t a = digitalRead(CONFIG_ENCODER_A_PIN);
    uint8_t b = digitalRead(CONFIG_ENCODER_B_PIN);
    if (a != a0)
    {
        a0 = a;
        if (b != b0)
        {
            b0 = b;
            count += ((a == b) ? 1 : -1);
            if ((a == b) != ab0)
            {
                count += ((a == b) ? 1 : -1);
            }
            ab0 = (a == b);
        }
    }

    if (count != countLast)
    {
        EncoderDiff += (count - countLast) > 0 ? 1 : -1;
        countLast = count;
    }
}

static void Encoder_PushHandler(ButtonEvent* btn, int event)
{
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        // Serial.println("pressed");
        // HAL::Buzz_Tone(500, 20);
        EncoderDiffDisable = true;
    } else if (event == ButtonEvent::EVENT_RELEASED)
    {
        // Serial.println("released");
        // HAL::Buzz_Tone(700, 20);
        EncoderDiffDisable = false;
    }  else if (event == ButtonEvent::EVENT_DOUBLE_CLICKED)
    {
        Serial.println("button event double clicked");
        isNormalMode=!isNormalMode;
        einkNeedUpdate=true;
        // HAL::Buzz_Tone(700, 20);
        // EncoderDiffDisable = false;
    }  else if (event == ButtonEvent::EVENT_LONG_PRESSED)
    {
        Serial.println("button event long pressed");
        // HAL::Audio_PlayMusic("Shutdown");
        HAL::Power_Shutdown();
    }
}

static void Encoder_RotateHandler(int16_t diff)
{
    HAL::Buzz_Tone(300, 5);

    actEncoder->Commit((const void*) &diff, sizeof(int16_t));
    actEncoder->Publish();
}

void HAL::Encoder_Init()
{
    pinMode(CONFIG_ENCODER_A_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_B_PIN, INPUT_PULLUP);
    pinMode(CONFIG_ENCODER_PUSH_PIN, INPUT_PULLUP);

    attachInterrupt(CONFIG_ENCODER_A_PIN, Encoder_IrqHandler, CHANGE);

    EncoderPush.EventAttach(Encoder_PushHandler);
    Serial.println("encoder intr and event attached");

    actEncoder = new Account("Encoder", DataProc::Center(), sizeof(int16_t), nullptr);

}

void HAL::Encoder_Update()
{
    // if(Encoder_GetIsPush())
    //     Serial.println("is pushed");
    EncoderPush.EventMonitor(Encoder_GetIsPush());
}

int16_t HAL::Encoder_GetDiff()
{
    int16_t diff = -EncoderDiff / 2;
    if (diff != 0)
    {
        // EncoderDiff是实际的脉冲数；把本次变量用掉了，需要重新置0
        EncoderDiff = 0;
        Encoder_RotateHandler(diff);
    }

    return diff;
}

bool HAL::Encoder_GetIsPush()
{
    
    if (!EncoderEnable)
    {
        return false;
    }

    return (digitalRead(CONFIG_ENCODER_PUSH_PIN) == LOW);
}

void HAL::Encoder_SetEnable(bool en)
{
    EncoderEnable = en;
}
