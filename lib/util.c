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

char* chttps_err_str(CHTTPS_ERROR err)
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
