#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "06";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.02";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 7;
	static const long BUILD = 655;
	static const long REVISION = 8357;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1325;
	#define RC_FILEVERSION 1,7,655,8357
	#define RC_FILEVERSION_STRING "1, 7, 655, 8357\0"
	static const char FULLVERSION_STRING[] = "1.7.655.8357";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 28;
	

}
#endif //VERSION_H
