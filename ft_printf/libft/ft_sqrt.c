/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerk <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 16:34:20 by ljerk             #+#    #+#             */
/*   Updated: 2019/09/04 17:30:21 by ljerk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_sqrt(int nb)
{
	int n;

	n = 1;
	if (nb <= 0)
		return (0);
	else
	{
		while (n * n < nb)
			n++;
		if (nb % n == 0)
			return (n);
		else
			return (0);
	}
}
