/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:34:39 by injah             #+#    #+#             */
/*   Updated: 2025/06/06 13:21:22 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	post_process(t_data *data)
{
	const int 	visibility_dist = data->player.visibility_dist;
	unsigned int	*pixel_color_buffer = data->color_buffer;
	float			*pixel_distance_buffer = data->distance_buffer;
	const int		height = data->screen_height;
	const int		width = data->screen_width;
	const int		flash_light_radius = data->screen_height / 6;

	if (data->flashlight.equiped)
	{
		for (int y = 0; y < height; y++)
		{
			int row = y * width;
			float dy = y - data->flashlight.pos.y;

			for (int x = 0; x < width; x++)
			{
					float world_distance = pixel_distance_buffer[row + x];
					float fact = 0.0f;

					float dx = x - data->flashlight.pos.x;
					float distance_squared = dx * dx + dy * dy;
	
					float distance_to_light = sqrtf(distance_squared);
					float light_range = flash_light_radius * sqrtf(world_distance);
					float dist_ratio = distance_to_light / light_range;
					// Lumière de la torche : parabole inversée
					float flashlight_intensity = 1.0f - dist_ratio;
					if (flashlight_intensity > 0)
						flashlight_intensity *= flashlight_intensity;
					else
						flashlight_intensity = 0;
						
					// Lumière ambiante : atténuation douce
					float ambient_intensity = 1.0f - world_distance / visibility_dist;
					if (ambient_intensity > 0)
						ambient_intensity *= ambient_intensity;
					else
						ambient_intensity = 0;
					// Combine lumière torche + lumière ambiante
					fact = ambient_intensity + flashlight_intensity;
					
					if (fact < 0.0f)
						fact = 0.0f;

				pixel_color_buffer[row + x] = apply_light_modifier(pixel_color_buffer[row + x], fact, 10, 10, 20);
			}
		}
	}
	else
		for (int y = 0; y < height; y++)
		{
			int row = y * width;
			for (int x = 0; x < width; x++)
			{
				float world_distance = pixel_distance_buffer[row + x];
				float ambient_intensity = 1.0f - world_distance / visibility_dist;

				if (ambient_intensity < 0.0f)
					ambient_intensity = 0.0f;
				else
					ambient_intensity *= ambient_intensity;

				pixel_color_buffer[row + x] = apply_light_modifier(pixel_color_buffer[row + x], ambient_intensity, 10, 10, 20);
			}
		}
}

void	render(t_data *data)
{
	// floorcasting(data);
	raycasting(data, &data->map);
}

void	update(t_data *data, float delta_time)
{
	update_player(data, delta_time);
	update_flashlight(data, delta_time);
}

static void	hud(t_data *data)
{
	draw_minimap2(data);
}

int	update_frame(t_data *data)
{
	static size_t	last_frame_time = 0;
	size_t 			current_time;
	float 			delta_time;

	current_time = ft_gettime();
	delta_time = (float)(current_time - last_frame_time) / 1000;
	if (delta_time >= 0.01666)
	{
		last_frame_time = current_time;

		size_t 			start = ft_gettime_us();

		memset(data->frame_img.addr, 0, data->frame_img.buffer_size);
		memset(data->color_buffer, 0, data->screen_height * data->screen_width * sizeof(unsigned int));
		memset(data->distance_buffer, 0, data->screen_height * data->screen_width * sizeof(unsigned int));

		size_t	after_memset = ft_gettime_us();

		update(data, delta_time);

		size_t	after_update_player = ft_gettime_us();

		render(data);

		size_t	after_raycasting = ft_gettime_us();

		// post_process(data);

		size_t	after_post_process = ft_gettime_us();

		hud(data);
		memcpy(data->frame_img.addr, data->color_buffer, data->screen_width * data->screen_height * sizeof(unsigned int));

		size_t	after_minimap = ft_gettime_us();

		mlx_put_image_to_window(data->mlx, data->win, data->frame_img.img_ptr, 0, 0);
		size_t	after_put_image = ft_gettime_us();

		if (0)
			printf("Memset: %ld us, Update player: %ld us, world_screen: %ld us, Postprocess: %ld us, Minimap: %ld us, Put image: %ld us\n",
        after_memset - start,
        after_update_player - after_memset,
        after_raycasting - after_update_player,
        after_post_process - after_raycasting,
        after_minimap - after_post_process,
        after_put_image - after_minimap);
		if (0)
		{
			printf("position: x:%f, y:%f, z:%f\npitch: %f\n, floor_height: %f\n", data->player.position.x, data->player.position.y, data->player.position.z, data->player.pitch, data->player.floor_height);
			printf("direction = x:%f, y:%f\n\n", data->player.direction.x, data->player.direction.y);
		}
	}
	return (0);
}

int main()
{
	t_data data;
	
	init(&data);
	get_map(&data, &data.map, "maps/map0.ber");
	data.mlx = mlx_init();
	if (!data.mlx)
		destroy(&data);
	data.win = mlx_new_window(data.mlx, data.screen_width, data.screen_height, "DUKE");
	init_images(&data);
	mlx_hook(data.win, 17, 1L << 17, destroy, &data);
	mlx_hook(data.win, 6, 1L<<6, mouse_move, &data);
	mlx_hook(data.win, 2, 1L << 0, keypress, &data);
	mlx_hook(data.win, 3, 1L << 1, keyrelease, &data);
	mlx_hook(data.win, 4, 1L<<2, mouse_down, &data);
	mlx_loop_hook(data.mlx, update_frame, &data);
	mlx_loop(data.mlx);
	return (0);
}