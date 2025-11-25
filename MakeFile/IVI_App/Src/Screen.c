#include "Screen.h"

static int count = 0;

void Init_ListScreen(Screen_Stack* screen,int max_screen){
    screen->list_screen = (char**)malloc(max_screen * sizeof(char*)); 
    if(screen->list_screen == NULL){
        printf("không đủ bộ nhớ cấp phát");
        return;
    }
    screen->size = max_screen;
    screen->top = -1;
}

static bool IsScreenfull(Screen_Stack screen){
    return (screen.top == screen.size - 1);
}
static bool IsScreenEmpty(Screen_Stack screen){
    return (screen.top == -1);
}
static void map_screen(const char* screen) {
    printf("[UI] %s Screen\n", screen);
    if (strcmp(screen, "Home") == 0) {
        printf("  - Audio\n  - Bluetooth\n  - Settings\n");
    } else if (strcmp(screen, "Audio") == 0) {
        printf("  - Now Playing\n  - Volume Control\n");
    } else if (strcmp(screen, "Bluetooth") == 0) {
        printf("  - Connected Devices\n  - Scan\n");
    } else if (strcmp(screen, "Settings") == 0) {
        printf("  - Display\n  - Network\n  - Time\n");
    } else {
        printf("  - Unknown screen\n");
    }
}

void show_current_screen(Screen_Stack screen_stack) {
    if (!IsScreenEmpty(screen_stack)) {
        printf("Currently at: %s\n", screen_stack.list_screen[screen_stack.top]);
        map_screen(screen_stack.list_screen[screen_stack.top]);
    } else {
        printf("At Main Menu\n");
    }
}

void push_screen(Screen_Stack* screen_stack,const char* screen_name) {
    if (!IsScreenfull(*screen_stack)) {

        int i = ++screen_stack->top;

        //cấp phát heap cho phần tử hiện tại
        screen_stack->list_screen[i] = (char*)malloc(strlen(screen_name) + 1); // '\0'
        if(screen_stack->list_screen[i] == NULL){
            printf("không đủ heap để lưu %s\n",screen_name);
            return;
        }

        //sao chép chuỗi vào heap
        strcpy(screen_stack->list_screen[i],screen_name);

        //in ra chuỗi sau khi sao chép
        printf("\n→ Navigated to: %s\n", screen_stack->list_screen[i]);

        //ánh xạ chuỗi đến màn hình hiển thị tương ứng
        map_screen(screen_stack->list_screen[i]);
    } else {
        printf("Stack full\n");
    }

    //cập nhật số lượng màn hình sau khi push vào stack
    count++;
    if(count <= (*screen_stack).size)    printf("total Screens : %d\n",count);
    else count = (*screen_stack).size;
}
void pop_screen(Screen_Stack* screen_stack) {
    if (!IsScreenEmpty(*screen_stack)) {

        //giải phóng chuỗi trỏ đến bởi con trỏ hiện tại trong mảng heap
        free(screen_stack->list_screen[screen_stack->top]);
        
        int top = --screen_stack->top;

        if (top > 0) {
            printf("\n <= Back to: %s\n", screen_stack->list_screen[top]); //hiển thị màn hình trước đó sau khi thoát khỏi screen hiện tại
            map_screen(screen_stack->list_screen[top]);     //hiển thị màn hình tương ứng với chuỗi được ánh xạ
        } else {
            printf("\n <= Back to: (Main Menu)\n");
        }
    } else {
        printf("No screen to go back\n");
    }

    //cập nhật số lượng màn hình còn lại sau khi pop khỏi stack
    count--;
    if(count >= 0)     printf("total Screens : %d\n",count);
    else count = 0;
}