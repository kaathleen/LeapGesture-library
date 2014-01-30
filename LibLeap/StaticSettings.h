/*
 * StaticSettings.h
 *
 *  Created on: 29-01-2014
 *      Author: oli
 */

#ifndef STATICSETTINGS_H_
#define STATICSETTINGS_H_

class StaticSettings {
public:
	StaticSettings();
	StaticSettings(char* path, char* name);

	char* path;
	char* name;


};

#endif /* STATICSETTINGS_H_ */
