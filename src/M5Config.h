#ifndef M5CONFIG_H
#define M5CONFIG_H

#ifndef MAX_DEPTH // Size of the stack (BIGGER THAN 0)
    #define MAX_DEPTH 8
#endif
#ifndef DEFAULT_SCROLL // default ScrollType
    #define DEFAULT_SCROLL 0
#endif

#include <stdint.h>
#include <type_traits>
#include <Arduino.h>
#include <M5GFX.h>

extern M5Canvas canvas;

class M5Config
{
public:

    struct ConfigMenu;
    struct ConfigItem;
    using SettingInteracted = void (*)(ConfigItem*,ConfigMenu*);

    struct ExplorerTheme {
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

    enum class Input {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        BACK,
        SELECT
    };

    // lower level stuff from now on:

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

    enum class ScrollType {
        TYPE_CLAMP, // range 0-100 | 100 + 10 = 100 (lock)
        TYPE_WRAP,  // range 0-100 | 100 + 10 = 0   (teleport)
        TYPE_CYCLIC // range 0-100 | 100 + 10 = 10  (modulo)
    };

    union IncrementType {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        int8_t i8;
        int16_t i16;
        int32_t i32;
    };

    union Pointer {
        void* data;
        void (*function)();
    };

    struct ConfigItem
    {
        const char* name;
        ValueType type;

        Pointer pointer;
        String* array_pointer = nullptr;
        IncrementType increment;
        IncrementType lower_limit;
        IncrementType upper_limit;

        ScrollType scroll_type;


        ConfigItem(const char* n, uint8_t* ptr, uint8_t inc = 1, uint8_t min = 0, uint8_t max = UINT8_MAX, ScrollType sct = ScrollType(DEFAULT_SCROLL))
            : name(n), type(ValueType::TYPE_UINT8_T), scroll_type(sct) {
            pointer.data = ptr; increment.u8 = inc; lower_limit.u8 = min; upper_limit.u8 = max;
        }

        ConfigItem(const char* n, uint16_t* ptr, uint16_t inc = 1, uint16_t min = 0, uint16_t max = UINT16_MAX, ScrollType sct = ScrollType(DEFAULT_SCROLL))
            : name(n), type(ValueType::TYPE_UINT16_T), scroll_type(sct) {
            pointer.data = ptr; increment.u16 = inc; lower_limit.u16 = min; upper_limit.u16 = max;
        }

        ConfigItem(const char* n, uint32_t* ptr, uint32_t inc = 1, uint32_t min = 0, uint32_t max = UINT32_MAX, ScrollType sct = ScrollType(DEFAULT_SCROLL))
            : name(n), type(ValueType::TYPE_UINT32_T), scroll_type(sct) {
            pointer.data = ptr; increment.u32 = inc; lower_limit.u32 = min; upper_limit.u32 = max;
        }

        ConfigItem(const char* n, int8_t* ptr, int8_t inc = 1, int8_t min = INT8_MIN, int8_t max = INT8_MAX, ScrollType sct = ScrollType(DEFAULT_SCROLL))
            : name(n), type(ValueType::TYPE_INT8_T), scroll_type(sct) {
            pointer.data = ptr; increment.i8 = inc; lower_limit.i8 = min; upper_limit.i8 = max;
        }

        ConfigItem(const char* n, int16_t* ptr, int16_t inc = 1, int16_t min = INT16_MIN, int16_t max = INT16_MAX, ScrollType sct = ScrollType(DEFAULT_SCROLL))
            : name(n), type(ValueType::TYPE_INT16_T), scroll_type(sct) {
            pointer.data = ptr; increment.i16 = inc; lower_limit.i16 = min; upper_limit.i16 = max;
        }

        ConfigItem(const char* n, int32_t* ptr, int32_t inc = 1, int32_t min = INT32_MIN, int32_t max = INT32_MAX, ScrollType sct = ScrollType(DEFAULT_SCROLL))
            : name(n), type(ValueType::TYPE_INT32_T), scroll_type(sct) {
            pointer.data = ptr; increment.i32 = inc; lower_limit.i32 = min; upper_limit.i32 = max;
        }

        // function overload

        ConfigItem(const char* n, void (*func)())
            : name(n),
              type(ValueType::TYPE_FUNCTION)
        {
            pointer.function = func;
        }

        // bool overload

        ConfigItem(const char* n, bool* ptr)
            : name(n),
              type(ValueType::TYPE_BOOL)
        {
            pointer.data = ptr;
            increment.u8 = 1;
            lower_limit.u8 = 0;
            upper_limit.u8 = 1;
        }
        // string array overload
        template <size_t N>
        ConfigItem(const char* n, uint8_t* ptr, String (&array)[N], ScrollType sct = ScrollType(DEFAULT_SCROLL))
            : name(n), type(ValueType::TYPE_STRING_ARRAY),
            scroll_type(sct)
        {
            increment.u8 = 1;
            lower_limit.u8 = 0;
            upper_limit.u8 = N - 1;
            pointer.data = ptr;
            array_pointer = array;
        }

        // submenu overload
        ConfigItem(const char* n, ConfigMenu* submenu)
            : name(n),
              type(ValueType::TYPE_SUBMENU)
        {
            pointer.data = submenu;
        }
    };

    struct ConfigMenu {
        uint8_t id;
        ConfigItem* config_items;
        uint16_t size;
        template <size_t N>
            ConfigMenu(uint8_t id_, ConfigItem (&items)[N])
        : id(id_), config_items(items), size(N){}
    };


private:
    // internal engine stuff
    M5Canvas* _canvas;

    ExplorerTheme _theme;
    SettingInteracted _callback = nullptr;

    ConfigMenu* _menuStack[MAX_DEPTH];

    uint8_t _width = 0;
    uint8_t _half_width = 0;
    uint8_t _height = 0;

    int8_t _cursor_index = 0;
    uint16_t _cursor_offset = 0;
    uint16_t _selection = 0;
    uint16_t _stack_index = 0;

    bool _had_theme = false;
    bool _active = false;

    void _goBack();
    void _incrementValue(ConfigItem* item, int8_t delta);
    String _formatValue(ConfigItem* item);

    public:

    void begin(M5Canvas* targetCanvas, SettingInteracted callback = nullptr);
    void setTheme(ExplorerTheme* theme = nullptr);
    void open();
    void close();
    void render();
    void goToMenu(ConfigMenu* menu, bool append = false); // if not append, override menu stack.
    void process_input(Input input);
    ConfigMenu* get_current_menu();
};

#endif