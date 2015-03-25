

set(SOURCES
	mo_anneal.f90
	mo_append.f90
	mo_constants.f90
	mo_dds.f90
	mo_errormeasures.f90
	mo_finish.f90
	mo_julian.f90
	mo_kind.f90
	mo_linfit.f90
	mo_mcmc.f90
	mo_message.f90
	mo_moment.f90
	mo_ncread.f90
	mo_ncwrite.f90
	mo_nml.f90
	mo_orderpack.f90
	mo_sce.f90
#	mo_sort.f90
	mo_string_utils.f90
	mo_timer.f90
	mo_utils.f90
	mo_xor4096.f90
)

add_library(mhm-base ${SOURCES})

# Enable C preprocessor on all files to enable #ifdefs
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS -cpp)

target_link_libraries (mhm-base 
	${NETCDF_LIBRARIES}
	gfortran
)
