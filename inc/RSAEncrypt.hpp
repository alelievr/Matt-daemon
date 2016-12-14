/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSA.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:34:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/14 15:01:34 by root             ###   ########.fr       */
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
#include <stdlib.h>
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
		RSA *			_myKey;
		RSA *			_remoteKey;


	public:
		RSAEncrypt(void);
		RSAEncrypt(const RSAEncrypt &) = delete;
		virtual ~RSAEncrypt(void);

		RSAEncrypt &	operator=(RSAEncrypt const & src) = delete;

		void			WriteTo(const int sock, char *msg, const size_t size);
		std::string		ReadOn(const int sock, long *r);

		size_t			GetMyPublicKey(unsigned char *buff);
		void			SetRemotePublicKey(unsigned char *k, size_t size);

};

std::ostream &	operator<<(std::ostream & o, RSAEncrypt const & r);
