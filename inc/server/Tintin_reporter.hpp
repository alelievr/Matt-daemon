/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:01:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/13 15:56:12 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#pragma clang diagnostic ignored "-Wc++98-compat"

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

class		Tintin_reporter
{
	private:
		static std::ofstream		*_logStream;
		static std::ofstream		*_clientStream;
		static int					_InternalLog(std::ofstream * stream, const char *type, const std::string & message);
		static void					_OpenStream(std::ofstream & stream, const char *file, const std::ios_base::openmode mode);

	public:
		Tintin_reporter(void) = delete;
		Tintin_reporter(const Tintin_reporter&) = delete;
		virtual ~Tintin_reporter(void) = delete;

		Tintin_reporter &	operator=(Tintin_reporter const & src) = delete;

		static int				Log(const std::string & message);
		static int				LogInfo(const std::string & message);
		static int				LogError(const std::string & message);
		static int				LogClient(const int clientNumber, const std::string & message);
		static void				Init(void);
		static void				DeInit(void);
};
