/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 22:25:10 by injah             #+#    #+#             */
/*   Updated: 2025/06/04 21:47:08 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	init_audio(t_data *data)
{
	InitAudioDevice();
	data->player.foot_steps[0] = LoadSound("sound/footstep/421149__giocosound__footstep_wood_heel_2.wav");
	SetSoundVolume(data->player.foot_steps[0], 0.1);
	data->player.foot_steps[1] = LoadSound("sound/footstep/9918__snoman__wood2.wav");
	SetSoundVolume(data->player.foot_steps[1], 0.1);
	data->player.foot_steps[2] = LoadSound("sound/footstep/9919__snoman__wood3.wav");
	SetSoundVolume(data->player.foot_steps[2], 0.1);
	data->player.foot_steps[3] = LoadSound("sound/footstep/9920__snoman__wood4.wav");
	SetSoundVolume(data->player.foot_steps[3], 0.1);
	data->player.foot_steps[4] = LoadSound("sound/footstep/110100__ronaldvanwonderen__heavy_footstep_wood_3.wav");
	SetSoundVolume(data->player.foot_steps[4], 0.1);
}

static void init_player(t_data *data)
{
	data->player.move_speed = 1.0f;
	data->player.velocity_z = 0.0f;
	data->player.floor_height = 0.0f;
	data->player.position.x = 1.5f;
	data->player.position.y = 1.5f;
	data->player.position.z = 0.0f;
	data->player.pos_z_modifier = 0.0f;
	data->player.view_z = 0.0f;
	data->player.rot_speed = 3.0f;
	data->player.direction.x = 0.0f;
	data->player.direction.y = 1.0f;
	data->player.direction.z = 0.0f;
	data->player.state = STANDING;
	data->player.plane.x = -0.66f;
	data->player.plane.y = 0.0f;
	data->player.plane.z = 0.0f;
	data->player.pitch = 0.0f;
	data->player.visibility_dist = 4;
}

static void init_game(t_data *data)
{
	data->map.col = 0;
	data->map.row = 0;
	data->hotkeys.key_a = false;
	data->hotkeys.key_w = false;
	data->hotkeys.key_z = false;
	data->hotkeys.key_q = false;
	data->hotkeys.key_d = false;
	data->hotkeys.key_s = false;
	data->hotkeys.mouse_x = 0.0;
	data->hotkeys.mouse_y = 0.0;
	data->screen_width = 1440;
	data->screen_height = 900;
	data->screen_half_width = data->screen_width / 2;
	data->screen_half_height = data->screen_height / 2;
	data->flashlight.pos.x = data->screen_half_width - data->screen_half_width / 5;
	data->flashlight.pos.y = data->screen_half_height + data->screen_half_height / 5;
	data->flashlight.pos.z = 0;
	data->flashlight.equiped = false;
	data->color_buffer = malloc(sizeof(unsigned int) * data->screen_width * data->screen_height);
	data->distance_buffer = malloc(sizeof(float) * data->screen_width * data->screen_height);
}
static void nullify_ptr(t_data *data)
{
	data->mlx = NULL;
	data->win = NULL;
	data->color_buffer = NULL;
	data->distance_buffer = NULL;
	data->map.grid = NULL;
}

void init(t_data *data)
{
	nullify_ptr(data);
	init_game(data);
	init_audio(data);
	init_player(data);
}

void	init_images(t_data *data)
{
	data->frame_img = ft_mlx_new_img(data->mlx, data->screen_width, data->screen_height);
	data->floor =  ft_mlx_new_img_xpm(data->mlx, "textures/Wooden_Floor_Vertical_64x64.xpm");
	data->wall =  ft_mlx_new_img_xpm(data->mlx, "textures/Brick_Wall_64x64.xpm");
	data->ceil =  ft_mlx_new_img_xpm(data->mlx, "textures/Brick_Wall_64x64.xpm");
	data->minimap =  ft_mlx_new_img_xpm(data->mlx, "textures/minimap.xpm");
}



int destroy(t_data *data)
{
	mlx_destroy_image(data->mlx, data->frame_img.img_ptr);
	mlx_destroy_image(data->mlx, data->wall.img_ptr);
	mlx_destroy_image(data->mlx, data->floor.img_ptr);
	mlx_destroy_image(data->mlx, data->ceil.img_ptr);
	mlx_destroy_image(data->mlx, data->minimap.img_ptr);
	if (data->win && data->mlx)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
		mlx_destroy_display(data->mlx);
	free(data->mlx);
	ft_free_tab(data->map.grid);
	free(data->color_buffer);
	free(data->distance_buffer);
	UnloadSound(data->player.foot_steps[0]);
	UnloadSound(data->player.foot_steps[1]);
	UnloadSound(data->player.foot_steps[2]);
	UnloadSound(data->player.foot_steps[3]);
	UnloadSound(data->player.foot_steps[4]);
	CloseAudioDevice();
	exit(0);
	return (0);
}

void duke_error(t_data *data, const char *error)
{
	ft_dprintf(2, error);
	destroy(data);
}