/**
* @file server.h
* @brief Main server module.
*
* @author Piotr J. Tomaszewski
*/

#ifndef CHECKERSSERVER_SERVER_H
#define CHECKERSSERVER_SERVER_H

/**
 * Initializes the game server and starts the server loop.
 * @param port A port on which the server should listen.
 */
void server_run(int port);
#endif //CHECKERSSERVER_SERVER_H
