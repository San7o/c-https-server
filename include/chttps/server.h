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

#ifndef _CHTTPS_SERVER_H
#define _CHTTPS_SERVER_H

#include <chttps/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Itinitalize the server for listening. You
 * can pass an optional configuration pointer or
 * a default value will be used (chttps_config_default()).
 * Before passing a custom configuration, it is advised
 * to initialize the default one and change the
 * values that you want to change.
 */
chttps_error chttps_server_init(chttps_server *server,
                                chttps_config *conf);
/*
 * Listen for client connection and return an allocated
 * client struct. Remember to deallocate this when
 * the session has ended.
 */
chttps_error chttps_server_listen(chttps_server *server,
		  		  chttps_client **client);
/*
 * Remember to close the server or nightmares
 * will hunt you.
 */
chttps_error chttps_server_close(chttps_server *server);

#ifdef __cplusplus
}
#endif
  
#endif
