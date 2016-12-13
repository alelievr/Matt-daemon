/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSA.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:34:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/12 23:49:25 by root             ###   ########.fr       */
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

typedef struct
{
	char	*data;
	size_t	length;
}				EncodedData;

class		RSA
{
	private:
		int				GenerateKeys(const char *modulo, const char *publicKey, const char *privateKey);
		int				GetGCD(int a, int b) const;
		int				ModInverse(int a, int m) const;

		mpz_t			_privateKey;
		mpz_t			_publicKey;
		mpz_t			_modulo;
		const char *	_publicKeyText;
		const char *	_moduloText;

		void			EncodeBlock(mpz_t & encoded, mpz_t & plain) const;
		void			DecodeBlock(mpz_t & plain, mpz_t & encoded) const;

	public:
		RSA(void);
		RSA(const RSA&) = delete;
		virtual ~RSA(void);

		RSA &			operator=(RSA const & src) = delete;

		//std::string		Encode(const std::string & message) const;
		//std::string		Decode(const std::string & message) const;
		void				EncodeWrite(const int sock, std::string message) const;
		std::string			DecodeRead(const int sock, long *r) const;

		const char *	GetPublicKey(void) const;
		void			SetPublicKey(const char *k);
		const char *	GetModulo(void) const;
		void			SetModulo(const char *m);

};

std::ostream &	operator<<(std::ostream & o, RSA const & r);
