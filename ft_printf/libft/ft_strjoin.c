/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerk <ljerk@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/08 00:30:37 by ljerk             #+#    #+#             */
/*   Updated: 2020/08/19 13:36:53 by ljerk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*sr;
	char	*st;

	if (!s1 || !s2)
		return (NULL);
	sr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!sr)
		return (NULL);
	st = sr;
	while (*s1)
	{
		*sr = *s1;
		sr++;
		s1++;
	}
	while (*s2)
	{
		*sr = *s2;
		sr++;
		s2++;
	}
	*sr = '\0';
	return (st);
}
