/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mac <mac@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 16:32:22 by eozben            #+#    #+#             */
/*   Updated: 2023/11/15 02:44:58 by mac              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(char *str, int *integer)
{
	int		sign;
	long	value;

	sign = 1;
	value = 0;
	if (!str)
		return (0);
	if (*str == '+' | *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		if (*str > '9' || *str < '0')
			return (0);
		value = value * 10 + *str - '0';
		if ((sign > 0 && value > INT_MAX) || (sign < 0 && value > 2147483648))
			return (0);
		str++;
	}
	*integer = value * sign;
	return (1);
}
