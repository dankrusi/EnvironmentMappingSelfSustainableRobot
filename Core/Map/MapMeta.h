/*
 * MapMeta.h
 *
 *  Created on: Mar 10, 2009
 *      Author: dankrusi
 */

#ifndef MAPMETA_H_
#define MAPMETA_H_

class MapMeta {

public:
	bool show;

public:
	MapMeta(bool show) { this->show = show; };
	virtual ~MapMeta() {};
};

#endif /* MAPMETA_H_ */
