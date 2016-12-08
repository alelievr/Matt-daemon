/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RSA.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:34:05 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/08 13:36:32 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#pragma clang diagnostic ignored "-Wc++98-compat"

# include <iostream>
# include <string>

class		RSA
{
	private:
		std::string		_privateKey;
		std::string		_publicKey;

	public:
		RSA(void);
		RSA(const RSA&) = delete;
		virtual ~RSA(void);

		RSA &			operator=(RSA const & src) = delete;

		int				GetGCD(const int n) const;
		int				GenerateKeys(const int p, const int q);
		std::string		GetPublicKey(void) const;

};

std::ostream &	operator<<(std::ostream & o, RSA const & r);
