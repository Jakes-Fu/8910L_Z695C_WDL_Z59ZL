import os
import sys
import string
import filecmp

# Go to script directory
script_dir = os.path.dirname(sys.argv[0].replace("\\", os.sep).replace("/", os.sep))
if script_dir:
	os.chdir(script_dir)

# Open the main header file.
headers = []

op_dev_header = open('./OpDevInternal.h', 'r')
for itr in op_dev_header:
	if itr.find('#include') != -1 and itr.find('OpDev')!= -1:
		for header in itr.split('"'):
			if header.find('.h') != -1:
				headers.append(header)\

op_dev_header.close()

# Read all the headers and get all the functions out
func_list = []
for itr in headers:
	header_file = open('./'+itr, 'r')
	funcion = []
	func_line = 0
	for line in header_file:
		if (line.find("OD_MMI_TASK") != -1 or \
		   line.find("OD_CORE_TASK") != -1 or \
		   line.find("OD_ALL_TASK") != -1) and \
		   line.find("typedef") == -1:
			func_line  = 1
		if (func_line != 0):
			#strip the comments
			if line.find('//') != -1:
				line = line.split('//')[0]
			line = line.lstrip(string.whitespace)
			line = line.rstrip(string.whitespace)
			funcion.append(line)
			func_line = func_line + 1
		if line.find(";") !=-1 and func_line != 0:
			func_line = 0
			func_list.append("".join(funcion))
			funcion = []
	header_file.close()

# Generate header files
header_file_for_sim = open('./OpDevSim.h.tmp', 'w+')
header_file_for_func_ren = open('./OperaFuncRename.h.tmp', 'w+')

# Write the headers
header_file_for_sim.write('''/** Generated file, DO NOT edit it manually */
#ifndef OPDEVSIM_H
#define OPDEVSIM_H

#ifdef __cplusplus
extern "C" {
#endif

''')
header_file_for_func_ren.write('''/** Generated file, DO NOT edit it manually */
#ifndef OPERAFUNCRENAME_H
#define OPERAFUNCRENAME_H

#ifdef __cplusplus
extern "C" {
#endif

// OD_API_RENAME_PREFIX is a prefix of the APIs, for example, OPERA_MINI_
#ifdef OD_API_RENAME_PREFIX

/*
* FIXME: Currently OD_API_RENAME_PREFIX is not used when rename symbols,
* because following code don't work on ADS12:
* 
* #define OD_API_RENAME_PREFIX _OperaMini_
* #define OPERA_API_RENAME(x, y) x##y
* #define OD_FileOpen OPERA_API_RENAME(OD_API_RENAME_PREFIX, OD_FileOpen)
* 
* OD_FileOpen will be replaced to "_OperaMini_  OD_FileOpen". It will add some spaces btween the two names.
* 
* So we hardcoded to "#define OPERA_API_RENAME(x)	_OperaMini_##x"
*/
	#undef OPERA_API_RENAME
	#define OPERA_API_RENAME(x)	_OperaMini_##x

''')

# Write the decalarations of function pointers
function_name_list = []
for itr in func_list:
	# Split the parameter list out
	function_comp = itr.split('(')
	function_param = '('+function_comp[1]
	# extact the name out
	function_sec = function_comp[0].split();
	sec_len = len(function_sec)
	function_name = function_sec[sec_len-1]
	function_name_list.append(function_name)
	function_rval = " ".join(function_sec[0:sec_len-1])
	header_file_for_sim.write("typedef "+ function_rval+" (*"+function_name+"_func_t)"+function_param + "\n")

# Write the struct that contains all the functions
header_file_for_sim.write('''
struct OpDevFunc
{
''')

for itr in function_name_list:
	header_file_for_sim.write("	"+itr+"_func_t  __"+itr+";\n")

header_file_for_sim.write("};\n\n")

# Write the macros that override function defines
for itr in function_name_list:
	header_file_for_sim.write("#define "+itr+"  getOpDevFuncs()->__"+itr+"\n")
	header_file_for_func_ren.write("	#define %s OPERA_API_RENAME(%s)\n" % (itr, itr))

# Write the footer
header_file_for_sim.write('''
extern struct OpDevFunc* getOpDevFuncs();
extern void   OD_InitFuncs();

#ifdef __cplusplus
}
#endif

#endif // OPDEVSIM_H
''')
header_file_for_func_ren.write('''
#endif // OD_API_RENAME_PREFIX

#ifdef __cplusplus
}
#endif

#endif // OPERAFUNCRENAME_H
''')

header_file_for_sim.close()
header_file_for_func_ren.close()


# Write the c file
c_file_for_sim = open('./OpDevSim.c.tmp', 'w+')
c_file_for_sim.write('''/** Generated file, DO NOT edit it manually */
#include "OpDev.h"

struct OpDevFunc* g_od_funcs = NULL;
struct OpDevFunc g_opdevfuncs;

struct OpDevFunc* getOpDevFuncs()
{
	return &g_opdevfuncs;
}
''')

for itr in function_name_list:
	c_file_for_sim.write("#undef "+itr+"\n")

c_file_for_sim.write('''void OD_InitFuncs()
{
	g_od_funcs = &g_opdevfuncs;
	memset(g_od_funcs, 0, sizeof(struct OpDevFunc));
''')

for itr in function_name_list:
	c_file_for_sim.write("	g_od_funcs->__"+itr+" = "+itr+";\n")

c_file_for_sim.write("}\n")
c_file_for_sim.close()

# Only touch the target file when its content need to update

def rename_if_diff (fromfile, tofile):
	if ((not os.path.exists(tofile)) or (not filecmp.cmp(fromfile, tofile))):
		os.rename(fromfile, tofile)
		print tofile + " updated"

rename_if_diff('./OpDevSim.h.tmp', './OpDevSim.h')
rename_if_diff('./OpDevSim.c.tmp', './OpDevSim.c')
rename_if_diff('./OperaFuncRename.h.tmp', './OperaFuncRename.h')

try:
	os.remove('./OpDevSim.h.tmp')
	os.remove('./OpDevSim.c.tmp')
	os.remove('./OperaFuncRename.h.tmp')
except:
	pass
