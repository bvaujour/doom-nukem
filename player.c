/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 08:41:27 by injah             #+#    #+#             */
/*   Updated: 2025/06/04 20:19:23 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	apply_velocity_z(t_player *player, float delta_time)
{
	const float		gravity = 3.5f;
	
	player->position.z += player->velocity_z * delta_time;
	player->velocity_z -= gravity * delta_time;
	if (player->velocity_z < 0 && player->position.z <  player->floor_height)
	{
		player->velocity_z = 0.0f;
		player->position.z = player->floor_height;
		player->state = STANDING;
	}
}

static bool	move(t_player *player, t_map *map, float dir_x, float dir_y, float delta_time)
{
	float		new_pos_x;
	float 		new_pos_y;
	const float	max_step_height = 0.15f;
	float		new_floor_height;

	new_pos_x = player->position.x + dir_x * player->move_speed * delta_time;
	new_pos_y = player->position.y + dir_y * player->move_speed * delta_time;
	if (is_out_of_map(map, floor(new_pos_x), floor(new_pos_y)))
		return (false);
	new_floor_height = get_wall_height(map, new_pos_x, new_pos_y);
	if (new_floor_height > player->position.z + max_step_height)
		return (false);
	if (new_floor_height < player->floor_height - max_step_height)
		player->state = FALLING;
	player->position.x = new_pos_x;
	player->position.y = new_pos_y;
	player->floor_height = new_floor_height;
	return (true);
}

// static void	rotate(t_player *player, float rot_velocity)
// {
// 	double oldDirX = player->direction.x;

// 	player->direction.x = player->direction.x * cos(rot_velocity) -player->direction.y * sin(rot_velocity);
// 	player->direction.y = oldDirX * sin(rot_velocity) + player->direction.y * cos(rot_velocity);

// 	double oldPlaneX = player->plane.x;

// 	player->plane.x =player->plane.x * cos(rot_velocity) -player->plane.y * sin(rot_velocity);
// 	player->plane.y = oldPlaneX * sin(rot_velocity) +player->plane.y * cos(rot_velocity);
// }

static void	handle_state(t_player *player, float time_moving, float delta_time)
{
	const float	step_frequency = player->move_speed * 8;
	const float	step_amplitude = player->move_speed * 0.02;
	const float step_position = sin(time_moving * step_frequency);
	static bool	foot_step_played = false;

	if (player->state == STANDING)
	{
		player->pos_z_modifier = finterp(player->pos_z_modifier, step_position * step_amplitude, 5, delta_time);
		if (step_position < -0.9 && foot_step_played == false)
		{
			PlaySound(player->foot_steps[rand() % 4]);
			foot_step_played = true;
		}
		if (step_position >= 0.9)
			foot_step_played = false;
	}
	else if (player->state == FALLING)
		apply_velocity_z(player, delta_time);
	else if (player->state == CROUCHED)
		player->pos_z_modifier = finterp(player->pos_z_modifier, -0.25, 8, delta_time);
	else if (player->state == GETTING_UP)
	{
		player->pos_z_modifier = finterp(player->pos_z_modifier, player->position.z, 10, delta_time);
		if (player->pos_z_modifier >= player->position.z)
		{
			player->pos_z_modifier = player->position.z;
			player->state = STANDING;
		}
	}
	if (player->state != FALLING)
		player->position.z = finterp(player->position.z, player->floor_height, 10, delta_time);
}

static bool	handle_movement(t_player *player, t_hotkeys *hotkeys, t_map *map, float delta_time)
{
	bool		moved;

	moved = false;
	if (hotkeys->key_z || hotkeys->key_w)
		moved = move(player, map, player->direction.x, player->direction.y, delta_time);
	if (hotkeys->key_s)
		moved = move(player, map, -player->direction.x, -player->direction.y, delta_time);
	if (hotkeys->key_a || hotkeys->key_q)
		moved = move(player, map, player->direction.y, -player->direction.x, delta_time);
	if (hotkeys->key_d)
		moved = move(player, map, -player->direction.y, player->direction.x, delta_time);
	return (moved);
}

static void	handle_rotation(t_player *player, t_hotkeys *hotkeys, float delta_time)
{
	static float angle = 0;
	if (hotkeys->mouse_x)
	{
		float sensitivity = 1.0f;
		angle += hotkeys->mouse_x * delta_time * sensitivity;

		angle = fmodf(angle, 2 * PI);
		if (angle < 0)
			angle += 2 * PI;

		player->direction.x = cosf(angle);
		player->direction.y = sinf(angle);
		player->plane.x = -player->direction.y * 0.66;
   		player->plane.y =  player->direction.x * 0.66;
		hotkeys->mouse_x = 0.0f;
	}
	if (hotkeys->mouse_y)
	{
		player->pitch += hotkeys->mouse_y * -700 * delta_time;
		hotkeys->mouse_y = 0.0;
		if (player->pitch > 1000)
			player->pitch = 1000;
		if (player->pitch < -1000)
			player->pitch = -1000;
	}
}
	
void update_player(t_data *data, float delta_time)
{
	static float	time_moving = 0.0f;
	
	if (handle_movement(&data->player, &data->hotkeys, &data->map, delta_time))
		time_moving += delta_time;
	else
		time_moving = 0;
	handle_rotation(&data->player, &data->hotkeys, delta_time);
	handle_state(&data->player, time_moving, delta_time);
	data->player.view_z = (data->player.position.z + data->player.pos_z_modifier) * data->screen_height;

}