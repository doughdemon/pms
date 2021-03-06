/* vi:set ts=8 sts=8 sw=8 noet:
 *
 * PMS	<<Practical Music Search>>
 * Copyright (C) 2006-2015  Kim Tore Jensen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>

#include "list.h"

ListItem::ListItem(List * owner)
{
	assert(owner);
	list = owner;
	selected_ = false;
}

ListItem::~ListItem()
{
}

void
ListItem::set_selected(bool state)
{
	if (state == selected_) {
		return;
	}
	selected_ = state;
	list->set_selection_cache_valid(false);
}

bool
ListItem::selected()
{
	return selected_;
}

bool
ListItem::match(string term, long flags)
{
	return false;
}
