#ifndef SSS_EVENT_LIST_HPP
#define SSS_EVENT_LIST_HPP

#include "_includes.hpp"

/** @file
 *  Defines time-related functions and classes.
 */

SSS_BEGIN;

enum EventList
{
	Model,
	Alpha,
	TexOffset,
	Content,
	Resize,
	Hover,
	Leave,
	Clicked,
	Held,
	LastEvent
};

SSS_END;

#endif // SSS_EVENT_LIST_HPP