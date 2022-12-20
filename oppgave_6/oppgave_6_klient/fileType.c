#include <string.h>

#include "include/fileType.h"

/* getContentType() ---------------------------------
    Revision    : 1.0.0

    Comments:
    Returns the correct Content-Type based on the filepath.
   -------------------------------------------------- */

char *getFileType(char *pszContentType)
{
    if (strstr(pszContentType, "text/html") != NULL)
    {
        return "html";
    }
    else
    {
        return "txt";
    }
}