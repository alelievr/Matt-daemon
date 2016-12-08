/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt_daemon.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 12:48:49 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/08 21:16:04 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "globals.h"

extern "C" void		matt_daemon(int lock_fd) __attribute__((noreturn));
