/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:01:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/08 13:38:15 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#pragma clang diagnostic ignored "-Wc++98-compat"

#include <string>
#include <iostream>
#include <fstream>

#define LOGFILE		"/var/log/matt_daemon/matt_daemon.log"

class		Tintin_reporter
{
	private:
		std::ostream		_logStream;

	public:
		Tintin_reporter(void);
		Tintin_reporter(const Tintin_reporter&) = delete;
		virtual ~Tintin_reporter(void);

		Tintin_reporter &	operator=(Tintin_reporter const & src) = delete;

		int					writeToLog(const std::string & message) const;
};

std::ostream &	operator<<(std::ostream & o, Tintin_reporter const & r);
