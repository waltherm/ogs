

set(SOURCES
	mo_canopy_interc.f90
	mo_file.f90
	mo_global_variables.f90
	mo_init_states.f90
	mo_meteo_forcings.f90
	mo_mhm.f90
	mo_mhm_constants.f90
	mo_mhm_eval.f90
	mo_mpr_runoff.f90
	mo_mpr_smhorizons.f90
	mo_mpr_soilmoist.f90
	mo_multi_param_reg.f90
	mo_net_startup.f90
	mo_objective_function.f90
	mo_pet.f90
#	mo_prepare_gridded_LAI.f90
	mo_prepare_gridded_lai.f90
	mo_radiation.f90
	mo_read_config.f90
#	mo_read_lai.f90
	mo_read_latlon.f90
	mo_read_lut.f90
	mo_read_meteo.f90
	mo_read_spatial_data.f90
	mo_read_timeseries.f90
	mo_read_wrapper.f90
#	mo_refer_et.f90
	mo_restart.f90
	mo_routing.f90
	mo_runoff.f90
	mo_set_netcdf_outputs.f90
#	mo_set_netcdf_restart.f90
	mo_signatures.f90
	mo_snow_accum_melt.f90
	mo_soil_database.f90
	mo_soil_moisture.f90
	mo_spatial_agg_disagg_forcing.f90
	mo_startup.f90
	mo_template.f90
	mo_temporal_disagg_forcing.f90
	mo_upscaling_operators.f90
	mo_write_ascii.f90
	mo_write_fluxes_states.f90
)

add_library(libmhm ${SOURCES})
add_dependencies(libmhm mhm-base)

#add_executable(mhm mhm_driver.f90)
add_library(mhm mhm_driver.f90)
add_dependencies(mhm libmhm)


# Enable C preprocessor on all files to enable #ifdefs
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS -cpp)

#target_link_libraries (mhm libmhm mhm-base ${NETCDF_LIBRARIES})
#SET(mhm_module mhm libmhm mhm-base ${NETCDF_LIBRARIES} CACHE STRING "mhm libraries" FORCE) 
