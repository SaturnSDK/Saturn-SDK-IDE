#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.09";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 9;
	static const long BUILD = 851;
	static const long REVISION = 4628;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 1514;
	#define RC_FILEVERSION 0,9,851,4628
	#define RC_FILEVERSION_STRING "0, 9, 851, 4628\0"
	static const char FULLVERSION_STRING[] = "0.9.851.4628";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 51;
	

}
#endif //VERSION_H
