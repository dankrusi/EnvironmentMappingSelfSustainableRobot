/*
 *  ObjectMap.cpp
 *
 *  ===========================================================================
 *
 *  Copyright 2008-2009 Daniel Kruesi (Dan Krusi) and David Grob
 *
 *  This file is part of the emms framework.
 *
 *  The emms framework  is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The emms framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this software. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ObjectMap.h"

#include "../Object/Object.h"
#include "../Library/Util.h"

ObjectMap::ObjectMap(Core *core) : Map("Object", core) {
	this->objects = new QList<Object*>();
}

ObjectMap::~ObjectMap() {
	delete objects;
}

void ObjectMap::paint(QPainter &painter, QRect view) {
	// Loop all objects and call thier paint() method
	for(int i = 0; i < objects->count(); i++){
		Util::resetPainter(painter);
		objects->at(i)->paint(painter, view, core->scale);
	}
}

Core::AreaType ObjectMap::checkForCollision(long x, long y, Object *exclude)
{
	for(int i = 0; i < objects->count(); i++) {
		if(objects->at(i) != exclude && objects->at(i)->checkForCollision(x,y) == true) return Core::AreaTypeCollision;
	}
	return Core::AreaTypeOpen;
}

long ObjectMap::mostDistantObjectX() {
	// Return the most distant x coordinate of all objects...
	long maxWidth = 0;
	for(int i = 0; i < objects->count(); i++){
		Object *obj = objects->at(i);
		if(obj->x + obj->width > maxWidth) maxWidth = obj->x + obj->width;
	}
	return maxWidth;
}

long ObjectMap::mostDistantObjectY() {
	// Return the most distant y coordinate of all objects...
	long maxHeight = 0;
	for(int i = 0; i < objects->count(); i++){
		Object *obj = objects->at(i);
		if(obj->y + obj->height > maxHeight) maxHeight = obj->y + obj->height;
	}
	return maxHeight;
}

long ObjectMap::width() {
	return 0;
}

long ObjectMap::height() {
	return 0;
}

