# Pebble-Binary-101
JIFFFFFFF

Setting top to                           : /tmp/tmppMYZxB 
Setting out to                           : /tmp/tmppMYZxB/build 
Found Pebble SDK for basalt in:          : /app/sdk3/Pebble/basalt 
Checking for program gcc,cc              : arm-none-eabi-gcc 
Checking for program ar                  : arm-none-eabi-ar 
Found Pebble SDK for aplite in:          : /app/sdk3/Pebble/aplite 
Checking for program gcc,cc              : arm-none-eabi-gcc 
Checking for program ar                  : arm-none-eabi-ar 
'configure' finished successfully (0.118s)
Waf: Entering directory `/tmp/tmppMYZxB/build'
[ 1/30] Start build for basalt: 
[ 2/30] subst: ../../app/sdk3/Pebble/common/pebble_app.ld.template -> build/basalt/pebble_app.ld.auto
[ 3/30] appinfo.auto.c: appinfo.json -> build/basalt/appinfo.auto.c
[ 4/30] app_resources.pbpack.table: ../../app/sdk3/Pebble/common/tools/pbpack_meta_data.py -> build/basalt/app_resources.pbpack.table
[ 5/30] app_resources.pbpack.manifest: ../../app/sdk3/Pebble/common/tools/pbpack_meta_data.py -> build/basalt/app_resources.pbpack.manifest
[ 6/30] app_resources.pbpack.data: ../../app/sdk3/Pebble/common/tools/pbpack_meta_data.py -> build/basalt/app_resources.pbpack.data
[ 7/30] resource_ids.auto.h: ../../app/sdk3/Pebble/common/tools/generate_resource_code.py -> build/basalt/src/resource_ids.auto.h
[ 8/30] c: build/basalt/appinfo.auto.c -> build/basalt/appinfo.auto.c.17.o
[ 9/30] c: src/Jif.c -> build/src/Jif.c.17.o
[10/30] app_resources.pbpack: build/basalt/app_resources.pbpack.manifest build/basalt/app_resources.pbpack.table build/basalt/app_resources.pbpack.data -> build/basalt/app_resources.pbpack
../src/Jif.c:83:22: error: 't' undeclared here (not in a function)
../src/Jif.c:86:36: error: expected ')' before 'sizeof'
../src/Jif.c:87:33: error: expected ')' before 'sizeof'
../src/Jif.c:88:1: error: expected identifier or '(' before '}' token
../src/Jif.c:81:15: warning: 'seconds_singles_buffer' defined but not used [-Wunused-variable]
../src/Jif.c:82:15: warning: 'seconds_tens_buffer' defined but not used [-Wunused-variable]
Waf: Leaving directory `/tmp/tmppMYZxB/build'
Build failed
 -> task in 'basalt/pebble-app.elf' failed (exit status 1): 
	{task 139941646319888: c Jif.c -> Jif.c.17.o}
['arm-none-eabi-gcc', '-std=c99', '-mcpu=cortex-m3', '-mthumb', '-ffunction-sections', '-fdata-sections', '-g', '-Os', '-D_TIME_H_', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter', '-Wno-error=unused-function', '-Wno-error=unused-variable', '-fPIE', '-I/tmp/app/sdk3/Pebble/basalt/include', '-I/app/sdk3/Pebble/basalt/include', '-I/tmp/tmppMYZxB/build', '-I/tmp/tmppMYZxB', '-I/tmp/tmppMYZxB/build/src', '-I/tmp/tmppMYZxB/src', '-I/tmp/tmppMYZxB/build/basalt', '-I/tmp/tmppMYZxB/basalt', '-DRELEASE', '-DPBL_PLATFORM_BASALT', '-DPBL_COLOR', '-D__FILE_NAME__="Jif.c"', '../src/Jif.c', '-c', '-o', 'src/Jif.c.17.o']
[ERROR   ] A compilation error occurred
