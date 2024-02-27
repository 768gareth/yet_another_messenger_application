#include "utils.h"

char* get_current_time();

// Return the current UTC timestamp as a string (e.g "HH:MM:SS UTC").
char* get_current_time()
{
    time_t current_time;
    struct tm *local_time;

    time(&current_time);
    local_time = gmtime(&current_time);

    char *timestamp_output = (char*)malloc(14 * sizeof(char));
    sprintf(timestamp_output, "%02d:%02d:%02d UTC", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
    
    return timestamp_output;
}