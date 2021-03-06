/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tiny.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmostafa <mmostafa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 15:25:01 by mmostafa          #+#    #+#             */
/*   Updated: 2020/10/15 15:50:28 by mmostafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_managers.h"


static void     create_tiny_list()
{
	t_zone  *tinies;
	t_mem   *tiny;
	int     i;
	t_mem   *head;

	g_zones.tinies = (t_zone *)mini_malloc(sizeof(t_zone), 0);
	tinies = g_zones.tinies;
	tinies->zone = (t_mem *)mini_malloc(sizeof(t_mem), 0);
	tiny = tinies->zone;
	tiny->ptr = mini_malloc(TINY_SIZE, 0);
	tiny->is_empty = 'F';
	tiny->size = g_zones.requested_size;
	tiny->prev = NULL;
	head = tiny;
	tiny->next = (t_mem *)mini_malloc(sizeof(t_mem), 0);
	tiny->next->prev = tiny;
	i = 1;
	while (i < MAX_MALLOCS)
	{
		tiny = tiny->next;
		tiny->ptr = mini_malloc(TINY_SIZE, 0);
		tiny->is_empty = 'E';
		tiny->size = TINY_SIZE;
		tiny->next = (t_mem *)mini_malloc(sizeof(t_mem), 0);
		tiny->next->prev = tiny;
		i++;
	}
	tiny->next = NULL;
	tiny = head;
}
	

static void    *search_in_tiny_list()
{
	t_zone  *tinies;
	t_mem   *tiny;
	t_mem   *tiny_head;
	t_zone   *tinies_head;


	tinies = g_zones.tinies;
	tiny = tinies->zone;
	tiny_head = tiny;
	tinies_head = tinies;
	while (tinies_head)
	{
		while (tiny_head)
		{  
			if (tiny_head->is_empty == 'E')
			{
				tiny_head->is_empty = 'F';
				tiny_head->size = g_zones.requested_size;
				return (tiny_head->ptr);
			}
			tiny_head = tiny_head->next;
		}
		if (!tinies_head->next)
		{
			create_tiny_list();
			tinies_head->next = g_zones.tinies;
			g_zones.tinies->prev = tinies_head;
			g_zones.tinies = tinies;
			break ;
		}
		tinies_head = tinies_head->next;
		tiny_head = tinies_head->zone;
	}
	printf("in searching %p\n", tinies_head->next->zone->ptr);
	return (tinies_head->next->zone->ptr);
}

void    *ft_tiny()
{
	if (g_zones.tinies)
	{
		return (search_in_tiny_list());
	}
	create_tiny_list();
	return (g_zones.tinies->zone->ptr);
}