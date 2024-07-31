#ifndef __HERO_ENGINE_HOST_NAME_H__
#define __HERO_ENGINE_HOST_NAME_H__

typedef void(*asyn_host_name_callback)(int ip, void *userdata);

typedef unsigned int(*asyn_host_name_get_net_id)();

int hero_asyn_host_by_name(char *name, asyn_host_name_callback callback, void *userdata, asyn_host_name_get_net_id netidcallback);

#endif
