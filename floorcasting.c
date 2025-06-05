/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floorcasting.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:13:14 by injah             #+#    #+#             */
/*   Updated: 2025/06/02 19:11:08 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"





void	floorcasting(t_data *data)
{
	float rayDirX0 = data->player.direction.x - data->player.plane.x;
	float rayDirY0 = data->player.direction.y - data->player.plane.y;
	float rayDirX1 = data->player.direction.x + data->player.plane.x;
	float rayDirY1 = data->player.direction.y + data->player.plane.y;
	int horizon = data->screen_half_height + data->player.pitch;

	for (int y = 0; y < data->screen_height; ++y)
	{
		// 👇 Position corrigée de l'horizon avec pitch
		int p = y - horizon;
		
		if (p == 0)
			continue;
		float	camZ;
		bool is_floor = y > data->screen_height / 2 + data->player.pitch;
		if (is_floor)
			camZ = (0.5f * data->screen_height) + data->player.view_z;
		else
			camZ = (0.5f * data->screen_height) - data->player.view_z ;
		float rowDistance = camZ / abs(p);
		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / data->screen_width;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / data->screen_width;

		float floorX = data->player.position.x + rowDistance * rayDirX0;
		float floorY = data->player.position.y + rowDistance * rayDirY0;

		for (int x = 0; x < data->screen_width; ++x)
		{
			int cellX = (int)(floorX);
			int cellY = (int)(floorY);

			int tx = (int)(data->floor.width * (floorX - cellX)) & (data->floor.width - 1);
			int ty = (int)(data->floor.height * (floorY - cellY)) & (data->floor.height - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			unsigned int color;
			if (cellX < 0 || cellX >= data->map.col || cellY < 0 || cellY >= data->map.row)
				continue ;
			if (p > 0)
			{
				// Dessiner le SOL
				color = data->floor.addr[data->floor.width * ty + tx];
				data->distance_buffer[y * data->screen_width + x] = rowDistance;
				// ft_mlx_set_pixel(&data->frame_img, x, y, color);
			}
			else
			{
				// Dessiner le PLAFOND
				color = 0x568974;
				data->distance_buffer[y * data->screen_width + x] = 1000.0f;
				// ft_mlx_set_pixel(&data->frame_img, x, y, color);
			}
			data->color_buffer[y * data->screen_width + x] = color;
		}
	}
}

// void	floorcasting(t_data *data)
// {
// 	float rayDirX0 = data->player.direction.x - data->player.plane.x;
// 	float rayDirY0 = data->player.direction.y - data->player.plane.y;
// 	float rayDirX1 = data->player.direction.x + data->player.plane.x;
// 	float rayDirY1 = data->player.direction.y + data->player.plane.y;
// 	int horizon = data->screen_half_height + data->player.pitch;
// 	for (int y = 0; y < data->screen_height; ++y)
// 	{
// 		// 👇 Position corrigée de l'horizon avec pitch
// 		int p = y - horizon;
		
// 		if (p == 0)
// 			continue;
// 		float	camZ;
// 		bool is_floor = y > data->screen_height / 2 + data->player.pitch;
// 		if (is_floor)
// 			camZ = 0.5f * data->screen_height + data->player.view_z;
// 		else
// 			camZ = 0.5f * data->screen_height - data->player.view_z ;
// 		float rowDistance = camZ / abs(p); // Use fabs to get positive distance
// 		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / data->screen_width;
// 		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / data->screen_width;

// 		float floorX = data->player.position.x + rowDistance * rayDirX0;
// 		float floorY = data->player.position.y + rowDistance * rayDirY0;

// 		for (int x = 0; x < data->screen_width; ++x)
// 		{
// 			int cellX = (int)(floorX);
// 			int cellY = (int)(floorY);

// 			int tx = (int)(data->floor.width * (floorX - cellX)) & (data->floor.width - 1);
// 			int ty = (int)(data->floor.height * (floorY - cellY)) & (data->floor.height - 1);

// 			floorX += floorStepX;
// 			floorY += floorStepY;

// 			data->distance_buffer[y * data->screen_width + x] = rowDistance;
// 			unsigned int color;
// 			if (cellX < 0 || cellX >= data->map.col || cellY < 0 || cellY >= data->map.row || data->map.grid[cellY][cellX] != '0')
// 				continue ;
// 			else if (p > 0)
// 			{
// 				// Dessiner le SOL
// 				color = data->floor.addr[data->floor.width * ty + tx];
// 				// ft_mlx_set_pixel(&data->frame_img, x, y, color);
// 			}
// 			else
// 			{
// 				// Dessiner le PLAFOND
// 				color = data->ceil.addr[data->ceil.width * ty + tx];
// 				// ft_mlx_set_pixel(&data->frame_img, x, y, color);
// 			}
// 			data->color_buffer[y * data->screen_width + x] = color;
// 		}
// 	}
// }

