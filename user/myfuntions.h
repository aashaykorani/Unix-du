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

char* strcat(char* s1, const char* s2)
{
  char* b = s1;

  while (*s1) ++s1;
  while (*s2) *s1++ = *s2++;
  *s1 = 0;

  return b;
}

// char* strcat(char* destination, const char* source)
// {
//     // make `ptr` point to the end of the destination string
//     char* ptr = destination + strlen(destination);
 
//     // appends characters of the source to the destination string
//     while (*source != '\0') {
//         *ptr++ = *source++;
//     }
 
//     // null terminate destination string
//     *ptr = '\0';
 
//     // the destination is returned by standard `strcat()`
//     return destination;
// }

char* strstr(const char* string, const char* substring)
{
	const char *a, *b;

	/* First scan quickly through the two strings looking for a
	 * single-character match.  When it's found, then compare the
	 * rest of the substring.
	 */

	b = substring;

	if(*b == 0)
	{
		return (char*)string;
	}

	for(; *string != 0; string += 1)
	{
		if(*string != *b)
		{
			continue;
		}

		a = string;

		while(1)
		{
			if(*b == 0)
			{
				return (char*)string;
			}
			if(*a++ != *b++)
			{
				break;
			}
		}

		b = substring;
	}

	return ;
}