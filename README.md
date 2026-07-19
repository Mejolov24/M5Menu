# M5Menu
A lightweight Menu UI library for M5Stack devices, featuring a recursive menu navigator and custom themes.

Only tested on CardputerADV, but should work on any M5Stack device.

![thumbnail](https://github.com/Mejolov24/M5Menu/blob/main/thumbnail.jpeg?raw=true)

## Features
* Direct Data: Pass direct pointers to your variables (uint8_t, int32_t, bool, etc.). The library automatically handles type parsing, bounds checking, and variable updates under the hood.

* Functional Menu Actions: Support direct function pointers to execute instant code blocks right from the UI selection.

* Event-Driven Callbacks: SettingInteracted callback hooks to trigger background routines (like saving configurations to EEPROM or adjusting hardware state) the exact millisecond a value shifts.

* Recursive Directory Navigation: Smoothly browse the Menu and its SubMenus.

* Highly Customizable Themes: Fully customizable fonts, item size, window constraints, and colors for borders, active selections, and variable type states such as bool.

## Installation
### PlatformIO
Add the following to your `platformio.ini`:
```ini
lib_deps = 
    https://github.com/Mejolov24/M5Menu
```

## Prerequisites
M5Menu relies on the official M5Stack ecosystem for hardware interaction. Ensure your project is targeting an ESP32 variant and that you initialize the device via `M5.begin()` inside `setup()`, and canvas using `M5Canvas canvas(&M5.Lcd)` at the top of your main sketch after.

## Initialization example:
```cpp
#include <M5GFX.h>
#include <M5Unified.h>
M5Canvas canvas(&M5.Lcd);

#include <M5Menu.h>
M5Menu menu;

void OnUsage(M5Menu::MenuItem* item, M5Menu::Menu* menu){}

void setup(){
    auto cfg = M5.config();
    M5.begin(cfg); 
    canvas.createSprite(M5.Lcd.width(), M5.Lcd.height());
    menu.begin(&canvas,OnUsage);
```
## Usage
All usage examples can be found under /examples

## Definitions

### preprocesor definitions
```cpp
#define MAX_DEPTH 8 // Size of the stack (BIGGER THAN 0)
#define DEFAULT_SCROLL 0 // default ScrollType
```

### Menu Theme
```cpp
struct MenuTheme {
    uint16_t background_color = BLACK;
    uint16_t border_color = WHITE;
    uint16_t selection_color = BLUE;
    uint16_t text_color = WHITE;
    uint16_t value_color = WHITE;
    uint16_t item_height = 16;
    uint16_t item_window = 6;
    uint16_t bool_true_color = GREEN;
    uint16_t bool_false_color = RED;
    String bool_true_string = "On";
    String bool_false_string = "Off";
    const lgfx::v1::IFont* font = &fonts::Font2;
};
```
### ValueType
Used for casting the Pointer.data, automaticlly handled by the preprocessor, if you need to declare MenuItems at runtime, you must use them.
```cpp
enum class ValueType {
    TYPE_UINT8_T,
    TYPE_UINT16_T,
    TYPE_UINT32_T,
    TYPE_INT8_T,
    TYPE_INT16_T,
    TYPE_INT32_T,
    TYPE_BOOL,
    TYPE_STRING_ARRAY,
    TYPE_SUBMENU,
    TYPE_FUNCTION
};
```
### ScrollType
```cpp
enum class ScrollType {
    TYPE_CLAMP, // range 0-100 | 100 + 10 = 100 (lock)
    TYPE_WRAP,  // range 0-100 | 100 + 10 = 0   (teleport)
    TYPE_CYCLIC // range 0-100 | 100 + 10 = 10  (modulo)
};
```
### IncrementType
In order to support multiple data types as in ValueType, an union is necesary for storing the increments.
They must match the ValueType or else unexpected behaviour will occur.
```cpp
union IncrementType {
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    int8_t i8;
    int16_t i16;
    int32_t i32;
};
```
### MenuItem and Menu
each tweakable element from the UI requires to be in an array and assigned into a Menu struct
```cpp
union Pointer {
    void* data;
    void (*function)();
};

struct MenuItem
{
    const char* name;
    ValueType type;

    Pointer pointer;
    IncrementType increment;
    IncrementType lower_limit;
    IncrementType upper_limit;

    ScrollType scroll_type;
}

struct Menu {
    uint8_t id;
    MenuItem* config_items;
};

// Usage
uint8_t item1 = 0;
bool item2 = 0;
void item3(){}

M5Menu::MenuItem mymenu[] = {
    {
        "0-10", // name
        &item1, // pointer to variable
        1, // increment
        0, // minimum
        10,// maximum
        M5Menu::ScrollType::TYPE_CLAMP
    },
    {
        "bool",
        &item2
    },
    {
        "function",
        item3 // function pointer
    },
    {
        "sub menu",
        &menu1 // pointer to Menu
    }
};

M5Menu::Menu menu(0,mymenu)
```
### goToMenu()
menu: pointer to Menu.
append: if true: adds the Menu pointer to the stack, if false: it overrides the entire stack (default to false)

Only call with append argument set to true when adding some kind of hidden settings menu that has no entry point.

This is usually called once at setup or if you have multiple settings at diffent places then its called before open()
```cpp
M5Menu::MenuItem mymenu;

menu.goToMenu(&mymenu);
```

### begin()
#### Arguments:
targetCanvas : pointer to a canvas object
callback : funtion to call after interacting with a MenuItem (optional argument).
```cpp
#include <M5GFX.h>
#include <M5Unified.h>
M5Canvas canvas(&M5.Lcd);
#include <M5Menu.h>

void OnUsage(M5Menu::MenuItem* item){}

void setup(){
    auto cfg = M5.config();
    M5.begin(cfg); 
    canvas.createSprite(M5.Lcd.width(), M5.Lcd.height());
    menu.begin(&canvas,OnUsage);
}
```

### setTheme()
overrides current theme
#### Arguments:
theme : pointer of a MenuTheme struct
```cpp
M5Menu::MenuTheme theme;
theme.value_color = WHITE;
menu.setTheme(&theme);
```

### open()
activates and renders the menu canvas at stack head activating all inputs 

### close()
closes and clears the canvas screen desactivating all inputs

### get_current_menu()
returns pointer of currently navigated menu

### process_input()
handles inputs.
```cpp
enum class Input {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    BACK,
    SELECT
};
// Usage
menu.process_input(M5Menu::Input::);
```

## License
This project is licensed under the MIT License.
