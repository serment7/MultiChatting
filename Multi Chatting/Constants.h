#pragma once

#define WINDOW_WIDTH (GetSystemMetrics(SM_CXSCREEN))
#define WINDOW_HEIGHT (GetSystemMetrics(SM_CYSCREEN))

#define SERVICE_TITLE "Multi Chatting"
#define USERNAME_MAX 16
#define BUFFER_MAX 256-USERNAME_MAX-1
#define CLIENT_MAX 7
#define PORT 2000
#define QUIT_CODE "/a quit"
#define SERVER_NUMBER 7