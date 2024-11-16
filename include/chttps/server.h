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

#include <pthread.h>       /* Mutexes */

#include <chttps/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Itinitalize the server for listening. You
 * can pass an optional configuration pointer or
 * a default value will be used (chttps_config_default()).
 * Before passing a custom configuration, it is advised
 * to initialize the default one and change the
 * values that you want to change.
 */
chttps_error chttps_server_init(chttps_server *server,
                                chttps_config *conf);
/**
 * Listen for client connection with blocking and return
 * an allocated client struct. Remember to deallocate
 * this when the session has ended.
 */
chttps_error chttps_server_listen(chttps_server *server,
		  		  chttps_client **client);
/**
 * Remember to close the server or nightmares
 * will hunt you.
 */
chttps_error chttps_server_close(chttps_server *server);

/**
 * Getter and setter for the global boolean
 * variable "stop" and Its mutex. When "stop" is
 * set to true, all loops will safely end
 * their execution and correctly terminate the
 * program.
 * Note: set_stop is thread safe
 */
pthread_mutex_t *chttps_get_stop_mutex();
bool *chttps_get_stop();
void chttps_set_stop(bool stop);
  
#ifdef __cplusplus
}
#endif
  
#endif
