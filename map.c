/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 23:32:00 by injah             #+#    #+#             */
/*   Updated: 2025/05/29 16:57:58 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static int	get_longest_string(char **map)
{
	int	i;
	int	longest_line_len;
	int	current_line_len;

	i = 0;
	longest_line_len = 0;
	current_line_len = 0;
	while (map[i])
	{
		current_line_len = ft_strlen(map[i]);
		if (current_line_len > longest_line_len)
			longest_line_len = current_line_len;
		i++;
	}
	return (longest_line_len);
}

void	get_map(t_data *data, t_map *map, char *path)
{
	ft_free_tab(map->grid);
	map->grid = NULL;
	map->grid = ft_stock_file(path);
	if (!map->grid)
		duke_error(data, "Nukem: get_Map: Failed to stock_file\n");
	data->map.col = get_longest_string(map->grid);
	int		i;
	char	*str;
	i = 0;
	while (map->grid[i])
	{
		str = calloc((data->map.col + 1), sizeof(char));
		if (!str)
			duke_error(data, "Nukem: get_Map: Failed to stock_file\n");
		ft_strlcpy(str, map->grid[i], ft_strlen(map->grid[i]));
		free(map->grid[i]);
		map->grid[i] = str;
		i++;
	}
	data->map.row = i;
}