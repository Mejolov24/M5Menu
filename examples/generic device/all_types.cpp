#include <M5GFX.h>
#include <M5Unified.h>
M5Canvas canvas(&M5.Lcd);
#define MAX_DEPTH 1
#define DEFAULT_SCROLL 1 // 0 = CLAMP | 1 = WRAP | 2 = CYCLIC
#include <M5Config.h>
M5Config config;

uint8_t item1 = 0;
uint16_t item2 = 0;
uint32_t item3 = 0;
int8_t item4 = 0;
int16_t item5 = 0;
int32_t item6 = 0;
bool item7 = false;
uint8_t item8 = 0;
uint8_t item9 = 0;

uint8_t item11 = 0; // can also be declared like this
uint8_t item11_increment = 1;
uint8_t item11_min = 0;
uint8_t item11_max = 5;

uint8_t item12 = 0;
uint8_t item13 = 0;

String item14[]{"Hello","Wolrd!",":)"};
uint8_t item14_index = 0;


void item10(){
    canvas.clear();
    canvas.drawString("Hi, this is a function,", 0,0,&fonts::Font4);
    canvas.drawString("Menu isnt auto closed", 0,32,&fonts::Font4);
    canvas.drawString("You must call close().", 0,64,&fonts::Font4);
    canvas.pushSprite(0,0);
}


// full declarations:
M5Config::ConfigItem configs2[] = {
    {
        "0-10", // name
        &item8, // pointer to variable
        1, // increment
        0, // minimum
        10 // maximum
    },
    {
        "0-100, +10, clamp", 
        &item9,
        10,
        0,
        100,
        M5Config::ScrollType::TYPE_CLAMP
    },
        {
        "0-100, +10, wrap", 
        &item12,
        10,
        0,
        100,
        M5Config::ScrollType::TYPE_WRAP
    },
        {
        "0-100, +10, cyclic", 
        &item13,
        10,
        0,
        100,
        M5Config::ScrollType::TYPE_CYCLIC
    },
    {
        "function",
        item10 // function pointer
    },
    {
        "declaration 2",
        &item11, 
        item11_increment,
        item11_min,
        item11_max
    }
};

M5Config::ConfigMenu menu2(2,configs2);

M5Config::ConfigItem configs[] = {
    {
        "uint8_t",
        &item1,
    },
    {
        "uint16_t",
        &item2,
    },
    {
        "uint32_t",
        &item3,
    },
    {
        "int8_t",
        &item4,
    },
    {
        "int16_t",
        &item5,
    },
    {
        "int32_t",
        &item6,
    },
    {
        "bool",
        &item7
    },
    {
        "String array",
        &item14_index,
        item14
    },
    {
        "sub menu",
        &menu2
    },
};

M5Config::ConfigMenu menu(1,configs);

void OnUsage(M5Config::ConfigItem* item, M5Config::ConfigMenu* menu){
    // used for updating values
    }

void setup(){
    auto cfg = M5.config();
    M5.begin(cfg); 
    canvas.createSprite(M5.Lcd.width(), M5.Lcd.height());

    // custom theme setup, setting every single theme entry:
    M5Config::ExplorerTheme theme;
    theme.background_color = 0x211a;
    theme.border_color = 0x2c9f;
    theme.selection_color = 0x06e0;
    theme.value_color = WHITE;
    theme.item_height = 23;
    theme.item_window = 5;
    theme.bool_false_color = RED;
    theme.bool_true_color = GREEN;
    theme.bool_true_string = "True";
    theme.bool_false_string = "False";
    theme.font = &fonts::FreeSans12pt7b;

    // Setup and open menu
    config.begin(&canvas,OnUsage);
    config.setTheme(&theme);
    config.goToMenu(&menu); // Only call with append argument true when adding some kind of hidden setting that has no item entry
    config.open(); // if an argument is not providen, it will use defualts or latest config.
}

void loop(){
// TODO : send input via config.process_input(M5Config::Input::);
}