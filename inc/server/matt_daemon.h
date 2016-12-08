/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt_daemon.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 12:48:49 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/08 21:14:57 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/file.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "globals.h"

#define LOCK_FILE	"/var/lock/matt_daemon.lock"
#define	PID_FILE	"/var/run/matt-daemon.pid"

void		matt_daemon(int lock_fd) __attribute__((noreturn));
