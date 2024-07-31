
#ifndef MMIJAVA_PRELOAD_H
#define MMIJAVA_PRELOAD_H

void javacall_preloader_check_n_install(void);
javacall_bool javacall_preloader_is_preload_midlet(int suiteId);
void MMIJAVA_LAUNCH_PRELOAD_MIDLET(char *midletName);

#endif //MMIJAVA_PRELOAD_H
