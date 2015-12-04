#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <sql.h>

#define BUF_SIZE 5000
using namespace std;
    int
main(int argc, char *argv[])
{
    SQL database;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];
    string resp;
    std::ofstream myfile;
    myfile.open ("example.txt");

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully bind(2).
       If socket(2) (or bind(2)) fails, we (close the socket
       and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */

        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    /* Read datagrams and echo them back to sender */
    int count = 0;
    for (;;) {
        peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                (struct sockaddr *) &peer_addr, &peer_addr_len);
        if (nread == -1)
            continue;               /* Ignore failed request */

        char host[NI_MAXHOST], service[NI_MAXSERV];

        s = getnameinfo((struct sockaddr *) &peer_addr,
                peer_addr_len, host, NI_MAXHOST,
                service, NI_MAXSERV, NI_NUMERICSERV);
        if (s == 0)
        {
            string msg = string(buf);
            string delimiter = ";";
            size_t pos = 0;
            string token;
            //cout << msg.substr(0, msg.find(delimiter)) << endl;
            int command = stoi(msg.substr(0, msg.find(delimiter)));
            msg.erase(0, msg.find(delimiter) + delimiter.length());
            string msg_id = msg.substr(0, msg.find(delimiter));
            msg.erase(0, msg.find(delimiter) + delimiter.length());
            if (command == 1) {         //read
                string user_id = msg;
                resp=database.database_retrieve(msg_id, user_id);
            }
            if (command == 0) {         //write
                string encrypted_msg = msg.substr(0, msg.find(delimiter));
                msg.erase(0, msg.find(delimiter) + delimiter.length());
                string readers=msg;
                database.database_insert(msg_id, encrypted_msg, readers);
                resp="SUCCESS";

            }
            
            //count++;
            memset(buf, 0, sizeof buf);

        }
        else
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

        if (sendto(sfd, resp.data(), resp.size(), 0,
                    (struct sockaddr *) &peer_addr,
                    peer_addr_len) != resp.size())
            fprintf(stderr, "Error sending response\n");
        if ( count > 4 ) break;
    }
    myfile.close();
    return 0;
}
