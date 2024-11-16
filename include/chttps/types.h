/* 
 * MIT License
 * 
 * Copyright (c) 2024 Giovanni Santini
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _CHTTPS_TYPES_H
#define _CHTTPS_TYPES_H

#include <netinet/in.h>  /* in_port_t   */
#include <sys/socket.h>  /* sockaddr_in */ 
#include <stdbool.h>     /* for bools   */

#define CHTTPS_MAX_STRING_SIZE 255
#define CHTTPS_MAX_ROUTES 255

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Tip: Convert the enum to a string with chttps_err_str()
 */
typedef enum 
{
  CHTTPS_NO_ERROR = 0,
  CHTTPS_NULL_CONF_ERROR,
  CHTTPS_SERVER_IS_NULL_ERROR,
  CHTTPS_CLIENT_IS_NULL_ERROR,
  CHTTPS_CONNECTIONS_IS_NULL_ERROR,
  CHTTPS_CONNECTION_IS_NULL_ERROR,
  CHTTPS_CONNECTIONS_FULL_ERROR,
  CHTTPS_PTHREAD_ATTR_INIT_ERROR,
  CHTTPS_PTHREAD_CREATE_ERROR,
  CHTTPS_PTHREAD_JOIN_ERROR,
  CHTTPS_PTHREAD_CANCEL_ERROR,
  CHTTPS_PARSE_WRONG_METHOD_ERROR,
  CHTTPS_PARSE_WRONG_URI_ERROR,
  CHTTPS_PARSE_WRONG_VERSION_ERROR,
  CHTTPS_RESPONSE_IS_NULL_ERROR,
  CHTTPS_ROUTE_IS_NULL_ERROR,
  CHTTPS_ROUTER_IS_NULL_ERROR,
  CHTTPS_ROUTER_FULL_ERROR,
  CHTTPS_ROUTER_NO_MATCH_ERROR,
  CHTTPS_REMOVE_CONNECTION_ERROR,
  CHTTPS_IP_CONVERSION_ERROR,
  CHTTPS_SOCKET_ERROR,
  CHTTPS_BIND_ERROR,
  CHTTPS_LISTEN_ERROR,
  CHTTPS_ACCEPT_CONNECTION_ERROR,
  CHTTPS_CLOSE_SERVER_SOCKET_ERROR,
  _CHTTPS_ERROR_MAX

} chttps_error;

/**
 * Tip: Convert the enum to a string with chttps_log_level_str()
 */
typedef enum
{
  CHTTPS_DEBUG = 0,
  CHTTPS_INFO,
  CHTTPS_WARN,
  CHTTPS_ERR,
  CHTTPS_OUT,
  CHTTPS_DISABLED,   /* This is guaranteed to not print anything */
  _CHTTPS_LOG_LEVEL_MAX  /* as well as this                          */

} chttps_log_level;

  
/* =========================================
 * HTTPS TYPES
 * ========================================= */

typedef enum
{
  CHTTPS_GET,
  CHTTPS_HEAD,
  CHTTPS_POST,
  _CHTTPS_REQUEST_TYPE_MAX,

} chttps_request_method;
  
typedef struct
{
  chttps_request_method method;
  char uri[CHTTPS_MAX_STRING_SIZE];
  char version[4];
  
} chttps_request_header;

/*
 * Example request line:
 * GET http://www.w3.org/pub/WWW/TheProject.html HTTP/1.0
 */
typedef struct
{
  chttps_request_header header;
  size_t body_len;
  char body[CHTTPS_MAX_STRING_SIZE];

} chttps_request;

/**
 * Note: use chttps_code_str() to convert the status code
 * to string.
 */
typedef struct
{
  /* RFC 1945
   * Status-Code    = "200"   ; OK
   *                | "201"   ; Created
   *                | "202"   ; Accepted
   *                | "204"   ; No Content
   *                | "301"   ; Moved Permanently
   *                | "302"   ; Moved Temporarily
   *                | "304"   ; Not Modified
   *                | "400"   ; Bad Request
   *                | "401"   ; Unauthorized
   *                | "403"   ; Forbidden
   *                | "404"   ; Not Found
   *                | "500"   ; Internal Server Error
   *                | "501"   ; Not Implemented
   *                | "502"   ; Bad Gateway
   *                | "503"   ; Service Unavailable
   */
  int status_code;
  char version[4];

} chttps_response_header;
  
typedef struct
{
  chttps_response_header header;
  size_t body_len;
  char body[CHTTPS_MAX_STRING_SIZE];

} chttps_response;

  
/* ========================================
 * ROUTES TYPES
 * ======================================== */
  
typedef struct
{
  char path[CHTTPS_MAX_STRING_SIZE];

  /* Functions called by the router based on the request */
  chttps_error (*get)(chttps_request *req, char *out);
  chttps_error (*head)(chttps_request *req, char *out);
  chttps_error (*post)(chttps_request *req, char *out);

} chttps_route;

typedef struct
{
  size_t registered_routes;
  chttps_route *routes[CHTTPS_MAX_ROUTES];

} chttps_router;

  
/* ====================================
 * CONNECTION TYPES
 * ==================================== */
  
typedef uint16_t chttps_port_t; 

/* Alias of sockaddr
 *  struct sockaddr {
 *    sa_family_t sa_family; // Address family
 *    char        sa_data[]; // Socket address
 *  };
 */
typedef struct sockaddr chttps_addr;

/**
 * Configuration for the server.
 * This struct is stored inside the server after
 * chttps_server_init() is called. Lots of functions
 * will change their behaviour based on those configs.
 */
typedef struct
{
  const char* listen_ip;       /* The IP to listen from */
  chttps_port_t port;          /* Port to listen from   */
  chttps_log_level log_level;  /* Output log level */
  int waiting_queue_size;      /* Waiting queue for new connections */
  size_t max_connections;      /* Maximum number of open connections */

} chttps_config;

/**
 * Struct containing a single client instance
 */
typedef struct
{
  int cfd;                     /* Client file descriptor */
  chttps_addr addr;            /* Client address struct  */

} chttps_client;

/**
 * This struct contains an array of connected clients
 * and another array of booleans to know if a position
 * in the client's array is available to use.
 */
typedef struct
{
  size_t len;                  /* The len of the two arrays */
  chttps_client **clients;
  pthread_t *threads;          /* Thread ID of each connection */
  bool *is_available;

} chttps_connections;

/**
 * Struct containing the server instance
 */
typedef struct
{
  int sfd;                     /* Server file descriptor */
  chttps_config conf;          /* Server configuration   */
  chttps_connections connections; /* List of active/inactive connections */
  chttps_router router;

} chttps_server;
  
#ifdef __cplusplus
}
#endif

#endif
