#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_CONNECTIONS 5

// Wait on any terminated child processes
void childHandler(int sig) {
    int oldErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }
    errno = oldErrno;
}

int main(int argc, char *argv[]) {
    int listen_fd, new_fd;
    int yes = 1;
    int ret_val;
    struct addrinfo hints, *server_info;
    struct sockaddr_storage peer_addr;
    socklen_t sin_size;
    //char s[INET6_ADDRSTRLEN];
    struct sigaction sigact;
    
    // Verify command line argument
    if (argc < 2 || 
        strcmp(argv[1], "--help") == 0 || 
        strcmp(argv[1], "-h") == 0) {
        fprintf(stderr, "usage: %s listening_port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // Zero-initialize and set addrinfo structure
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    // IPv4 or IPv6, whichever is available
    hints.ai_socktype = SOCK_STREAM;// TCP
    hints.ai_flags = AI_PASSIVE;    // Use local host's IP
    
    // Look up the local host's address info
    if ((ret_val = getaddrinfo(NULL, argv[1], &hints, &server_info)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret_val));
        exit(EXIT_FAILURE);
    }
    
    // Open a socket based on the local host's address info
    if ((listen_fd = socket(
                        server_info->ai_family,
                        server_info->ai_socktype,
                        server_info->ai_protocol
					)) < 0) {
        perror("server: socket");
    }
    
    // Attempt to reuse the socket if it's already in use
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, 
        sizeof(int)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // Bind the socket to the port
    if (bind(listen_fd, server_info->ai_addr, server_info->ai_addrlen) < 0) {
        close(listen_fd);
        perror("server: bind");
        exit(EXIT_FAILURE);
    }
    
    // Free memory used by local host's address info
    freeaddrinfo(server_info);
    
    // Listen on port for up to MAX_CONNECTIONS
    if (listen(listen_fd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // Set signal handler for cleaning up zombies
    sigact.sa_handler = childHandler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART;   // Restart system calls
    if (sigaction(SIGCHLD, &sigact, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    
    // Stay in accept loop until terminated by a KILL or STOP signal
    while(1) {
        sin_size = sizeof(peer_addr);
        new_fd = accept(listen_fd, (struct sockaddr *)&peer_addr, &sin_size);
        
        // If a connection fails, just print an error and continue
        if (new_fd < 0) {
            perror("accept");
            continue;
        }
        
        if (ret_val = fork() == 0) {
            // Child process doesn't need the listen socket
            close(listen_fd);
            // do stuff
            close(new_fd);
            exit(EXIT_SUCCESS);
        }
        else if (ret_val < 0) {
            // Fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        }
        
        // Parent process resumes here
        close(new_fd);
    }
    
    return 0;
}
