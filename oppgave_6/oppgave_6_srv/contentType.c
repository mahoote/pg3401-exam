#include <string.h>

#include "include/contentType.h"

char *getContentType(char *pszFilePath)
{
    if (strstr(pszFilePath, ".html") != NULL)
    {
        return "text/html";
    }
    else if (strstr(pszFilePath, ".jpg") != NULL)
    {
        return "image/jpeg";
    }
    else if (strstr(pszFilePath, ".png") != NULL)
    {
        return "image/png";
    }
    else if (strstr(pszFilePath, ".gif") != NULL)
    {
        return "image/gif";
    }
    else
    {
        return "text/plain";
    }
}