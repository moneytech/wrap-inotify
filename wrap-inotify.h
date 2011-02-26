/*
 * wrap inotify, mainly implement recursively monitoring, header file
 *
 * Copyright (c) 2010, 2011 lxd <edl.eppc@gmail.com>
 * 
 * This file is part of File Synchronization System(fss).
 *
 * fss is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, 
 * (at your option) any later version.
 *
 * fss is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with fss.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _WRAP_INOTIFY_H_
#define _WRAP_INOTIFY_H_

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_PATH_LEN 1024
#define BUF_LEN 4096

#define FIFO0 "/tmp/fifo-wrap-inotify.0"
#define FIFO1 "/tmp/fifo-wrap-inotify.1"

extern int errno;

// monitoring STRUCT per directory
typedef struct monitor
{
  char *pathname;
  int fd;
  int wd;

  struct monitor *p;  //previous one
  struct monitor *n;  //next one, as u c, it is a doubly linked list
} monitor;


int rfd, wfd;
pid_t pid;

static char fullpath[MAX_PATH_LEN];
static fd_set set;
int maxfd;

/* all sub-dirs gonna be monitored under same inotify mask */
static uint32_t mask;

/* point to current monitor of ddlink,
 * after recurse iterator, it point to tail of list */
static monitor *tail_monitor;

/* API */
int monitors_init(const char *root_path, uint32_t mask, int *fd);
int monitors_cleanup();

static int monitors_poll();

static int verify_path(const char *path);
static int strip_path(const char *path, char *p);
static int join_fname(const monitor *this_monitor,
		      const char *fname);
static int monitor_connect(const char *path);
static int monitor_connect_via_fpath(const monitor *this_monitor,
				   const char *fname);
static int monitor_disconnect(monitor *this_monitor);
static int monitor_disconnect_via_fpath(const monitor *this_monitor,
				      const char *fname);
static int deepin(char *path);  //recursively called

#endif