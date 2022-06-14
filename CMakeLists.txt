cmake_minimum_required(VERSION 3.22)
project(BRender LANGUAGES C ASM_MASM VERSION 1.1.2)

set (CMAKE_C_STANDARD 17)
set (CMAKE_MSVC_RUNTIME_LIBRARY "")
set (CMAKE_C_EXTENSIONS OFF)

option (BRENDER_USE_FIXED_POINT 
	"This determines if BRender should prefer fixed-point math routines over floating-point maths." 
	ON)

add_compile_definitions (
	$<$<PLATFORM_ID:Windows>:_CRT_SECURE_NO_WARNINGS>)

if (CMAKE_C_BYTE_ORDER MATCHES BIG_ENDIAN)
	add_compile_definitions ("BR_ENDIAN_BIG=1")
else()
	add_compile_definitions ("BR_ENDIAN_BIG=0")
endif()

if (CMAKE_C_BYTE_ORDER MATCHES LITTLE_ENDIAN)
	add_compile_definitions ("BR_ENDIAN_LITTLE=1")
else()
	add_compile_definitions ("BR_ENDIAN_LITTLE=0")
endif ()

if (BRENDER_USE_FIXED_POINT)
	add_compile_definitions(BASED_FIXED=1)
	set (BRENDER_LIBRAY_SUFFIX "xr")
else ()
	add_compile_definitions(BASED_FLOAT=1)
	set (BRENDER_LIBRAY_SUFFIX "fr")
endif ()

include_directories("${PROJECT_SOURCE_DIR}/INC")

add_library(FW)
target_sources(FW PRIVATE 
	"${PROJECT_SOURCE_DIR}/FW/actsupt.c"
	"${PROJECT_SOURCE_DIR}/FW/angles.c"
	"${PROJECT_SOURCE_DIR}/FW/brlists.c"
	"${PROJECT_SOURCE_DIR}/FW/brqsort.c"
	"${PROJECT_SOURCE_DIR}/FW/bswap.c"
	"${PROJECT_SOURCE_DIR}/FW/custsupt.c"
	"${PROJECT_SOURCE_DIR}/FW/datafile.c"
	"${PROJECT_SOURCE_DIR}/FW/def_mdl.c"
	"${PROJECT_SOURCE_DIR}/FW/def_mat.c"
	"${PROJECT_SOURCE_DIR}/FW/diag.c"
	"${PROJECT_SOURCE_DIR}/FW/envmap.c"
	"${PROJECT_SOURCE_DIR}/FW/error.c"
	"${PROJECT_SOURCE_DIR}/FW/ffhooks.c"
	"${PROJECT_SOURCE_DIR}/FW/file.c"
	"${PROJECT_SOURCE_DIR}/FW/fileops.c"
	"${PROJECT_SOURCE_DIR}/FW/fixed.c"
	"${PROJECT_SOURCE_DIR}/FW/fontptrs.c"
	"${PROJECT_SOURCE_DIR}/FW/fwsetup.c"
	"${PROJECT_SOURCE_DIR}/FW/light8.c"
	"${PROJECT_SOURCE_DIR}/FW/light8o.c"
	"${PROJECT_SOURCE_DIR}/FW/light24.c"
	"${PROJECT_SOURCE_DIR}/FW/light24o.c"
	"${PROJECT_SOURCE_DIR}/FW/logprint.c"
	"${PROJECT_SOURCE_DIR}/FW/matrix23.c"
	"${PROJECT_SOURCE_DIR}/FW/matrix34.c"
	"${PROJECT_SOURCE_DIR}/FW/matrix4.c"
	"${PROJECT_SOURCE_DIR}/FW/mem.c"
	"${PROJECT_SOURCE_DIR}/FW/onscreen.c"
	"${PROJECT_SOURCE_DIR}/FW/pick.c"
	"${PROJECT_SOURCE_DIR}/FW/pixelmap.c"
	"${PROJECT_SOURCE_DIR}/FW/pmdsptch.c"
	"${PROJECT_SOURCE_DIR}/FW/pmgenops.c"
	"${PROJECT_SOURCE_DIR}/FW/pmmemops.c"
	"${PROJECT_SOURCE_DIR}/FW/pool.c"
	"${PROJECT_SOURCE_DIR}/FW/prelight.c"
	"${PROJECT_SOURCE_DIR}/FW/prepmatl.c"
	"${PROJECT_SOURCE_DIR}/FW/prepmesh.c"
	"${PROJECT_SOURCE_DIR}/FW/prepmap.c"
	"${PROJECT_SOURCE_DIR}/FW/preptab.c"
	"${PROJECT_SOURCE_DIR}/FW/quat.c"
	"${PROJECT_SOURCE_DIR}/FW/quantize.c"
	"${PROJECT_SOURCE_DIR}/FW/register.c"
	"${PROJECT_SOURCE_DIR}/FW/regsupt.c"
	"${PROJECT_SOURCE_DIR}/FW/resource.c"
	"${PROJECT_SOURCE_DIR}/FW/scalar.c"
	"${PROJECT_SOURCE_DIR}/FW/scale.c"
	"${PROJECT_SOURCE_DIR}/FW/scratch.c"
	"${PROJECT_SOURCE_DIR}/FW/scrstr.c"
	"${PROJECT_SOURCE_DIR}/FW/surface.c"
	"${PROJECT_SOURCE_DIR}/FW/transfrm.c"
	"${PROJECT_SOURCE_DIR}/FW/vector.c"

	"${PROJECT_SOURCE_DIR}/FW/fixed_agnostic.c"
	"${PROJECT_SOURCE_DIR}/FW/blockops.c"
	"${PROJECT_SOURCE_DIR}/FW/font_f_3x5.c"
	"${PROJECT_SOURCE_DIR}/FW/font_p_4x6.c"
	"${PROJECT_SOURCE_DIR}/FW/font_p_7x9.c"
	"${PROJECT_SOURCE_DIR}/FW/memloops.c"
)

add_library(ZB)
target_sources(ZB PRIVATE
	"${PROJECT_SOURCE_DIR}/ZB/awtmz.c"
	"${PROJECT_SOURCE_DIR}/ZB/bbox.c"
	"${PROJECT_SOURCE_DIR}/ZB/decalz.c"
	"${PROJECT_SOURCE_DIR}/ZB/dither.c"
	"${PROJECT_SOURCE_DIR}/ZB/frcp.c"
	"${PROJECT_SOURCE_DIR}/ZB/l_piz.c"
	"${PROJECT_SOURCE_DIR}/ZB/p_piz.c"
	"${PROJECT_SOURCE_DIR}/ZB/perspz.c"
	"${PROJECT_SOURCE_DIR}/ZB/zbclip.c"
	"${PROJECT_SOURCE_DIR}/ZB/zbmatl.c"
	"${PROJECT_SOURCE_DIR}/ZB/zbmesh.c"
	"${PROJECT_SOURCE_DIR}/ZB/zbmeshe.c"
	"${PROJECT_SOURCE_DIR}/ZB/zbmeshp.c"
	"${PROJECT_SOURCE_DIR}/ZB/zbrendr.c"
	"${PROJECT_SOURCE_DIR}/ZB/zbsetup.c"
	
	"${PROJECT_SOURCE_DIR}/ZB/mesh_agnostic.c"
	"${PROJECT_SOURCE_DIR}/ZB/safediv.c"
	"${PROJECT_SOURCE_DIR}/ZB/t_piza.c"
	"${PROJECT_SOURCE_DIR}/ZB/ti8_pizp.c"
	"${PROJECT_SOURCE_DIR}/ZB/ti8_piz.c"
	"${PROJECT_SOURCE_DIR}/ZB/tt15_piz.c"
	"${PROJECT_SOURCE_DIR}/ZB/tt24_piz.c"
)

target_include_directories(ZB
	PRIVATE
		"${PROJECT_SOURCE_DIR}/FW")

add_library(FMT)
target_sources(FMT PRIVATE
	"${PROJECT_SOURCE_DIR}/FMT/loadnff.c"
	"${PROJECT_SOURCE_DIR}/FMT/loadasc.c"
	"${PROJECT_SOURCE_DIR}/FMT/loadscr.c"
	"${PROJECT_SOURCE_DIR}/FMT/loadgif.c"
	"${PROJECT_SOURCE_DIR}/FMT/loadiff.c"
	"${PROJECT_SOURCE_DIR}/FMT/loadbmp.c"
	"${PROJECT_SOURCE_DIR}/FMT/loadtga.c"
	"${PROJECT_SOURCE_DIR}/FMT/savescr.c"
)
target_include_directories(FMT
	PRIVATE
		"${PROJECT_SOURCE_DIR}/FW")

add_library(STD)
target_sources(STD PRIVATE
	"${PROJECT_SOURCE_DIR}/STD/stdfile.c"
	"${PROJECT_SOURCE_DIR}/STD/stdmem.c"
	"${PROJECT_SOURCE_DIR}/STD/stddiag.c"
)
target_include_directories(STD
	PRIVATE
		"${PROJECT_SOURCE_DIR}/FW")

set_property(TARGET FW PROPERTY OUTPUT_NAME "brfwm${BRENDER_LIBRAY_SUFFIX}")
set_property(TARGET ZB PROPERTY OUTPUT_NAME "brzbm${BRENDER_LIBRAY_SUFFIX}")
set_property(TARGET FMT PROPERTY OUTPUT_NAME "brfmm${BRENDER_LIBRAY_SUFFIX}")
set_property(TARGET STD PROPERTY OUTPUT_NAME "brstm${BRENDER_LIBRAY_SUFFIX}")