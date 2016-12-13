/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ben_afk.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:31:41 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/13 13:40:27 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <poll.h>

#include <string>
#include <iostream>
#include "RSA.hpp"
