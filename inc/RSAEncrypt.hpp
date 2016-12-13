/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSA.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:34:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/13 19:49:35 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

#include <iostream>
#include <string>
#include <sstream>
#include <limits>

#include <math.h>
#include <gmp.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define KEY_LENGTH	1024
#define PUB_EXP		3

typedef struct
{
	char	*data;
	size_t	length;
}				EncodedData;

class		RSAEncrypt
{
	private:
		static char *			_publicKey;
		static size_t			_publicKeyLength;
		static char *			_privateKey;
		static size_t			_privateKeyLength;

	public:
		RSAEncrypt(void) = delete;
		RSAEncrypt(const RSAEncrypt &) = delete;
		virtual ~RSAEncrypt(void) = delete;

		RSAEncrypt &	operator=(RSAEncrypt const & src) = delete;

		static void			Init();
		static void			DeInit();

		static void			WriteTo(const int sock, std::string message);
		static std::string	ReadOn(const int sock, long *r);

		static const char *	GetPublicKey(void);
		static void			SetPublicKey(char *k);

};

std::ostream &	operator<<(std::ostream & o, RSAEncrypt const & r);
