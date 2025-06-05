/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 21:11:12 by injah             #+#    #+#             */
/*   Updated: 2025/06/04 21:33:39 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DOOM_NUKEM
# define DOOM_NUKEM

# include "mlx.h"
# include <raylib.h>
# include "libft.h"
# include <stdlib.h>
# include <stdbool.h>
# include <sys/time.h>
# include <math.h>

# define ERROR_ALLOC "Nukem: Failed to allocate memory\n"
# define ERROR_MAP_PATH "Nukem: Failed to open map path.\n"
# define ESCAPE 65307
# define L_CTRL 65507
# define L_SHIFT 65505
# define SPACE 32

enum	e_card
{
	NORTH = 0,
	SOUTH,
	EAST,
	WEST,
};

enum	e_player_state
{
	STANDING = 0,
	CROUCHED,
	FALLING,
	GETTING_UP,
	FLYING,
};

typedef struct	s_img
{
	void				*img_ptr;
	unsigned int		*addr;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
	int					pixel_per_line;
	int					width;
	int					height;
	size_t				buffer_size;
}				t_img;

typedef struct	s_vec3
{
	float	x;
	float	y;
	float	z;
}				t_vec3;

typedef struct	s_flashlight
{
	t_vec3	pos;
	bool	equiped;
}				t_flashlight;

typedef struct	s_player
{
	t_vec3				position;
	float				pos_z_modifier;
	float				velocity_z;
	float				view_z;
	t_vec3				direction;
	t_vec3				plane;
	enum e_player_state	state;
	int					visibility_dist;
	float				pitch;
	float				move_speed;
	float				rot_speed;
	float				floor_height;
	Sound				foot_steps[5];
}				t_player;

typedef struct	s_hotkeys
{
	bool	key_a;
	bool	key_q;
	bool	key_s;
	bool	key_d;
	bool	key_w;
	bool	key_z;
	float	mouse_x;
	float	mouse_y;
}				t_hotkeys;


typedef struct s_ray
{
	double		camera_x;
	double		dir_x;
	double		dir_y;
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	double		sidedist_x;
	double		sidedist_y;
	double		deltadist_x;
	double		deltadist_y;
	double		wall_dist;
	double		wall_height;
	double		wall_x;
	int			side;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			texture_x;
	int			texture_y;
	double		texture_step;
	double		texture_pos;
	enum e_card	wall_card;
	int			platform_start;
}			t_ray;

typedef struct	s_map
{
	char	**grid;
	int		col;
	int		row;
}				t_map;

typedef struct	s_data
{
	void			*mlx;
	void			*win;
	int				screen_width;
	int				screen_height;
	int				screen_half_width;
	int				screen_half_height;
	t_map			map;
	t_img			minimap;
	t_img			wall;
	t_img			floor;
	t_img			ceil;
	t_img			frame_img;
	t_player		player;
	t_flashlight	flashlight;
	t_hotkeys		hotkeys;
	unsigned int	*color_buffer;
	float			*distance_buffer;
	
}				t_data;

int 				keyrelease(int key, t_data *data);
int 				keypress(int key, t_data *data);
int					mouse_move(int x, int y, t_data *data);
int					mouse_down(int button, t_data *data);

void 				update_player(t_data *data, float delta_time);
void 				update_flashlight(t_data *data, float delta_time);

void 				init(t_data *data);
void				init_images(t_data *data);

int 				destroy(t_data *data);
void				duke_error(t_data *data, const char *error);



void 				draw_minimap2(t_data *data);
void 				draw_minimap(t_data *data);



void				get_map(t_data *data, t_map *map, char *path);
float				get_wall_height(t_map *map, int x, int y);
bool				is_walkable(t_player *player, t_map *map, int x, int y);
bool				is_out_of_map(t_map *map, int x, int y);



void				ft_mlx_set_pixel(t_img *img, int x, int y, int color);
unsigned int		ft_mlx_get_pixel(t_img *img, int x, int y);
void				ft_mlx_put_img_to_img_scaled(t_img *dst, t_img *src);
void				ft_mlx_put_img_to_img(t_img *dst, t_img *src, int x, int y);
void				ft_mlx_draw_square(t_img *dest, int start_x, int start_y, int square_width, int square_height, int color);
void				ft_mlx_draw_circle(t_img *dest, int center_x, int center_y, int radius, int color);
void				ft_mlx_draw_line(t_img *dest, int start_x, int start_y, int length, float angle, int color);
t_img				ft_mlx_new_img(void *mlx, int width, int height);
t_img				ft_mlx_new_img_xpm(void *mlx, char *path);

void				ft_draw_square(unsigned int *pixel_buffer, int buffer_lenght, int start_x, int start_y, int square_width, int square_height, int color);
void				ft_draw_circle(unsigned int *pixel_buffer, int buffer_lenght, int center_x, int center_y, int radius, int color);
unsigned int 		apply_light_modifier(unsigned int color, float light_factor, int light_r, int light_g, int light_b);



void				raycasting(t_data *data, t_map *map);
void				floorcasting(t_data *data);

size_t				ft_gettime();
size_t				ft_gettime_us();
float				finterp(float start, float end, float speed, float delta_time);
int					ft_iclamp(int value, int min, int max);





#endif