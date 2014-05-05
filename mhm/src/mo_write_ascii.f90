!> \file mo_write_ascii.f90

!> \brief Module to write ascii file output.

!> \details Module to write ascii file output.\n
!>          Writing model output to ASCII should be the exception. Therefore, output is written usually as NetCDF
!>          and only:\n
!>          (1) The configuration file of mHM, \n
!>          (2) the final parameter set after optimization, and \n
!>          (3) the simulated vs. observed daily discharge \n
!>          is written in ASCII file format to allow for a quick assurance of proper model runs.

!> \authors Christoph Schneider, Juliane Mai, Luis Samaniego
!> \date May 2013

MODULE mo_write_ascii

  ! This module is a template for the UFZ CHS mesoscale hydrologic model mHM.

  ! Written  Christoph Schneider, May 2013
  ! Modified, Juliane Mai,        May 2013 - module version and documentation
  ! Modified, Luis Samaniego,     Nov 2013 - improving all formats  

  USE mo_kind, ONLY: i4, dp
  IMPLICIT NONE

  PUBLIC :: write_configfile                   ! Writes configuration file
  PUBLIC :: write_optifile                     ! Write final OF and best parameter set
  PUBLIC :: write_optinamelist                 ! Write final OF and best parameter set in a namelist format
  PUBLIC :: write_daily_obs_sim_discharge      ! Writes daily water balance file  
  ! ------------------------------------------------------------------

  !      NAME
  !         write_configfile

  !     PURPOSE
  !>        \brief This modules writes the results of the configuration into an ASCII-file

  !>        \details 

  !     INTENT(IN)
  !         None

  !     INTENT(INOUT)
  !         None

  !     INTENT(OUT)
  !         None

  !     INTENT(IN), OPTIONAL
  !         None

  !     INTENT(INOUT), OPTIONAL
  !         None

  !     INTENT(OUT), OPTIONAL
  !         None

  !     RETURN
  !         None

  !     RESTRICTIONS
  !         None

  !     EXAMPLE
  !         

  !     LITERATURE
  !         

  !     HISTORY
  !>        \author Christoph Schneider
  !>        \date May 2013
  !         Modified, Juliane Mai, May 2013 - module version and documentation

  PRIVATE

  ! ------------------------------------------------------------------

CONTAINS

  Subroutine write_configfile()

    use mo_kind,             only: i4, dp
    use mo_mhm_constants,    only: nodata_dp
    use mo_message,          only: message
    use mo_string_utils,     only: num2str
    USE mo_file,             only: file_config, uconfig
    use mo_global_variables, only:                     &
         processMatrix,             &
         nBasins,                   &
         basin,                     &
         gauge,                     &
         L0_nCells,                 &
         L1_nCells,                 &
         L11_nCells,                &
         L11_netPerm,               &
         L11_fromN,                 &
         L11_toN,                   &
         L11_rOrder,                &
         L11_label,                 &
         L11_length,                &         
         L11_slope,                 &
         L11_ID,                    &
         L1_L11_ID,                 &
         L1_areaCell,               &
         nGaugesTotal,              &
         timeStep,                  &
         resolutionHydrology,       &
         resolutionRouting,         &  
         restart_flag_states_read,  &
         restart_flag_states_write, &
         restart_flag_config_read,  &
         restart_flag_config_write, &
         dirConfigOut,              &
         dirMorpho,                 &
         dirLCover,                 &
         dirGauges,                 &
         dirPrecipitation,          &
         dirTemperature,            &
         dirReferenceET,            &
         dirOut,                    &
         dirRestartOut,             &  
         global_parameters,         &
         global_parameters_name,    &
         warmPer,                   &
         evalPer,                   &
         SimPer,                    &
         LCyearId,                  &
         LCfilename

    implicit none
    !
    ! local
    !
    character(256)                         :: fName
    integer(i4)                            :: i, j, n
    integer(i4)                            :: err

    fName=  trim(adjustl(dirConfigOut))//trim(adjustl(file_config))
    call message()
    call message('  Log-file written to ', trim(fName))
    open(uconfig, file=fName, status='unknown', action='write', iostat=err)
    if (err .ne. 0) then
       call message('  Problems while creating File' )
       call message('  Error-Code', num2str(err) )
       stop
    end if
    write(uconfig, 200) 
    write(uconfig, 100) 'mHM-UFZ v-5.0'
    write(uconfig, 100) 'L. Samaniego & R. Kumar, UFZ'
    write(uconfig, 200) 
    write(uconfig, 100)
    write(uconfig, 201) '         M A I N  mHM  C O N F I G U R A T I O N  I N F O R M A T I O N         '
    write(uconfig, 100)
    write(uconfig, 103) 'Number of basins            ', nBasins
    write(uconfig, 103) 'Total No. of nodes          ', L11_nCells
    write(uconfig, 103) 'Total No. of reaches        ', L11_nCells-1
    write(uconfig, 103) 'No. of cells L0             ', L0_nCells
    write(uconfig, 103) 'No. of cells L1             ', L1_nCells
    if ( processMatrix(8,1) .ne. 0 ) then
       write(uconfig, 103) 'No. of cells L11            ', L11_nCells
       write(uconfig, 103) 'Total No. of gauges         ', nGaugesTotal
    end if
    write(uconfig, 103)    'Time Step [h]               ', int(timeStep/3600._dp)
    write(uconfig, 125)    'Hydrology Resolution [m]      ', resolutionHydrology
    if ( processMatrix(8,1) .ne. 0 ) then
       write(uconfig, 125) 'Routing Resolution [m]        ', resolutionRouting
    end if
    write(uconfig, 126)    'Flag READ  restart states     ', restart_flag_states_read
    write(uconfig, 126)    'Flag WRITE restart states     ', restart_flag_states_write
    write(uconfig, 126)    'Flag READ  restart config.    ', restart_flag_config_read
    write(uconfig, 126)    'Flag WRITE restart config.    ', restart_flag_config_write
    !
    !******************
    ! Model Run period 
    !******************
    write(uconfig, 115) '                      Model Run Periods                      '
    write(uconfig, 116) &
         'From                To', &
         '   Day Month  Year   Day Month  Year'
    write(uconfig,117)  &
         'Warming Period (1)            ',&
         warmPer%dStart, warmPer%mStart, warmPer%yStart  ,& 
         warmPer%dEnd  , warmPer%mEnd  , warmPer%yEnd    ,&
         'Evaluation Period (2)         ',&
         evalPer%dStart   ,evalPer%mStart   , evalPer%yStart      ,& 
         evalPer%dEnd     ,evalPer%mEnd     , evalPer%yEnd        ,&
         'Simulation Period (1)+(2)     ',&
         SimPer%dStart  , SimPer%mStart  , SimPer%yStart  ,&
         SimPer%dEnd    , SimPer%mEnd    , SimPer%yEnd

    !*********************************
    ! Model Land Cover Observations 
    !*********************************
    write(uconfig,118) '            Land Cover Observations              '
    write(uconfig,119) '      Year', '    Land cover scene', 'Land Cover File'
    do i=1,SimPer%yEnd-SimPer%yStart+1
       write(uconfig,120) i+SimPer%yStart-1, LCyearId(i+SimPer%yStart-1), trim(LCfilename(LCyearId(i+SimPer%yStart-1)))
    end do
    !*********************************
    ! Initial Parameter Ranges
    !*********************************
    write(uconfig,121) '  Initial Transfer Function Parameter Ranges (gammas)  '
    !
    ! Transfer functions
    write(uconfig,122)      &
         '         i', '            min', '            max', '        current', &
         '                               name'
    do i=1, size(global_parameters,1)
       write(uconfig,123) &
            i, global_parameters(i,1), global_parameters(i,2), global_parameters(i,3), &
            trim(adjustl(global_parameters_name(i)))
    end do
    ! basin runoff data
    if ( processMatrix(8,1) .ne. 0 ) then
       write(uconfig, 202) '                Basin Runoff Data                '
       write(uconfig, 107) ' Gauge No.', '  Basin Id', '     Qmax[m3/s]', '     Qmin[m3/s]'         
       do i=1, nGaugesTotal
          write(uconfig,108) i, gauge%basinId(i), maxval(gauge%Q(:,i), gauge%Q(:,i) > nodata_dp), &
               minval(gauge%Q(:,i), gauge%Q(:,i) > nodata_dp)
       end do
    end if
    ! basin config
    write(uconfig,218) 'Basin-wise Configuration'
    do n=1,nBasins
       if ( processMatrix(8,1) .ne. 0 ) then
          write(uconfig,103) 'Basin No.                   ', n, &
               'No. of gauges               ', basin%nGauges(n)
       end if

       write(uconfig, 222)   'Directory list'

       write(uconfig, 224) 'Directory to morphological input         ',  dirMorpho(n)
       write(uconfig, 224) 'Directory to land cover input            ',  dirLCover(n)
       if ( processMatrix(8,1) .ne. 0 ) then
          write(uconfig, 224) 'Directory to gauging station input       ', dirGauges(n)
       end if
       write(uconfig, 224) 'Directory to precipitation input         ',  dirPrecipitation(n)
       write(uconfig, 224) 'Directory to temperature input           ',  dirTemperature(n)
       write(uconfig, 224) 'Directory to reference ET input          ',  dirReferenceET(n)
       write(uconfig, 224) 'Directory to write output by default     ',  dirOut(n)
       write(uconfig, 224) 'Directory to write output when restarted ',  dirRestartOut(n)

       if ( processMatrix(8,1) .ne. 0 ) then
          write(uconfig, 102) 'River Network       '
          write(uconfig, 100) 'Label 0 = intermediate draining cell '
          write(uconfig, 100) 'Label 1 = headwater cell             '
          write(uconfig, 100) 'Label 2 = sink cell                  '

          write(uconfig, 104)  '      Link', &
               '      From', &
               '        To', &
               '   Routing', &
               '     Label', &
               '    Length', &
               '      mean', &
               '        Id', &
               '      Node', &
               '      Node', &
               '  Sequence', &
               '          ', &
               '          ', &
               '     Slope'
          !
          write(uconfig, 105)  '', &
               '',           &
               '',           &
               '',           &
               '',           &
               '      [km]', &
               '       [-]'
          !
          do j=basin%L11_iStart(n), basin%L11_iEnd(n)-1
             i=L11_netPerm(j)
             write(uconfig,106) i, L11_fromN(i), L11_toN(i), L11_rOrder(i), L11_label(i), &
                  L11_length(i)/1000.0_dp, L11_slope(i)
          end do
          !        
          !
          ! draining node at L11
          write(uconfig, 109)  'Node Index', '   Node Id'
          do i=basin%L11_iStart(n), basin%L11_iEnd(n)
             write(uconfig, 110) i, L11_Id(i)
          end do
          !
          write(uconfig, 111)  '      Cell', '    L11-Id', '  efecArea'
          write(uconfig, 112)  '       No.', '       [-]', '     [km2]'

          do i=1,L1_nCells
             write(uconfig,113) i,  L1_L11_Id(i), L1_areaCell(i)
          end do
          write(uconfig,114)  ' Total[km2]', sum(L1_areaCell)
       end if
       !
    end do

    write(uconfig,*)
    close(uconfig)  

    !! Formats
100 format (a80)
102 format (/ 30('-') / a20 / 30('-') )
103 format (a20, 10x, i10)
104 format (/ 75('-') / 5a10, 5x, 2a10 / 5a10, 5x, 2a10)
105 format (5a10, 5x, 2a10 / 75('-'))
106 format (5i10, 5x, 2f10.3 )
107 format (2a10, 2a15)
108 format (2i10, 2f15.3)
    !
109 format (/ 20('-') / 2a10  / 20('-') )
110 format (             i10,  i10      )
    !
111 format (/ 30('-') / 3a10                   )
112 format (            3a10         / 30('-') )
113 format (            2i10,   1f10.3         )
114 format (30('-') / a15, 5x,  1f10.3 /       )
    !
115 format (/61('-')/ a61 /61('-'))
116 format (39x,a22 / 25x, a36)
117 format ( 3(a25,6(i6) /) )
    !
118 format (/50('-')/ a35  /50('-'))
119 format (a10,      a20, a20/)
120 format (i10, 10x, i10, a20)
    !
121 format (/55('-')/ a55 /55('-'))
122 format (a10, 3a15,   a35)
123 format (i10, 3f15.3, a35)
    !

125 format (a30,1x,f10.0)
126 format (a30,9x,L1)
    !
200 format (80('-'))
201 format (a80)
202 format (/50('-')/ a50 /50('-'))
    !
218 format (/ 80('-')/ 26x, a24,26x,  /80('-'))
222 format (/80('-')/ 26x,a21 /80('-'))
224 format (a40, 5x, a40)
  end Subroutine write_configfile


  ! ------------------------------------------------------------------

  !     NAME
  !         write_optifile

  !     PURPOSE
  !>        \brief Write briefly final optimization results.

  !>        \details Write overall best objective function and the best optimized parameter set to a file_opti.

  !     CALLING SEQUENCE

  !     INTENT(IN)
  !>        \param[in]  "real(dp)                   :: best_OF"         best objective function value as returned 
  !>                                                                    by the optimization routine
  !>        \param[in]  "real(dp), dimension(:)     :: best_paramSet"   best associated global parameter set

  !     INTENT(INOUT)
  !         None

  !     INTENT(OUT)
  !         None

  !     INTENT(IN), OPTIONAL
  !         None

  !     INTENT(INOUT), OPTIONAL
  !         None

  !     INTENT(OUT), OPTIONAL
  !         None

  !     RETURN
  !         None

  !     RESTRICTIONS
  !>        Called only when optimize is .TRUE.

  !     EXAMPLE
  !         None

  !     LITERATURE
  !         None

  !     HISTORY
  !>        \author David Schaefer
  !>        \date July 2013

  !         Modified, Rohini Kumar, Aug 2013  - change in structure of the code including call statements
  !                   Juliane Mai,  Oct 2013  - clear parameter names added
  !                                           - double precision written

  subroutine write_optifile(best_OF, best_paramSet, param_names)

    use mo_file,                only: file_opti, uopti
    use mo_global_variables,    only: dirConfigOut
    use mo_message,             only: message
    use mo_string_utils,        only: num2str

    implicit none

    real(dp),                       intent(in)     :: best_OF     
    real(dp),         dimension(:), intent(in)     :: best_paramSet
    character(len=*), dimension(:), intent(in)     :: param_names

    ! local variables
    character(256)                         :: fName, formHeader, formParams
    integer(i4)                            :: ii, err, n_params

    ! open file
    fName = trim(adjustl(dirConfigOut)) // trim(adjustl(file_opti))
    open(uopti, file=fName, status='unknown', action='write', iostat=err)
    if( err .ne. 0 ) then
       call message ('  IOError while openening ',trim(fName))
       call message ('  Error-Code ', num2str(err))
       stop
    end if

    ! number of parameters
    n_params = size(best_paramSet)

    ! header 
    write(formHeader, *) '( a40, ' , n_params,'(a40) )' 
    write(uopti, formHeader) 'OF', (trim(adjustl(param_names(ii))), ii=1,n_params)

    ! output
    write(formParams, *) '( es40.15, ', n_params,'(es40.15) )' 
    write(uopti, formParams) best_OF, (best_paramSet(ii), ii=1,n_params)

    ! close file
    close(uopti)

    ! screen output
    call message()
    call message(' Optimized parameters written to ', trim(fName) )

  end subroutine write_optifile

  ! ------------------------------------------------------------------

  !     NAME
  !         write_optinamelist

  !     PURPOSE
  !>        \brief Write final, optimized parameter set in a namelist format.

  !>        \details  Write final, optimized parameter set in a namelist format. 
  !>                  Only parameters of processes which were switched on are written to the namelist. 
  !>                  All others are discarded.

  !     CALLING SEQUENCE
  !         None

  !     INTENT(IN)
  !>        \param[in]  "integer(i4)      :: processMatrix(:,:)"   information on wihich process is switched on and off
  !>        \param[in]  "real(dp)         :: parameters(:,:)"      information about parameter (min, max, opti)
  !>        \param[in]  "logical          :: maskpara(:)"          infomation which parameter where optimized
  !>        \param[in]  "character(len=*) :: parameters_name(:)"   clear names of parameters

  !     INTENT(INOUT)
  !         None

  !     INTENT(OUT)
  !         None

  !     INTENT(IN), OPTIONAL
  !         None

  !     INTENT(INOUT), OPTIONAL
  !         None

  !     INTENT(OUT), OPTIONAL
  !         None

  !     RETURN
  !         None

  !     RESTRICTIONS
  !>        Called only when optimize is .TRUE.

  !     EXAMPLE
  !         None

  !     LITERATURE
  !         None

  !     HISTORY
  !>        \author Juliane Mai
  !>        \date Dec 2013

  !         Modified, 

  subroutine write_optinamelist(processMatrix, parameters, maskpara, parameters_name)

    use mo_file,                only: file_opti_nml, uopti_nml
    use mo_global_variables,    only: dirConfigOut, nProcesses
    use mo_message,             only: message
    use mo_string_utils,        only: num2str

    implicit none

    integer(i4),      dimension(nProcesses, 3),      intent(in) :: processMatrix     ! information about which process 
    !                                                                                ! case was used
    real(dp),         dimension(:,:),                intent(in) :: parameters        ! (min, max, opti)
    logical,          dimension(size(parameters,1)), intent(in) :: maskpara          ! .true. if parameter was calibrated
    character(len=*), dimension(size(parameters,1)), intent(in) :: parameters_name   ! clear names of parameters

    ! local variables
    character(256)                           :: fName
    character(3)                             :: flag
    character(len=25), dimension(nProcesses) :: Process_descr
    integer(i4)                              :: err
    integer(i4)                              :: iProc, iPar, iPar_start

    Process_descr(1) = 'interception'
    Process_descr(2) = 'snow'
    Process_descr(3) = 'soilmoisture'
    Process_descr(4) = 'directSealedAreaRunoff'
    Process_descr(5) = 'meteoCorrectionFactor'
    Process_descr(6) = 'interflow'
    Process_descr(7) = 'percolation'
    Process_descr(8) = 'routing'
    Process_descr(9) = 'geology'

    ! open file
    fName = trim(adjustl(dirConfigOut)) // trim(adjustl(file_opti_nml))
    open(uopti_nml, file=fName, status='unknown', action='write', iostat=err)
    if( err .ne. 0 ) then
       call message ('  IOError while openening ',trim(fName))
       call message ('  Error-Code ', num2str(err))
       stop
    end if

    write(uopti_nml,*) '!global_parameters'
    write(uopti_nml,*) '!PARAMETER                       lower_bound  upper_bound          value   FLAG  SCALING'

    iPar_start = 1
    do iProc=1,nProcesses

       write(uopti_nml,*) '! ',trim(adjustl(process_descr(iProc)))

       select case (iProc)
       case(1)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&interception1'
          end if
       case(2)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&snow1'
          end if
       case(3)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&soilmoisture1'
          end if
       case(4)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&directRunoff1'
          end if
       case(5)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&actualET1'
          end if
       case(6)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&interflow1'
          end if
       case(7)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&percolation1'
          end if
       case(8)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&routing1'
          end if
       case(9)
          if (processMatrix(iProc,1) .eq. 1) then
             write(uopti_nml,*) '&geoparameter'
          end if
       end select

       do iPar=iPar_Start, processMatrix(iProc, 3)
          if (maskpara(iPar)) then
             flag=' 1 '
          else
             flag=' 0 '
          end if
          write(uopti_nml,*) trim(adjustl(parameters_name(iPar))), ' = ', &
               parameters(iPar,1), ' , ', &
               parameters(iPar,2), ' , ', &
               parameters(iPar,3), ' , ', &
               flag, ', 1 '
       end do

       iPar_Start = processMatrix(iProc, 3) + 1

       write(uopti_nml,*) '/'
       write(uopti_nml,*) ' '

    end do ! loop over processes

    ! close file
    close(uopti_nml)

    ! screen output
    call message()
    call message(' Optimized parameters written in namelist format to ', trim(fName) )

  end subroutine write_optinamelist

  ! ------------------------------------------------------------------

  !     NAME
  !         write_daily_obs_sim_discharge

  !     PURPOSE
  !>        \brief Write a file for the daily observed and simulated discharge timeseries 
  !>                during the evaluation period for each gauging station

  !>        \details Write a file for the daily observed and simulated discharge timeseries 
  !>                during the evaluation period for each gauging station

  !     CALLING SEQUENCE

  !     INTENT(IN)
  !>        \param[in]  "real(dp), dimension(:,:)    :: Qobs"    daily time series of observed discharge 
  !>                                                             dims = (nModeling_days , nGauges_total)
  !>        \param[in]  "real(dp), dimension(:,:)    :: Qsim"    daily time series of modeled discharge
  !>                                                             dims = (nModeling_days , nGauges_total)

  !     INTENT(INOUT)
  !         None

  !     INTENT(OUT)
  !         None

  !     INTENT(IN), OPTIONAL
  !         None

  !     INTENT(INOUT), OPTIONAL
  !         None

  !     INTENT(OUT), OPTIONAL
  !         None

  !     RETURN
  !         None 

  !     RESTRICTIONS
  !         None

  !     EXAMPLE
  !         None

  !     LITERATURE
  !         None

  !     HISTORY
  !>        \author Rohini Kumar
  !>        \date August 2013

  subroutine write_daily_obs_sim_discharge(Qobs, Qsim)

    use mo_julian,              only: dec2date
    use mo_file,                only: file_daily_discharge, udaily_discharge
    use mo_global_variables,    only: nBasins, basin, dirOut, gauge, evalPer
    use mo_message,             only: message
    use mo_string_utils,        only: num2str

    implicit none

    ! input arguments
    real(dp), dimension(:,:), intent(in)     :: Qobs      ! observed time series  [nModeling_days X nGauges_total]
    real(dp), dimension(:,:), intent(in)     :: Qsim      ! simulated time series [nModeling_days X nGauges_total]

    ! local vars
    character(256)                           :: fName, formHeader, formData, dummy
    integer(i4)                              :: bb, gg, tt, err
    integer(i4)                              :: igauge_start, igauge_end
    integer(i4)                              :: day, month, year
    real(dp)                                 :: newTime
    

    ! initalize igauge_start
    igauge_start = 1

    ! basin loop
    do bb = 1, nBasins
       if( basin%nGauges(bb) .lt. 1 ) cycle

       ! estimate igauge_end
       igauge_end = igauge_start + basin%nGauges(bb) - 1

       ! check the existance of file
       fName = trim(adjustl(dirOut(bb))) // trim(adjustl(file_daily_discharge))
       open(udaily_discharge, file=trim(fName), status='unknown', action='write', iostat=err)
       if( err .ne. 0 ) then
          call message ('  IOError while openening ',trim(fName))
          call message ('  Error-Code ', num2str(err))
          stop
       end if

       ! header
       write(formHeader, *) '( 4a8, ' , basin%nGauges(bb),'(2X, a5, i10.10, 2X, a5, i10.10) )' 
       write(udaily_discharge, formHeader) 'No', 'Day', 'Mon', 'Year', &
            ( 'Qobs_', gauge%gaugeId(gg), &
            'Qsim_', gauge%gaugeId(gg), gg=igauge_start, igauge_end )

       ! form data
       write(formData, *) '( 4I8, ' , basin%nGauges(bb),'(2X,   f15.7 , 2X,  f15.7  ) )' 

       ! write data
       newTime  = real(evalPer%julStart,dp) - 0.5_dp

       do tt = 1, (evalPer%julEnd - evalPer%julStart + 1)          
          call dec2date(newTime, yy=year, mm=month, dd=day)
          write(udaily_discharge, formData) tt, day, month, year, ( Qobs(tt,gg), Qsim(tt,gg) , gg=igauge_start, igauge_end )
          newTime = newTime + 1.0_dp
       end do

       ! close file
       close(udaily_discharge)

       ! screen output
       call message()
       write(dummy,'(I3)') bb
       call message('  OUTPUT: saved daily discharge file for basin ', trim(adjustl(dummy)))
       call message('    to ',trim(fname))

       ! update igauge_start
       igauge_start = igauge_end + 1
       !
    end do

    !
  end subroutine write_daily_obs_sim_discharge


END MODULE mo_write_ascii



