/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nweber <nweber@student.42Heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 12:45:19 by anakin            #+#    #+#             */
/*   Updated: 2025/11/12 15:35:05 by nweber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_rt.h"

/**
 * Prints a descriptive error message to stderr and returns the provided code.
 * If msg is non-NULL the output is: "<msg>: <system error string>\n".
 * If msg is NULL only the system error string is printed.
 * @param msg human readable context for the error (may be NULL)
 * @param error_code integer value to return to the caller
 * @return error_code
 */
int	ft_error(char *msg, int error_code)
{
	perror(msg);
	return (error_code);
}
