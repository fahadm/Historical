#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char DATE[] = "04";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.06";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 3;
	static const long MINOR = 7;
	static const long BUILD = 308;
	static const long REVISION = 1736;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 749;
	#define RC_FILEVERSION 3,7,308,1736
	#define RC_FILEVERSION_STRING "3, 7, 308, 1736\0"
	static const char FULLVERSION_STRING[] = "3.7.308.1736";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 8;
	

#endif //VERSION_H
