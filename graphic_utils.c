/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:12:43 by injah             #+#    #+#             */
/*   Updated: 2025/05/29 00:20:34 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	ft_mlx_set_pixel(t_img *img, int x, int y, int color)
{
	int	pixel_index;

	pixel_index = y * img->pixel_per_line + x;
	img->addr[pixel_index] = color;
}

unsigned int	ft_mlx_get_pixel(t_img *img, int x, int y)
{
	int	pixel_index;
	int	color;

	pixel_index = y * img->pixel_per_line + x;
	color = img->addr[pixel_index];
	return (color);
}

void	ft_mlx_put_img_to_img_scaled(t_img *dst, t_img *src)
{
	int		i;
	int		j;
	float	x;
	float	y;

	i = 0;
	x = 0;
	while (i < dst->width)
	{
		j = 0;
		y = 0;
		while (j < dst->height)
		{
			ft_mlx_set_pixel(dst, i, j, ft_mlx_get_pixel(src, (int)x, (int)y));
			j++;
			y += (float)src->height / dst->height;
		}
		i++;
		x += (float)src->width / dst->width;
	}
}

void	ft_mlx_put_img_to_img(t_img *dst, t_img *src, int x, int y)
{
	int	i;
	int	j;

	j = 0;
	while (j < src->height)
	{
		i = 0;
		while (i < src->width)
		{
			if (x + i > 0 && y + j > 0 && x + i < dst->width
				&& y + j < dst->height)
				ft_mlx_set_pixel(dst, x + i, y + j, ft_mlx_get_pixel(src, i, j));
			i++;
		}
		j++;
	}
}

void	ft_mlx_draw_square(t_img *dest, int start_x, int start_y, int square_width, int square_height, int color)
{
	int	x;
	int	y;

	y = 0;
	while (y < square_height)
	{
		x = 0;
		while (x < square_width)
		{
			ft_mlx_set_pixel(dest, start_x + x,  start_y + y, color);
			x++;
		}
		y++;
	}
}

void	ft_draw_square(unsigned int *pixel_buffer, int buffer_lenght, int start_x, int start_y, int square_width, int square_height, int color)
{
	int	x;
	int	y;

	y = start_y;
	while (y < start_y + square_height)
	{
		x = start_x;
		while (x < start_x + square_width)
		{
			pixel_buffer[y * buffer_lenght + x] = color;
			x++;
		}
		y++;
	}
}

void	ft_mlx_draw_circle(t_img *dest, int center_x, int center_y, int radius, int color)
{
	int	x;
	int	y;

	x = center_x - radius;
	while (x <= center_x + radius)
	{
		y = center_y - radius;
		while (y <= center_y + radius)
		{
			if ((x - center_x) * (x - center_x) + (y - center_y)
				* (y - center_y) <= radius * radius)
				ft_mlx_set_pixel(dest, x, y, color);
			y++;
		}
		x++;
	}
}

void	ft_draw_circle(unsigned int *pixel_buffer, int buffer_lenght, int center_x, int center_y, int radius, int color)
{
	int	x;
	int	y;

	x = center_x - radius;
	while (x <= center_x + radius)
	{
		y = center_y - radius;
		while (y <= center_y + radius)
		{
			if ((x - center_x) * (x - center_x) + (y - center_y)
				* (y - center_y) <= radius * radius)
				pixel_buffer[y * buffer_lenght + x] = color;
			y++;
		}
		x++;
	}
}


void	ft_mlx_draw_line(t_img *dest, int start_x, int start_y, int length, float angle, int color)
{
	double	x;
	double	y;
	int	i;

	x = 0;
	y = 0;
	i = 0;
	while (i < length)
	{
		ft_mlx_set_pixel(dest, start_x + (int)x, start_y + (int)y, color);
		x += cos(angle);
		y += sin(angle);
		i++;
	}
}

t_img	ft_mlx_new_img(void *mlx, int width, int height)
{
	t_img	img;

	img.img_ptr = mlx_new_image(mlx, width, height);
	img.addr = (unsigned int *)mlx_get_data_addr(img.img_ptr, &img.bits_per_pixel, &img.line_length, &img.endian);
	img.width = width;
	img.height = height;
	img.pixel_per_line = img.line_length / (img.bits_per_pixel / 8);
	img.buffer_size = width * height * (img.bits_per_pixel / 8);
	return (img);
}

t_img	ft_mlx_new_img_xpm(void *mlx, char *path)
{
	t_img	img;

	img.img_ptr = mlx_xpm_file_to_image(mlx, path, &img.width, &img.height);
	img.addr = (unsigned int *)mlx_get_data_addr(img.img_ptr, &img.bits_per_pixel, &img.line_length, &img.endian);
	img.pixel_per_line = img.line_length / (img.bits_per_pixel / 8);
	img.buffer_size = img.width * img.height * (img.bits_per_pixel / 8);
	return (img);
}

unsigned int apply_light_modifier(unsigned int color, float light_factor, int light_r, int light_g, int light_b)
{
	int r = ((color >> 16) & 0xFF);
	int g = ((color >> 8) & 0xFF);
	int b = (color & 0xFF);

	r = r * light_factor + light_r * (1.0f - light_factor);
	g = g * light_factor + light_g * (1.0f - light_factor);
	b = b * light_factor + light_b * (1.0f - light_factor);
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
	return (r << 16 | g << 8 | b);
}


