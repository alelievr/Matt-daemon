/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:03:10 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/09 04:52:26 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"
#include <sys/stat.h>
#include "globals.h"
#include <string.h>

std::ofstream	*Tintin_reporter::_logStream = NULL;

int		Tintin_reporter::Log(const std::string & message)
{
	return _InternalLog("LOG", message);
}

int		Tintin_reporter::LogError(const std::string & message)
{
	return _InternalLog("ERROR", message);
}

int		Tintin_reporter::LogInfo(const std::string & message)
{
	return _InternalLog("INFO", message);
}

int		Tintin_reporter::_InternalLog(const char *type, const std::string & message)
{
	char		buff[0xF00];
	time_t		rawtime;
	size_t		size;
	struct tm	*timeinfo;
	std::string	mes(message);

	if (Tintin_reporter::_logStream == NULL)
		return (-1);
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	size = strftime(buff, sizeof(buff), "[%d/%m/%Y-%H:%M:%S] ", timeinfo);
	std::replace(mes.begin(), mes.end(), '\n', '$');
	(*Tintin_reporter::_logStream) << buff;
	(*Tintin_reporter::_logStream) << std::string("[ ") + type + " ] ";
	(*Tintin_reporter::_logStream) << mes << std::endl;
	return (0);
}

void		Tintin_reporter::Init(void)
{
	Tintin_reporter::_logStream = new std::ofstream;
	struct stat		st;
	stat(LOG_FILE_DIR, &st);
	if (!S_ISDIR(st.st_mode))
		mkdir(LOG_FILE_DIR, 0755);
	Tintin_reporter::_logStream->open(LOG_FILE_DIR LOG_FILE_NAME);
	if (errno != 0)
	{
		Tintin_reporter::_logStream->close();
		Tintin_reporter::_logStream->open(LOG_FILE_DIR LOG_FILE_NAME);
		if (errno != 0)
			perror("open log file"), exit(-1);
	}
}

void		Tintin_reporter::DeInit(void)
{
	Tintin_reporter::_logStream->close();
	delete Tintin_reporter::_logStream;
}
