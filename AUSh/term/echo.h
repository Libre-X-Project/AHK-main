#include "../../../monitor.h"

int echo(char* echoInput) {
    monitor_write(echoInput);
}