/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSA.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:34:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/14 07:31:20 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

#include <iostream>
#include <string>
#include <sstream>
#include <limits>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <math.h>
#include <gmp.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <bsd/string.h>

#define MSG_BLOCK_SIZE	2048
#define KEY_LENGTH		1024
#define PUB_EXP			3

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
		static RSA *			_keypair;

	public:
		RSAEncrypt(void) = delete;
		RSAEncrypt(const RSAEncrypt &) = delete;
		virtual ~RSAEncrypt(void) = delete;

		RSAEncrypt &	operator=(RSAEncrypt const & src) = delete;

		static void			Init();
		static void			DeInit();

		static void			WriteTo(const int sock, char *msg, const size_t size);
		static std::string	ReadOn(const int sock, long *r);

		static const char *	GetPublicKey(void);
		static void			SetPublicKey(char *k);

};

std::ostream &	operator<<(std::ostream & o, RSAEncrypt const & r);
