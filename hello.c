#include <stdio.h>
#include <winsock2.h>

WSADATA WSAData;
SOCKET sock, acceptSocket;
int SOCKET_VERSION = 0x0101;
int PORT = 1000;
int MAX_CLIENTS = 1;
int sockAddrInLength = sizeof(struct sockaddr_in);
int running = 1;
struct sockaddr_in server, client;

int initWSAStartup() {
  if(WSAStartup(SOCKET_VERSION, &WSAData) != 0) {
      printf("WSAStartup: failed\n");
      return 0;
  }

  printf("WSAStartup: success\n");
}

int initSocket() {
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1) {
    printf("Socket: failed\n");
    return 0;
  }

  printf("Socket: success\n");
}

int initBindSocket() {
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);

  if (bind(sock, (struct sockaddr*) &server, sizeof server) == SOCKET_ERROR) {
    printf("Binding socket: failed\n");
    return 0;
  }

  printf("Binding socket: success\n");
}

int initListenSocket() {
  listen(sock, MAX_CLIENTS);
  printf("Waiting for a connection...\n");
}

int initAcceptSocket() {
  acceptSocket = accept(sock, (struct sockaddr*) &client, &sockAddrInLength);
  
  if (acceptSocket == INVALID_SOCKET) {
      printf("Accept: failed.\n");
      return 0;
  }

  printf("Accept: success\n");
}

int initSendMessage() {
  FILE *htmlFile = fopen("index.html", "r");
  char myString[1000];
  char message[1000];

  int intChar;

  strcpy(myString, "");

  do {
    intChar = fgetc(htmlFile);
    char s = (char)(intChar);

    strcat(myString, &s);

  } while (intChar != EOF);

  int x = strlen(myString) - 1;
  char buffer[sizeof(int) * 4 + 1];
  sprintf(buffer, "%d", x);

  strcpy(message, "HTTP/1.1 200 OK\r\n");
  strcat(message, "Server: C\r\n");
  strcat(message, "Content-Type: text/html\r\n");
  strcat(message, "Content-Length: ");
  strcat(message, buffer);
  strcat(message, "\r\n\r\n");
  strcat(message, myString);

  
  if (send(acceptSocket, message, strlen(message), 0) < 0)  {
      printf("Send: failed\n");
      return 0;
  }

  printf("Send: success\n");
}

int main(void) {
  initWSAStartup();
  initSocket();
  initBindSocket();
  
  while(running) {
    initListenSocket();
    initAcceptSocket();
    initSendMessage();  
  }

  return 0;
}