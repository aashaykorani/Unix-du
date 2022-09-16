#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"

int isdigit(int c)
{
	return (c >= '0' && c <= '9' ? 1 : 0);
}

int isNumber(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    return 1;
}

// char* strcat(char* s1, const char* s2)
// {
//   char* b = s1;

//   while (*s1) ++s1;
//   while (*s2) *s1++ = *s2++;
//   *s1 = 0;

//   return b;
// }

char* strcat(char* destination, const char* source)
{
    // make `ptr` point to the end of the destination string
    char* ptr = destination + strlen(destination);
 
    // appends characters of the source to the destination string
    while (*source != '\0') {
        *ptr++ = *source++;
    }
 
    // null terminate destination string
    *ptr = '\0';
 
    // the destination is returned by standard `strcat()`
    return destination;
}