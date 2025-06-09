/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flashlight.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: injah <injah@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 21:57:10 by injah             #+#    #+#             */
/*   Updated: 2025/06/06 11:33:28 by injah            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	update_flashlight(t_data *data, float delta_time)
{
	static float	time = 0;

	if (data->flashlight.equiped)
	{
		time += delta_time;
		data->flashlight.pos.y = finterp(data->flashlight.pos.y, data->flashlight.pos.y + sin(time / 3) * 5, 1, delta_time);
		data->flashlight.pos.x = finterp(data->flashlight.pos.y, data->flashlight.pos.y + cos(time / 2) * 10, 1, delta_time);
	}
}