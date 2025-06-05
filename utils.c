/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:55:13 by injah             #+#    #+#             */
/*   Updated: 2025/06/04 21:48:32 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

size_t	ft_gettime()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

size_t	ft_gettime_us()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

int	ft_iclamp(int value, int min, int max)
{
	if (value > max)
		return (max);
	if (value < min)
		return (min);
	return (value);
}

float	finterp(float start, float end, float speed, float delta_time)
{
	const float	result = start + (end - start) * speed * delta_time;
	return (result);
}

bool	is_out_of_map(t_map *map, int x, int y)
{
	return (x >= map->col || x < 0 || y >= map->row || y < 0);
}

float	get_wall_height(t_map *map, int x, int y)
{
	if (is_out_of_map(map, x, y))
		return (5.0f);
	return ((map->grid[y][x] - 48) * 0.1f);
}

bool	is_walkable(t_player *player, t_map *map, int x, int y)
{
	float		floor_height;
	const float	max_step_height = 0.15f;
	if (is_out_of_map(map, x, y))
		return (false);
	floor_height = get_wall_height(map, x, y);
	if (floor_height > player->position.z + max_step_height)
		return (false);
	return (true);
}