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

#include <chttps/util.h>

char* chttps_err_str(chttps_error err)
{
  switch(err)
    {
    case CHTTPS_NO_ERROR:
      return "CHTTPS_NO_ERROR";
    case CHTTPS_NULL_CONF_ERROR:
      return "CHTTPS_NULL_CONF_ERROR";
    case CHTTPS_IP_CONVERSION_ERROR:
      return "CHTTPS_IP_CONVERSION_ERROR";
    case CHTTPS_SERVER_IS_NULL_ERROR:
      return "CHTTPS_SERVER_IS_NULL_ERROR";
    case CHTTPS_CLIENT_IS_NULL_ERROR:
      return "CHTTPS_CLIENT_IS_NULL_ERROR";
    case CHTTPS_CONNECTIONS_IS_NULL_ERROR:
      return "CHTTPS_CONNECTIONS_IS_NULL_ERROR";
    case CHTTPS_CONNECTION_IS_NULL_ERROR:
      return "CHTTPS_CONNECTION_IS_NULL_ERROR";
    case CHTTPS_CONNECTIONS_FULL_ERROR:
      return "CHTTPS_CONNECTIONS_FULL_ERROR";
    case CHTTPS_PTHREAD_ATTR_INIT_ERROR:
      return "CHTTPS_PTHREAD_ATTR_INIT_ERROR";
    case CHTTPS_PTHREAD_CREATE_ERROR:
      return "CHTTPS_PTHREAD_CREATE_ERROR";
    case CHTTPS_PTHREAD_JOIN_ERROR:
      return "CHTTPS_PTHREAD_JOIN_ERROR";
    case CHTTPS_PTHREAD_CANCEL_ERROR:
      return "CHTTPS_PTHREAD_CANCEL_ERROR";
    case CHTTPS_PARSE_WRONG_METHOD_ERROR:
      return "CHTTPS_PARSE_WRONG_METHOD_ERROR";
    case CHTTPS_PARSE_WRONG_URI_ERROR:
      return "CHTTPS_PARSE_WRONG_URI_ERROR";
    case CHTTPS_PARSE_WRONG_VERSION_ERROR:
      return "CHTTPS_PARSE_WRONG_VERSION_ERROR";
    case CHTTPS_REMOVE_CONNECTION_ERROR:
      return "CHTTPS_REMOVE_CONNECTION_ERROR";
    case CHTTPS_SOCKET_ERROR:
      return "CHTTPS_SOCKET_ERROR";
    case CHTTPS_BIND_ERROR:
      return "CHTTPS_BIND_ERROR";
    case CHTTPS_LISTEN_ERROR:
      return "CHTTPS_LISTEN_ERROR";
    case CHTTPS_ACCEPT_CONNECTION_ERROR:
      return "CHTTPS_ACCEPT_CONNECTION_ERROR";
    case CHTTPS_CLOSE_SERVER_SOCKET_ERROR:
      return "CHTTPS_CLOSE_SERVER_SOCKET_ERROR";
    default:
      return "ERROR_NOT_RECOGNISED";
    }
}

char* chttps_log_level_str(chttps_log_level level)
{
  switch(level)
    {
    case CHTTPS_DEBUG:
      return "DEBUG";
    case CHTTPS_INFO:
      return "INFO";
    case CHTTPS_WARN:
      return "WARNING";
    case CHTTPS_ERR:
      return "ERROR";
    case CHTTPS_OUT:
      return "OUTPUT";
    default:
      return "UNKNOWN_LOG_LEVEL";
    }
}

chttps_config chttps_config_default()
{
  chttps_config conf = {
    .listen_ip              = "0.0.0.0",
    .port                   = 1234,
    .log_level              = CHTTPS_INFO,
    .waiting_queue_size     = 64,
    .max_connections        = 1024,
  };
  return conf;
}
