/*
** exec_broadcast_action.c for Zappy in /home/prost_m/Rendu/Semestre_4/Zappy/server/src/
**
** Made by Matthias Prost
** Login   <matthias.prost@epitech.eu@epitech.eu>
**
** Started on  Fri Jun 30 16:23:06 2017 Matthias Prost
** Last update Sun Jul  2 22:02:01 2017 Matthias Prost
*/

#include "server.h"

double		distance_users(t_users *userA, t_users *userB)
{
  double	distance;

  distance = sqrt((pow(((double)userB->posX - (double)userA->posX), 2)
		   + pow(((double)userB->posY - (double)userA->posY), 2)));
  return (distance);
}

void		fill_distance(t_distance *user_distance)
{
  int		i;

  i = -1;
  while (++i != MAX_FD)
    {
      user_distance[i].distance = -1;
      user_distance[i].user = NULL;
    }
}

void		bubble_sort(t_distance *user_distance, t_users *user,
			    t_env *env, char **msg)
{
  int		i;
  double	buffer;
  int		broad;
  t_distance	*user_dest;

  i = -1;
  user_dest = NULL;
  buffer = 10000;
  while (++i != MAX_FD)
    {
      if (user_distance[i].user != NULL && user_distance[i].user->socket != -1
	  && user_distance[i].distance != -1
	  && user_distance[i].user->socket != user->socket &&
	  user_distance[i].distance < buffer)
	{
	  buffer = user_distance[i].distance;
	  user_dest = &user_distance[i];
	}
    }
  if (user_dest != NULL  && user_dest->user->socket != -1
      && user->socket != -1 && strcmp(msg[0], "Fork") != 0)
    {
      delete_all_fd_actions(env, user_dest->user->socket);
      broad = broadcast(user, user_dest->user, env);
      dprintf(user_dest->user->socket, "message %d\n",
              broad);
      printf("--> Sent: \"message %d\" to socket %d\n", broad,
	     user_dest->user->socket);
      user_dest->user = NULL;
      user_dest->distance = -1;
    }
}

void		sendBroadCast(t_env *env, t_distance *dist,
			      t_users *user, char **msg)
{
  int		value;
  int		i;

  value = 0;
  if (user->lvl == 1)
    value = 1;
  else if (user->lvl == 2)
    value = 2;
  else if (user->lvl == 3)
    value = 2;
  else if (user->lvl == 4)
    value = 4;
  else if (user->lvl == 5)
    value = 4;
  else if (user->lvl == 6)
    value = 6;
  else if (user->lvl == 7)
    value = 6;
  i = -1;
  while (++i != value)
    bubble_sort(dist, user, env, msg);
}

void		broadcastAction(t_env *env, char **msg, t_users *user)
{
  int		i;
  int		a;
  t_distance	user_distance[MAX_FD];

  (void)msg;
  i = -1;
  a = 0;
  fill_distance(&user_distance[0]);
  if (user->socket != -1)
    {
      while (++i != MAX_FD)
	{
	  if (env->users[i].lock == false && env->users[i].socket != -1
	      && env->users[i].socket != user->socket
	      && env->users[i].teamName != NULL
	      && user->teamName && env->users[i].lvl == user->lvl)
            {
              printf("Distance between socket %d and socket %d: %lf\n",
		     user->socket, env->users[i].socket,
		     distance_users(user, &env->users[i]));
              user_distance[a].user = xmalloc(sizeof(t_users));
              memcpy(user_distance[a].user, &env->users[i], sizeof(t_users));
              user_distance[a].distance = distance_users(user, &env->users[i]);
              a++;
            }
	}
      if (a > 0)
	sendBroadCast(env, &user_distance[0], user, msg);
      dprintf(user->socket, "ok\n");
      printf("--> Sent: \"ok\" to socket %d\n", user->socket);
    }
}
