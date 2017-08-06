/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cs_uc_duo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnederlo <jnederlo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/24 10:51:14 by jnederlo          #+#    #+#             */
/*   Updated: 2017/07/29 14:59:17 by jnederlo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

int		choose_len_uc_d(t_type *type, t_badge *badge, va_list ap)
{
	long long	num;

	if (badge->l || badge->ll || badge->j || badge->z)
	{
		type->ll_int = va_arg(ap, long long);
		return (num = count_digit_lli(type));
	}
	else if (badge->h)
	{
		type->ll_int = va_arg(ap, long);
		return (num = count_digit_lli(type));
	}
	else
		type->ll_int = va_arg(ap, long);
	return (num = count_digit_lli(type));
}

int		choose_len_uc_uo(t_type *type, t_badge *badge, va_list ap)
{
	unsigned long long	num;

	if (badge->l || badge->ll || badge->j || badge->z)
	{
		type->ull_int = va_arg(ap, unsigned long long);
		return (num = count_digit_ulli(type));
	}
	else if (badge->h)
	{
		type->ull_int = va_arg(ap, unsigned long);
		return (num = count_digit_ulli(type));
	}
	else
		type->ull_int = va_arg(ap, unsigned long);
	return (num = count_digit_ulli(type));
}
