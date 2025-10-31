#include <stdio.h>
#include <stdarg.h>

typedef enum
{
    TURN_ON,
    TURN_OFF,
    SET_LEVEL,
    SEND_MESSAGE
} CommandType;

void sendCommand(CommandType command, ...)
{
    va_list args;
    va_start(args, command);

    switch (command)
    {
        case TURN_ON:
        case TURN_OFF:
        {
            int deviceID = va_arg(args, int);
            printf("Command: %s Device ID: %d\n", command == TURN_ON ? "Turn On" : "Turn Off", deviceID);
            break;
        }

        case SET_LEVEL:
        {
            int deviceID = va_arg(args, int);
            int level = va_arg(args, int);
            printf("Set Level of Device ID %d to %d\n", deviceID, level);
            break;
        }

        case SEND_MESSAGE:
        {
            char* message = va_arg(args, char*);
            printf("Send Message: %s\n", message);
            break;
        }
    }
    va_end(args);
}

int main()
{
    sendCommand(TURN_ON, 1);
    sendCommand(TURN_OFF, 2);
    sendCommand(SET_LEVEL, 3, 75);
    sendCommand(SEND_MESSAGE, "Hello World");
    return 0;
}

