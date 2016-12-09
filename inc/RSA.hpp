/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSA.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:34:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/09 06:30:03 by root             ###   ########.fr       */
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

class		RSA
{
	private:
		std::string		_privateKey;
		std::string		_publicKey;

		int				GenerateKeys(const int p, const int q);
		int				GetGCD(int a, int b) const;

	public:
		RSA(void);
		RSA(const RSA&) = delete;
		virtual ~RSA(void);

		RSA &			operator=(RSA const & src) = delete;

		std::string		Encode(std::string & message) const;
		std::string		Decode(std::string & message) const;

		std::string		GetPublicKey(void) const;
		void			SetPublicKey(std::string k);

};

std::ostream &	operator<<(std::ostream & o, RSA const & r);