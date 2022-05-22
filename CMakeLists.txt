cmake_minimum_required(VERSION 3.22)
project(BRender LANGUAGES C ASM_MASM VERSION 1.1.2)

set(CMAKE_C_STANDARD 17)

set(CMAKE_MSVC_RUNTIME_LIBRARY "")

# NOTE(bjrkk): hacky as hell but you know what I don't care
add_compile_definitions(
	STATIC=static
	$<$<PLATFORM_ID:Windows>:_CRT_SECURE_NO_WARNINGS>)

add_compile_definitions(
	BASED_FIXED=1
	$<$<COMPILE_LANG_AND_ID:C,MSVC>:__VISUALC__=1>
	$<$<COMPILE_LANG_AND_ID:C,MSVC>:__TARGET_MSC__=1>)

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
	PUBLIC
		"${PROJECT_SOURCE_DIR}/INC"
	PRIVATE
		"${PROJECT_SOURCE_DIR}/FW")

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

	"${PROJECT_SOURCE_DIR}/FW/fixed386.asm"
	"${PROJECT_SOURCE_DIR}/FW/fxadc386.asm"
	"${PROJECT_SOURCE_DIR}/FW/blockops.asm"
	"${PROJECT_SOURCE_DIR}/FW/magicsym.asm"
	"${PROJECT_SOURCE_DIR}/FW/fnt3x5.asm"
	"${PROJECT_SOURCE_DIR}/FW/fntp4x6.asm"
	"${PROJECT_SOURCE_DIR}/FW/fntp7x9.asm"
	"${PROJECT_SOURCE_DIR}/FW/memloops.asm"
)
target_include_directories(FW
	PUBLIC
		"${PROJECT_SOURCE_DIR}/INC")

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

	"${PROJECT_SOURCE_DIR}/ZB/mesh386.asm"
	"${PROJECT_SOURCE_DIR}/ZB/safediv.asm"
	"${PROJECT_SOURCE_DIR}/ZB/sar16.asm"
	"${PROJECT_SOURCE_DIR}/ZB/t_piza.asm"
	"${PROJECT_SOURCE_DIR}/ZB/t_piza2.asm"
	"${PROJECT_SOURCE_DIR}/ZB/ti8_piz.asm"
	"${PROJECT_SOURCE_DIR}/ZB/ti8_pizp.asm"
	"${PROJECT_SOURCE_DIR}/ZB/tt15_piz.asm"
	"${PROJECT_SOURCE_DIR}/ZB/tt24_piz.asm"
)

target_include_directories(ZB
	PUBLIC
		"${PROJECT_SOURCE_DIR}/INC"
		"${PROJECT_SOURCE_DIR}/FW")