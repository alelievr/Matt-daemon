/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ben_afk.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 16:31:41 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/12 01:36:58 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <string>
#include <iostream>
#include <signal.h>
#include "RSA.hpp"

#define PROMPT	"\033[38;5;129m$> \033[38;5;159m"
#define STDOUT	"\033[38;5;193m"//remote> "
#define STDERR	"\033[38;5;197m"//remote> "
