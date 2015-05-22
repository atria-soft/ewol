/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license APACHE v2.0 (see license file)
 */

#ifndef __EWOL_MM_INTERFACE_H__
#define __EWOL_MM_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

int mm_main(int _argc, const char *_argv[]);
void mm_exit();
void mm_openURL(const char *_url);

#ifdef __cplusplus
}
#endif

#endif
