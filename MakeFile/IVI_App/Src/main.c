#include "Screen.h"
#include "demo.h"
#include "Event.h"

#define MAX_EVENTS   10
#define MAX_SCREENS  1

void show_menu() {
    printf("\n========= UI MENU =========\n");
    printf("***SCREEN CHOICES***\n");
    printf("1. Go to Home\n");
    printf("2. Go to Audio\n");
    printf("3. Go to Bluetooth\n");
    printf("4. Go to Settings\n");
    printf("5. Back to previous screen\n");
    printf("***EVENT CHOICES***\n");
    printf("6. Trigger event: Volume Up\n");
    printf("7. Trigger event: Brightness Down\n");
    printf("8. Process earliest event\n");
    printf("9. Show current screen\n");
    printf("0. Exit\n");
    printf("===========================\n");
    printf("Select option: ");
}

int main() {
    Screen_Stack screenlst;
    Event_queue eventlst;

    Init_ListEvent(&eventlst,MAX_EVENTS);
    Init_ListScreen(&screenlst,MAX_SCREENS);

    int choice;
    int running = 1;

    demo();
    while (running) {
        show_menu();
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1: push_screen(&screenlst,"Home"); break;
            case 2: push_screen(&screenlst,"Audio"); break;
            case 3: push_screen(&screenlst,"Bluetooth"); break;
            case 4: push_screen(&screenlst,"Settings"); break;
            case 5: pop_screen(&screenlst); break;
            case 6: enqueue_event(&eventlst,"Touch: Volume Up"); break;
            case 7: enqueue_event(&eventlst,"Touch: Brightness Down"); break;
            case 8: process_event(&eventlst); break;
            case 9: show_current_screen(screenlst); break;
            case 0: running = 0; break;
            default: printf("⚠️ Invalid option\n");
        }
    }

    free(screenlst.list_screen);
    free(eventlst.list_event);
    screenlst.list_screen = NULL;
    eventlst.list_event = NULL;

    printf("🚗 UI system terminated.\n");
    return 0;
}
