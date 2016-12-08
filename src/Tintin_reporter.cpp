/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/08 13:03:10 by alelievr          #+#    #+#             */
/*   Updated: 2016/12/08 13:13:26 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Tintin_reporter.hpp"

Tintin_reporter::Tintin_reporter(void) : _logStream(NULL)
{
	//open the logStream
	std::cout << "Default constructor of Tintin_reporter called" << std::endl;
}

Tintin_reporter::~Tintin_reporter(void)
{
	std::cout << "Destructor of Tintin_reporter called" << std::endl;
}

std::ostream &	operator<<(std::ostream & o, Tintin_reporter const & r)
{
	o << "tostring of the class" << std::endl;
	(void)r;
	return (o);
}
