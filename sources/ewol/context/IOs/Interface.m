/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#import <UIKit/UIKit.h>
#import <ewol/context/IOs/AppDelegate.h>
//#import "AppDelegate.h"



int mm_main(int argc, const char *argv[]) {
	@autoreleasepool {
	    return UIApplicationMain(argc, (char**)argv, nil, NSStringFromClass([AppDelegate class]));
	}	// return no error
	return 0;
}

void mm_exit(void) {
	[[NSThread mainThread] cancel];
}

void mm_openURL(const char *_url) {
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:_url]];
}

