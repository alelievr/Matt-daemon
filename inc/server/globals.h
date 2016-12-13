/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 21:14:39 by root              #+#    #+#             */
/*   Updated: 2016/12/13 16:01:22 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define MAX_CONNECTION_QUEUE			3
#define	MAX_SIMULTANEOUS_CONNECTIONS	3
#define LOCK_FILE			"/var/lock/matt_daemon.lock"
#define	PID_FILE			"/var/run/matt-daemon.pid"
#define LOG_FILE_NAME		"matt_daemon.log"
#define LOG_FILE_NAME_C1	"client0.log"
#define LOG_FILE_NAME_C2	"client1.log"
#define LOG_FILE_NAME_C3	"client2.log"
#define LOG_FILE_NAME_C		{LOG_FILE_NAME_C1, LOG_FILE_NAME_C2, LOG_FILE_NAME_C3}
#define LOG_FILE_DIR		"/var/log/matt_daemon/"
