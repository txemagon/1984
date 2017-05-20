#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

#include "image_file.h"

#define PORT 3550
#define BACKLOG 2
#define MAXBUFF 4096
#define MAX_STR 0x100

const char *prog_name = "photo_keeper";
const char *default_filename =  "default_received.png";

static void
cleanup_child_keeper(int signal_number) {
  int status;
  wait(&status);
}

static void
handle(int connection, const char * filename) {
  char buffer[MAXBUFF];
  char *image = NULL;
  size_t size = 0;
  ssize_t bytes_read;
  FILE *pf = NULL;

  do {
    bytes_read = read(connection, buffer, sizeof(buffer));
    if (bytes_read > 0){
      image = (char *) realloc(image, size+bytes_read);
      memcpy(buffer, image+size, bytes_read) ;
      size += bytes_read;
      fprintf(stderr, "received %li bytes. Size: %li\n", bytes_read, size);
    }
  }while (bytes_read > 0);
  fprintf(stderr, "total: %li bytes - ", size);
  pf = fopen (filename, "wb");
  fwrite(image, sizeof(char), size, pf);
  fclose(pf);
  free(image);
}

int
main ()
{

  struct sigaction sigchld_action;
  int socket_fd, connection;
  struct sockaddr_in server;
  struct sockaddr_in client;
  socklen_t sin_size;
  char filename[MAX_STR];
  pid_t child_pid;

  bzero(&sigchld_action, sizeof(sigchld_action));
  sigchld_action.sa_handler = &cleanup_child_keeper;
  sigaction(SIGCHLD, &sigchld_action, NULL);
  strncpy (filename, default_filename, MAX_STR);
  socket_fd = socket (AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  server.sin_port = htons (PORT);
  server.sin_addr.s_addr = INADDR_ANY;
  bzero (&(server.sin_zero), 8);

  bind (socket_fd, (struct sockaddr *) &server, sizeof (struct sockaddr));

  listen (socket_fd, BACKLOG);

  fprintf (stderr, "%s up and running.\n", prog_name);
  while (connection != -1)
  {
    sin_size = sizeof (struct sockaddr_in);
    connection = accept (socket_fd, (struct sockaddr *) &client, &sin_size);
    child_pid = fork();
    if (child_pid == 0){
      close(STDIN_FILENO);
      close(STDOUT_FILENO);
      close(socket_fd);
      handle(connection, filename);
      close(connection);
      exit(0);
    } else
    close(connection);
    strcpy(filename, "vacio.png");
  }
  printf("\n");
  close (socket_fd);
  return EXIT_SUCCESS;
}
