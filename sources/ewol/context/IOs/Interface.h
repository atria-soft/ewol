/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EWOL_MM_INTERFACE_H__
#define __EWOL_MM_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

int mm_main(int argc, const char *argv[]);
void mm_exit();
void mm_openURL(const char *_url);

#ifdef __cplusplus
}
#endif

#endif
