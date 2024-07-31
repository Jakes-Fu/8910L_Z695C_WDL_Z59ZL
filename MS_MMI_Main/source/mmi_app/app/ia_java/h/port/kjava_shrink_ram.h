#ifndef KJAVA_SHRINK_RAM_H
#define KJAVA_SHRINK_RAM_H

#define DEFINE_SHRINK_VAR_1_DIM(type, var)   type* var;
#define DEFINE_SHRINK_VAR_2_DIM(type, var)   type** var;

#define DEFINE_STATIC_SHRINK_VAR_1_DIM(type, var)  static type* var;
#define DEFINE_STATIC_SHRINK_VAR_2_DIM(type, var)  static type** var;

#define BEGIN_DEFINE_SHRINK_VAR_INIT(filename) void shrink_ram_init_##filename() {
#define SHRINK_VAR_MEMORY_1_DIM(type, var, length)\
	if(!var)\
	{\
		var = (type*)SCI_ALLOC_APP(sizeof(type)*(length));\
		memset(var,0,sizeof(type)*(length));\
	}
#define SHRINK_VAR_MEMORY_2_DIM(type, var, length1, length2) \
	if(!var)\
	{\
		var = (type**)SCI_ALLOC_APP(sizeof(type)*(length1)*(length2));\
		memset(var,0,sizeof(type)*(length1)*(length2));\
	}
#define END_DEFINE_SHRINK_VAR_INIT     }


#define BEGIN_DEFINE_SHRINK_VAR_FINI(filename) void shrink_ram_fini_##filename(){
#define FREE_SHRINK_VAR_MEMORY(var) \
	if(var) \
	{\
		SCI_FREE(var); \
		var = PNULL;\
	}
#define END_DEFINE_SHRINK_VAR_FINI  }

#define CALL_SHRINK_VAR_INIT(filename) shrink_ram_init_##filename();
#define CALL_SHRINK_VAR_FINI(filename) shrink_ram_fini_##filename();

#endif