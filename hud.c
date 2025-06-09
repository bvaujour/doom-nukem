/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 16:09:11 by injah             #+#    #+#             */
/*   Updated: 2025/06/06 11:33:30 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void draw_minimap(t_data *data)
{
	int x;
	int y;
	const int cell_size = (data->screen_half_width / 2) / data->map.col;

	y = 0;
	while (data->map.grid[y])
	{
		x = 0;
		while (data->map.grid[y][x])
		{
			if (data->map.grid[y][x] != '0')
				ft_draw_square(data->color_buffer, data->screen_width, x * cell_size, y * cell_size, cell_size - 1, cell_size - 1, 0xFFFFFF);
			x++;
		}
		y++;
	}
	ft_draw_circle(data->color_buffer, data->screen_width, data->player.position.x * cell_size, data->player.position.y * cell_size, 2, 0x00FF00);
}

void draw_minimap2(t_data *data)
{
	int x, y;
	unsigned int color;
	const unsigned int *minimap_buffer = data->minimap.addr;
	const int minimap_width = data->minimap.width;
	const int minimap_height = data->minimap.height;
	const float cell_size = minimap_width / 20.0f; // 10 cases visibles
	int			last_map_x = -100;
	int			last_map_y = -100;
	
	for (y = 0; y < minimap_height; y++)
	{
		float map_y = data->player.position.y + ((float)y - minimap_height / 2.0f) / cell_size;
		int map_iy = (int)floor(map_y);
		for (x = 0; x < minimap_width; x++)
		{
			// Position sur la map
			
			// Par défaut, couleur de fond (ex: noir transparent)
			color = minimap_buffer[y * minimap_width + x];
			if (color == 0xFF000000)
				continue ;
			// Si dans la map, et pixel dans image spéciale
			float map_x = data->player.position.x + ((float)x - minimap_width / 2.0f) / cell_size;
			int map_ix = (int)floor(map_x);

			if (last_map_y == map_iy && last_map_x == map_ix && color == 0x6e6e6e && !is_out_of_map(&data->map, map_ix, map_iy) && data->map.grid[map_iy][map_ix] != '0')
			{
				// Utiliser la couleur spéciale si tu as marqué avec 0x006e6e6e
				color = 0x00ffff; // mur
			}
			else if (last_map_y == map_iy && last_map_x == map_ix && color == 0x6e6e6e && !is_out_of_map(&data->map, map_ix, map_iy) && data->map.grid[map_iy][map_ix] == '0')
			{
				// Utiliser la couleur spéciale si tu as marqué avec 0x006e6e6e
				color = 0xffffff; // mur
			}
			data->color_buffer[y * data->screen_width + x] = color;
			last_map_x = map_ix;
		}
		last_map_y = map_iy;
	}
}

