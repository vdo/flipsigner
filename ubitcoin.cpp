#include <furi.h>
#include <furi_hal_random.h>
#include <gui/gui.h>
// From: /lib/crypto
// #include <memzero.h>
// #include <rand.h>
// #include <pbkdf2.h>
// #include <monero/monero.h>
#include <Bitcoin.hpp>
#include <PSBT.hpp>
//#include <string>
//#include <wally_core.h>
//#include <wally_bip39.h>
#define TAG "ubitcoin"

typedef enum {
    MyEventTypeKey,
    MyEventTypeDone,
    MyEventTypeTrigger,
} MyEventType;

typedef struct {
    MyEventType type;
    InputEvent input;
} MyEvent;

FuriMessageQueue* queue;
char *mnemonic;
int x = 32;
int y = 48;

void my_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 10, 15, "uBitcoin");
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 20, 20, mnemonic); // TODO: SPLIT the phrase
}
/*  TEST STUFF */

void bip(void* context) {
    UNUSED(context);
    uint8_t entropy[10];
    furi_hal_random_fill_buf(entropy, 10);
    //UNUSED(res);
    //std::string s;
    //UNUSED(s);
    uint64_t fee;
    PSBT* psbt = new PSBT();
    //psbt->parseBase64("cHNidP8BAHECAAAAAbvujNVeCWqb0qj49V0FE4yNqzxNp/alDjJf6tfrDmLvAAAAAA");
    psbt->parseBase64("cHNidP8BAHECAAAAAbvujNVeCWqb0qj49V0FE4yNqzxNp/alDjJf6tfrDmLvAAAAAA"
    "D+////AgIMAwAAAAAAFgAUeVBepLdGFMTDXaEd8BcxXAjVXWSghgEAAAAAABYAFI0z1+/eJK2dc4lUc"
    "rmdIBEJbibsAAAAAAABAR/gkwQAAAAAABYAFBFV/d7WeO0DA8PtpNcne1avYrsIIgYCSUlKvX7bM2uW"
    "3icko9ATHDMcEV097AIjBJVvcYapQoAYpKDu61QAAIABAACAAAAAgAAAAAAAAAAAACICA0FKXUUYI9e"
    "Fz3Kf5iDa4Iz4fGUp1/a27bGx4zBNKl1mGKSg7utUAACAAQAAgAAAAIABAAAAAAAAAAAA");
    fee = psbt->fee();
    FURI_LOG_I(TAG, "Fee: %lld", fee);
    return;
}


void my_input_callback(InputEvent* input_event, void* context) {
    UNUSED(context);
    if(input_event->type == InputTypeShort) {

        if(input_event->key == InputKeyOk) {
            MyEvent event;
            event.type = MyEventTypeTrigger;
            bip(context);
            furi_message_queue_put(queue, &event, FuriWaitForever);
        }
        if(input_event->key == InputKeyBack) {
            MyEvent event;
            event.type = MyEventTypeDone;
            furi_message_queue_put(queue, &event, FuriWaitForever);
        }
    }
}

extern "C" int32_t flipero_app() {
    void* my_context = NULL;
    queue = furi_message_queue_alloc(8, sizeof(MyEvent));

    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, my_draw_callback, my_context);
    view_port_input_callback_set(view_port, my_input_callback, my_context);
    view_port_set_orientation(view_port, ViewPortOrientationHorizontal);
    Gui* gui = (Gui*) furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    MyEvent event;
    bool keep_processing = true;
    while (keep_processing) {
        if (furi_message_queue_get(queue, &event, FuriWaitForever) == FuriStatusOk) {
            if (event.type == MyEventTypeDone) {
                keep_processing = false;
            }
            else if(event.type == MyEventTypeTrigger) {
                // Run this when button pressed
                keep_processing = true;
            }
        } else {
            keep_processing = false;
        }
    }

    furi_message_queue_free(queue);
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    return 0;
}
