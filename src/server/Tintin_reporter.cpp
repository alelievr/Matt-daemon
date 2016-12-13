/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:03:10 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/13 16:16:24 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"
#include <sys/stat.h>
#include "globals.h"
#include <string.h>

std::ofstream	*Tintin_reporter::_logStream = NULL;
std::ofstream	*Tintin_reporter::_clientStream = {NULL};

int		Tintin_reporter::Log(const std::string & message)
{
	return _InternalLog(_logStream, "LOG", message);
}

int		Tintin_reporter::LogError(const std::string & message)
{
	return _InternalLog(_logStream, "ERROR", message);
}

int		Tintin_reporter::LogInfo(const std::string & message)
{
	return _InternalLog(_logStream, "INFO", message);
}

int		Tintin_reporter::LogClient(const int clientNumber, const std::string & message)
{
	if (clientNumber >= 0 && clientNumber <= 2)
		return (_InternalLog(_clientStream + clientNumber, "LOG", message));
	return (-1);
}

int		Tintin_reporter::_InternalLog(std::ofstream * stream, const char *type, const std::string & message)
{
	char		buff[0xF00];
	time_t		rawtime;
	size_t		size;
	struct tm	*timeinfo;
	std::string	mes(message);

	if (stream == NULL)
		return (-1);
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	size = strftime(buff, sizeof(buff), "[%d/%m/%Y-%H:%M:%S] ", timeinfo);
	std::replace(mes.begin(), mes.end(), '\n', '$');
	(*stream) << buff;
	(*stream) << std::string("[ ") + type + " ] ";
	(*stream) << mes << std::endl;
	return (0);
}

void		Tintin_reporter::_OpenStream(std::ofstream & stream, const char *file, const std::ios_base::openmode mode)
{
	stream.open(file, mode);
	if (errno != 0)
	{
		stream.close();
		stream.open(file, mode);
		if (errno != 0)
			perror("open log file"), exit(-1);
	}
}

void		Tintin_reporter::Init(void)
{
	Tintin_reporter::_logStream = new std::ofstream;
	Tintin_reporter::_clientStream = new std::ofstream[3];
	struct stat		st;
	stat(LOG_FILE_DIR, &st);
	if (!S_ISDIR(st.st_mode))
		mkdir(LOG_FILE_DIR, 0755);
	Tintin_reporter::_OpenStream(*_logStream, LOG_FILE_DIR LOG_FILE_NAME, std::fstream::in | std::fstream::app);
	Tintin_reporter::_OpenStream(_clientStream[0], LOG_FILE_DIR LOG_FILE_NAME_C1, std::fstream::in | std::fstream::trunc);
	Tintin_reporter::_OpenStream(_clientStream[1], LOG_FILE_DIR LOG_FILE_NAME_C2, std::fstream::in | std::fstream::trunc);
	Tintin_reporter::_OpenStream(_clientStream[2], LOG_FILE_DIR LOG_FILE_NAME_C3, std::fstream::in | std::fstream::trunc);
}

void		Tintin_reporter::DeInit(void)
{
	Tintin_reporter::_logStream->close();
	delete Tintin_reporter::_logStream;
	delete [] Tintin_reporter::_clientStream;
}
