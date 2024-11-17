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

#include <pthread.h>   /* UNIX threads */

#include <chttps/connections.h>

/* Note: chttps_connections is defined in include/chttps/utils. */

chttps_connections chttps_connections_init(const size_t max_connections)
{
  chttps_client **clients = malloc(sizeof(chttps_connections*)*max_connections);
  pthread_t *threads = malloc(sizeof(pthread_t) * max_connections);
  bool *is_available = malloc(sizeof(bool) * max_connections);
  for (size_t i = 0; i < max_connections; ++i)
    {
      is_available[i] = true;
      threads[i] = 0;
    }
  chttps_connections connections = {
    .len            = max_connections,
    .clients        = clients,
    .threads        = threads,
    .is_available   = is_available,
  };
  return connections;
}

chttps_error chttps_connections_free(chttps_connections *connections)
{
  if (connections == NULL || connections->clients == NULL
      || connections->is_available == NULL
      || connections->threads == NULL)
    return -CHTTPS_CONNECTIONS_IS_NULL_ERROR;

  for (size_t i = 0; i < connections->len; ++i)
      if (!connections->is_available[i])
	{
	  if (pthread_cancel(connections->threads[i]))
            return -CHTTPS_PTHREAD_CANCEL_ERROR;
	    
	  if (pthread_join(connections->threads[i], NULL))
	    return -CHTTPS_PTHREAD_JOIN_ERROR;
	}

  for (size_t i = 0; i < connections->len; ++i)
      if (!connections->is_available[i])
	free(connections->clients[i]);

  free(connections->clients);
  free(connections->is_available);
  free(connections->threads);
  return CHTTPS_NO_ERROR;
}

chttps_error chttps_add_connection(chttps_server *server,
				   chttps_client *client)
{
  chttps_debug("Adding a connection", &(server->conf));
  chttps_connections *connections = &(server->connections);
  if (connections == NULL || connections->is_available == NULL
      || connections->clients == NULL || client == NULL
      || connections->threads == NULL)
    return -CHTTPS_CONNECTION_IS_NULL_ERROR;

  chttps_debug("  parameters are not null", &(server->conf));
  for (size_t i = 0; i < connections->len; ++i)
      if (connections->is_available[i])
	{
	  chttps_debug("  found an available slot", &(server->conf));
	  connections->clients[i] = client;
	  connections->is_available[i] = false;

	  /* Join thread if there were one */
	  if (connections->threads[i])
	    if (pthread_join(connections->threads[i], NULL))
	      return -CHTTPS_PTHREAD_JOIN_ERROR;
	  chttps_debug("  joined old thread", &(server->conf));
	  
	  /* Spawn new thread */
          int err;
          pthread_attr_t attr;
          err = pthread_attr_init(&attr);
          if (err)
	    return -CHTTPS_PTHREAD_ATTR_INIT_ERROR;
	  chttps_thread_info *info_ptr = malloc(sizeof(chttps_thread_info));
	  chttps_thread_info info = {
	    .server = server,
	    .client = client,
	  };
	  *info_ptr = info;
	  chttps_debug("  info ptr created", &(server->conf));

          pthread_t tid;
          err = pthread_create(&tid, &attr,
			       &chttps_connection_thread, info_ptr);
          if (err)
	    return -CHTTPS_PTHREAD_CREATE_ERROR;
	  connections->threads[i] = tid;

	  chttps_debug("  all done succesfully", &(server->conf));
          return CHTTPS_NO_ERROR;
	}

  return -CHTTPS_CONNECTIONS_FULL_ERROR;
}

chttps_error chttps_remove_connection(chttps_connections *connections,
				      chttps_client *client)
{
  if (connections == NULL || connections->is_available == NULL
      || connections->clients == NULL || client == NULL
      || connections->threads == NULL)
    return -CHTTPS_CONNECTION_IS_NULL_ERROR;

  for (size_t i = 0; i < connections->len; ++i)
      if (!connections->is_available)
	  if (connections->clients[i]->cfd == client->cfd)
	    {
	      free(connections->clients[i]);
	      connections->is_available[i] = true;
	      return CHTTPS_NO_ERROR;
	    }

  return -CHTTPS_REMOVE_CONNECTION_ERROR;
}
