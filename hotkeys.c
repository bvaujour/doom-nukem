/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotkeys.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 08:36:06 by injah             #+#    #+#             */
/*   Updated: 2025/06/06 11:33:23 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int keypress(int key, t_data *data)
{
	printf("key: %d\n", key);
	if (key == 'q')
		data->hotkeys.key_q = true;
	else if (key == 'a')
		data->hotkeys.key_a = true;
	else if (key == 'd')
		data->hotkeys.key_d = true;
	else if (key == 'z')
		data->hotkeys.key_z = true;
	else if (key == 's')
		data->hotkeys.key_s = true;
	else if (key == 'w')
		data->hotkeys.key_w = true;
	else if (key == 't')
		data->hotkeys.key_t = true;
	else if (key == SPACE)
	{
		if (data->player.state != FALLING)
		{
			data->player.velocity_z = 2.0f;
			data->player.state = FALLING;
		}
	}
	else if (key == L_CTRL)
	{
		data->player.move_speed = 0.7f;
		data->player.state = CROUCHED;

	}
	else if (key == L_SHIFT)
		data->player.move_speed = 2;
	else if (key == 'f')
		data->flashlight.equiped = !data->flashlight.equiped;
	else if (key == ESCAPE)
		destroy(data);
	else if (key == 'o')
	{
		printf("should\n");
		data->player.state = FLYING;
		data->player.position.z += 100;
	}
	else if (key == 'p')
		data->player.position.z -= 100;
	return (0);
}

int keyrelease(int key, t_data *data)
{
	if (key == 'q')
		data->hotkeys.key_q = false;
	else if (key == 'a')
		data->hotkeys.key_a = false;
	else if (key == 'd')
		data->hotkeys.key_d = false;
	else if (key == 'z')
		data->hotkeys.key_z = false;
	else if (key == 's')
		data->hotkeys.key_s = false;
	else if (key == 'w')
		data->hotkeys.key_w = false;
	else if (key == 't')
		data->hotkeys.key_t = false;
	else if (key == L_CTRL)
	{
		data->player.move_speed = 1;
		data->player.state = GETTING_UP;
	}
	else if (key == L_SHIFT)
		data->player.move_speed = 1;
	return (0);
}

int	mouse_move(int x, int y, t_data *data)
{
	static int	last_x = 0;
	static int	last_y = 0;
	if (data->hotkeys.key_t == true)
	{
		printf("x: %d, y: %d, color: %x, wall_dist: %f\n", x, y, data->color_buffer[y * data->screen_width + x], data->distance_buffer[y * data->screen_width + x]);
	}
	else
	{
		if (abs(last_x - x) < 100)
			data->hotkeys.mouse_x = (float)(x - last_x);
		if (abs(last_y - y) < 100)
			data->hotkeys.mouse_y = (float)(y - last_y);
		last_x = x;
		last_y = y;
	}
	return (0);
}

int	mouse_down(int button, t_data *data)
{
	(void)data;
	(void)button;
	// printf("button: %d\n", button);
	
	return (0);
}