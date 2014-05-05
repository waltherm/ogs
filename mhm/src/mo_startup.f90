!> \file mo_startup.f90

!> \brief Startup procedures for mHM.

!> \details This module initializes all variables required to run mHM. This
!> module needs to be run only one time at the beginning of a simulation if
!> re-starting files do not exist.

!> \author Luis Samaniego, Rohini Kumar
!> \date Dec 2012

MODULE mo_startup

  ! This module provides the startup routines for mHM.

  ! Written Luis Samaniego, Rohini Kumar, Dec 2012

  USE mo_kind,          ONLY: i4, dp
  USE mo_init_states,   ONLY: get_basin_info
  use mo_mhm_constants, only: nodata_i4, nodata_dp
  IMPLICIT NONE

  PRIVATE

  PUBLIC :: initialise                      ! initialization sequence

CONTAINS

  ! ------------------------------------------------------------------

  !     NAME
  !         initialise

  !     PURPOSE
  !>        \brief Initialize main mHM variables

  !>        \details Initialize main mHM variables for a given basin. \n
  !>              Calls the following procedures in this order:\n
  !>                 - Constant initialization. \n
  !>                 - Generate soil database. \n
  !>                 - Checking inconsistencies input fields. \n
  !>                 - Variable initialization at level-0. \n
  !>                 - Variable initialization at level-1. \n
  !>                 - Variable initialization at level-11. \n
  !>                 - Space allocation of remaining variable/parameters. \n
  !>        Global variables will be used at this stage. \n

  !     INTENT(IN)
  !>        \param[in] "integer(i4)       ::  iBasin"               basin id

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
  !>        \author  Luis Samaniego, Rohini Kumar
  !>        \date    Dec 2012
  !         Modified Luis Samaniego, Mar 2008 - fully distributed multilayer
  !                  Rohini Kumar,   Oct 2010 - matrix to vector version
  !                                           - openmp parallelization
  !                                           - routing level 11
  !                  Luis Samaniego, Jul 2012 - removal of IMSL dependencies
  !                  Luis Samaniego, Dec 2012 - modular version
  !                  Rohini Kumar,   May 2013 - code cleaned and error checks
  !                  Rohini Kumar,   Nov 2013 - updated documentation 

  subroutine initialise(iBasin)

    use mo_kind,             only: i4
    use mo_global_variables, only: processMatrix, soilDB
    use mo_global_variables, only: soilDB
    use mo_soil_database,    only: generate_soil_database
    use mo_init_states,      only: variables_alloc
    use mo_global_variables, only: restart_flag_config_read, dirRestartIn
    USE mo_restart,          ONLY: read_restart_L11_config, read_restart_config

    use mo_net_startup,      only: L11_variable_init, L11_flow_direction, L11_set_network_topology,  &
                                   L11_routing_order, L11_link_location, L11_set_drain_outlet_gauges,&
                                   L11_stream_features, routing_dummy_alloc
    implicit none

    integer(i4), intent(in) :: iBasin

    ! To be initialized only one time
    if (iBasin == 1) then
       ! constants initialization
       call constants_init()
       ! soilDB common for all basins
       call generate_soil_database(soilDB)
    end if

    ! L0 and L1 initialization
    call L0_check_input(iBasin)
    if ( .not. restart_flag_config_read ) then
       call L0_variable_init(iBasin, soilDB%is_present)
       call L1_variable_init(iBasin)
    else
       call read_restart_config( iBasin, soilDB%is_present, dirRestartIn(iBasin ) )
    end if

    ! L11: network initialization
    if ( processMatrix(8, 1) .ne. 0 ) then
       ! check if variables should be read from restart
       if ( .not. restart_flag_config_read ) then
          call L11_variable_init(iBasin)
          call L11_flow_direction(iBasin)
          call L11_set_network_topology(iBasin)
          call L11_routing_order(iBasin)
          call L11_link_location(iBasin)
          call L11_set_drain_outlet_gauges(iBasin)
          ! stream characteristics
          call L11_stream_features(iBasin)
       else
          call read_restart_L11_config(iBasin, dirRestartIn(iBasin) )
       end if
    else
       ! allocate dummy space for L0 variables related to routing process
       call routing_dummy_alloc(iBasin)
  end if

    ! State variables, fluxes and parameter fields 
    ! have to be allocated in any case
    call variables_alloc(iBasin)

  end subroutine initialise
  ! ------------------------------------------------------------------

  !      NAME
  !          constants_init

  !>        \brief Initialize mHM constants 

  !>        \details transformation of time units & initialize constants

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
  !         None

  !     LITERATURE
  !         None

  !     HISTORY
  !>        \author  Luis Samaniego
  !>        \date    Dec 2012
  !         Modified Rohini Kumar,                 Jan 2013
  !                  Juliane Mai & Matthias Cuntz, Nov 2013 - check timeStep

  subroutine constants_init( )

    use mo_global_variables, only: NTSTEPDAY, c2TSTu, timeStep
    use mo_message,          only: message
    use mo_string_utils,     only: num2str 

    implicit none

    ! transformation of time units & constants
    if (mod(24,timeStep) > 0) then
       call message('mo_startup: timeStep must be a divisor of 24: ', num2str(timeStep))
       stop
    endif
    NTSTEPDAY  = 24_i4/timeStep            ! # of time steps per day
    c2TSTu     = real(timeStep,dp)/24.0_dp ! from per timeStep to per day
    
  end subroutine constants_init

  ! ------------------------------------------------------------------

  !      NAME
  !          L0_check_input

  !>        \brief Check for errors in L0 input data 

  !>        \details Check for possible errors in input data (morphological and land cover) at level-0
                     
  !     INTENT(IN)
  !>        \param[in] "integer(i4)       ::  iBasin"               basin id

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
  !         \author  Rohini Kumar
  !         \date    Jan 2013
  !         Modified
  !                  Rohini Kumar, Aug  2013 - added iFlag_LAI_data_format to handle LAI options,
  !                                             and changed within the code made accordingly
  !                  Rohini  Kumar, Sep 2013 - read input data for routing processes according
  !                & Stephan Thober,           to process_matrix flag
  ! ------------------------------------------------------------------

  subroutine L0_check_input(iBasin)

    use mo_global_variables, only: basin                    , &
                                   L0_elev, L0_slope, L0_asp, &
                                   L0_fDir, L0_fAcc         , &
                                   L0_soilId, L0_geoUnit    , &
                                   L0_LCover_LAI            , &
                                   nLCover_scene            , &  
                                   L0_LCover, iFlag_LAI_data_format, & 
                                   processMatrix                                   
    use mo_constants,    only: eps_dp             
    use mo_message,      only: message, message_text
    use mo_string_utils, only: num2str

    implicit none

    integer(i4), intent(in) ::   iBasin

    ! local variables
    integer(i4)                                :: k, n

    ! START CHECKING VARIABLES
    do k = basin%L0_iStart(iBasin), basin%L0_iEnd(iBasin)

       ! elevation [m]
       if ( abs(L0_elev(k) - nodata_dp) .lt. eps_dp  ) then
          message_text = trim(num2str(k,'(I5)'))//','//trim(num2str(iBasin,'(I5)'))
          call message(' Error: elevation has missing value within the valid masked area at cell in basin ', &
               trim(message_text) )
          stop
       end if

       ! slope [%]
       if ( abs( L0_slope(k) - nodata_dp) .lt. eps_dp  ) then
          message_text = trim(num2str(k,'(I5)'))//','// trim(num2str(iBasin,'(I5)'))
          call message(' Error: slope has missing value within the valid masked area at cell in basin ', &
               trim(message_text) )
          stop
       end if

       if( processMatrix(8, 1) .NE. 0 ) then
         ! flow direction [-]
         if ( L0_fDir(k) .eq. nodata_i4  ) then
            message_text = trim(num2str(k,'(I5)'))//','// trim(num2str(iBasin,'(I5)'))
            call message(' Error: flow direction has missing value within the valid masked area at cell in basin ', &
                 trim(message_text) )
            stop
         end if
         ! flow accumulation [-]
         if ( L0_fAcc(k) .eq. nodata_i4 ) then
            message_text = trim(num2str(k,'(I5)'))//','// trim(num2str(iBasin,'(I5)'))
            call message(' Error: flow accumulation has missing values within the valid masked area at cell in basin ', &
                 trim(message_text) )
            stop
         end if

       end if     

       ! aspect [degree]
       if ( abs( L0_asp(k) - nodata_dp ) .lt. eps_dp  ) then
          message_text = trim(num2str(k,'(I5)'))//','// trim(num2str(iBasin,'(I5)'))
          call message(' Error: aspect has missing values within the valid masked area at cell in basin ', &
               trim(message_text) )
          stop
       end if

       ! soil-Id [-]
       if ( L0_soilId(k) .eq. nodata_i4 ) then
          message_text = trim(num2str(k,'(I5)'))//','// trim(num2str(iBasin,'(I5)'))
          call message(' Error: soil id has missing values within the valid masked area at cell in basin ', &
               trim(message_text) )
          stop
       end if

       ! geological-Id [-]
       if ( L0_geoUnit(k) .eq. nodata_i4 ) then
          message_text = trim(num2str(k,'(I5)'))//','// trim(num2str(iBasin,'(I5)'))
          call message(' Error: geological formation id has missing values within the valid masked area at cell in basin ', &
               trim(message_text) )
          stop
       end if

       ! landcover scenes
       do  n = 1, nLCover_scene
          if ( L0_LCover(k,n) .eq. nodata_i4  ) then
             message_text = trim(num2str(k,'(I5)'))//','// trim(num2str(iBasin,'(I5)'))//','// trim(num2str(n,'(I5)'))
             call message(' Error: land cover id has missing values within the valid masked area at cell in basin and scene ', &
                  trim(message_text) )
             stop
          end if
       end do


       ! land cover scenes related to LAI
       if(iFlag_LAI_data_format .EQ. 0) then
         if ( L0_LCover_LAI(k) .eq. nodata_i4 ) then
            message_text = trim(num2str(k,'(I5)'))//','// trim(num2str(iBasin,'(I5)'))
            call message(' Error: land cover ID for LAI has missing values within the valid masked area at cell in basin ', &
                 trim(message_text) )
            stop
         end if
       end if
       
    end do

  end subroutine L0_check_input
  
  ! ------------------------------------------------------------------

  !      NAME
  !          L0_variable_init

  !>        \brief   level 0 variable initialization

  !>        \details following tasks are performed for L0 data sets
  !>                 -  cell id & numbering 
  !>                 -  storage of cell cordinates (row and coloum id)
  !>                 -  empirical dist. of terrain slope
  !>                 -  flag to determine the presence of a particular soil id 
  !>                    in this configuration of the model run
  !>                 If a variable is added or removed here, then it also has to 
  !>                 be added or removed in the subroutine config_variables_set in
  !>                 module mo_restart and in the subroutine set_config in module
  !>                 mo_set_netcdf_restart

  !     INTENT(IN)
  !>        \param[in] "integer(i4)               :: iBasin"  basin id

  !     INTENT(INOUT)
  !>        \param[in,out] "integer(i4), dimension(:) :: soilId_isPresent"   
  !>        flag to indicate wether a given soil-id is present or not, DIMENSION [nSoilTypes]                                   

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
  !         \author  Rohini Kumar
  !         \date    Jan 2013
  subroutine L0_variable_init(iBasin, soilId_isPresent)

    use mo_global_variables, only: level0, L0_areaCell,    &
                                   L0_nCells, L0_cellCoor, &
                                   L0_Id, L0_slope,        & 
                                   L0_slope_emp,           &
                                   L0_soilId, nSoilTypes
    use mo_append,    only: append                    
    use mo_sort,      only: sort_index                

    implicit none

    integer(i4), intent(in)                               :: iBasin
    integer(i4), dimension(:), allocatable, intent(inout) :: soilId_isPresent

    ! local variables
    integer(i4)                               :: nCells
    integer(i4), dimension(:,:), allocatable  :: cellCoor
    integer(i4), dimension(:), allocatable    :: Id
    real(dp), dimension(:), allocatable       :: areaCell

    integer(i4)                               :: nrows, ncols
    integer(i4)                               :: iStart, iEnd
    logical, dimension(:,:), allocatable      :: mask

    real(dp), dimension(:), allocatable       :: slope_val, slope_emp
    integer(i4), dimension(:), allocatable    :: slope_sorted_index

    integer(i4)                               :: i, j, k

    !--------------------------------------------------------
    ! STEPS::
    ! 1) Estimate each variable locally for a given basin
    ! 2) Pad each variable to its corresponding global one
    !--------------------------------------------------------

    ! level-0 information
    call get_basin_info( iBasin, 0, nrows, ncols, nCells=nCells, iStart=iStart, iEnd=iEnd, mask=mask ) 

    allocate( cellCoor(nCells,2) )
    allocate(       Id(nCells  ) )
    allocate( areaCell(nCells  ) )

    cellCoor(:,:) =  nodata_i4
    Id(:)         =  nodata_i4
    areaCell(:)   =  nodata_dp

    !------------------------------------------------
    ! start looping for cell cordinates and ids
    !------------------------------------------------
    k = 0
    do j = 1, ncols
       do i = 1, nrows
          if ( .NOT. mask(i,j) ) cycle
          k = k + 1
          Id(k)         = k
          cellCoor(k,1) = i
          cellCoor(k,2) = j
          ! estimate area of cell at level-0
          areaCell(k) = level0%cellsize * level0%cellsize
       end do
    end do

    !---------------------------------------------------
    ! Estimate empirical distribution of slope
    !---------------------------------------------------
    allocate( slope_val(nCells), slope_sorted_index(nCells), slope_emp(nCells) )
    slope_val(:)          = L0_slope(iStart:iEnd)
    slope_sorted_index(:) = sort_index( slope_val(:) )

    do i = nCells, 1, -1
      slope_val(i) = real(i, dp)
    end do

    do i = 1, nCells
      j            = slope_sorted_index(i)
      slope_emp(j) = slope_val(i) / real((nCells+1), dp)
    end do

    !--------------------------------------------------------
    ! Start padding up local variables to global variables
    !--------------------------------------------------------
    call append( L0_cellCoor, cellCoor )
    call append( L0_Id, Id             )
    call append( L0_areaCell, areaCell )
    call append( L0_slope_emp, slope_emp )
    
    L0_nCells = size(L0_Id,1)
 
    !------------------------------------------------------
    ! Assign whether a given soil type is present or not
    !------------------------------------------------------
    if ( iBasin .eq. 1 ) then
       allocate( soilId_isPresent(nSoilTypes) )
       soilId_isPresent(:) = 0
    end if

    do k = iStart, iEnd
       j = L0_soilId(k)
       soilId_isPresent(j) = 1
    end do

    ! free space
    deallocate(cellCoor, Id, areaCell, mask, slope_val, slope_emp, slope_sorted_index)

  end subroutine L0_variable_init

  ! ------------------------------------------------------------------

  !      NAME
  !         L1_variable_init

  !>        \brief Level-1 variable initialization

  !>        \details following tasks are performed for L1 datasets
  !>                 -  cell id & numbering
  !>                 -  mask creation
  !>                 -  storage of cell cordinates (row and coloum id)
  !>                 -  sorage of four corner L0 cordinates 
  !>                 If a variable is added or removed here, then it also has to 
  !>                 be added or removed in the subroutine config_variables_set in
  !>                 module mo_restart and in the subroutine set_config in module
  !>                 mo_set_netcdf_restart

  !     INTENT(IN)
  !>        \param[in] "integer(i4)       ::  iBasin"               basin id

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
  !         \author  Rohini Kumar
  !         \date    Jan 2013

  subroutine L1_variable_init(iBasin)

    use mo_global_variables, only: nBasins, basin, level0, level1, &
                                   L0_areacell, L1_areaCell,       &
                                   L1_nCells, L1_Id, L1_cellCoor,  &
                                   L1_upBound_L0, L1_downBound_L0, &
                                   L1_leftBound_L0,                &
                                   L1_rightBound_L0, L1_nTCells_L0,&
                                   resolutionHydrology
     use  mo_init_states,  only : calculate_grid_properties
     use mo_append,        only : append                      ! append vector

    implicit none

    integer(i4), intent(in)                   :: iBasin

    ! local variables
    integer(i4)                               :: nrows0, ncols0
    integer(i4)                               :: iStart0, iEnd0
    real(dp)                                  :: xllcorner0, yllcorner0
    real(dp)                                  :: cellsize0
    logical, dimension(:,:), allocatable      :: mask0
    real(dp), dimension(:,:), allocatable     :: areaCell0_2D

    integer(i4)                               :: nrows1, ncols1
    logical, dimension(:,:), allocatable      :: mask1

    integer(i4)                               :: nCells
    integer(i4), dimension(:,:), allocatable  :: cellCoor
    integer(i4), dimension(:), allocatable    :: Id
    integer(i4), dimension(:), allocatable    :: upBound
    integer(i4), dimension(:), allocatable    :: downBound
    integer(i4), dimension(:), allocatable    :: leftBound
    integer(i4), dimension(:), allocatable    :: rightBound
    real(dp), dimension(:), allocatable       :: areaCell
    integer(i4), dimension(:), allocatable    :: nTCells

    real(dp)                                  :: cellFactorHydro

    integer(i4)                               :: iup, idown
    integer(i4)                               :: jl, jr

    integer(i4)                               :: i, j, k, ic, jc

    !--------------------------------------------------------
    ! STEPS::
    ! 1) Estimate each variable locally for a given basin
    ! 2) Pad each variable to its corresponding global one
    !--------------------------------------------------------

    ! level-0 information
    call get_basin_info( iBasin, 0, nrows0, ncols0, iStart=iStart0, iEnd=iEnd0, mask=mask0, &
                         xllcorner=xllcorner0, yllcorner=yllcorner0, cellsize=cellsize0     ) 

    if(iBasin == 1) then
       allocate( level1%nrows     (nBasins) )
       allocate( level1%ncols     (nBasins) )
       allocate( level1%xllcorner (nBasins) )
       allocate( level1%yllcorner (nBasins) )
    end if

    ! grid properties
    call calculate_grid_properties( nrows0, ncols0, xllcorner0, yllcorner0, cellsize0, nodata_dp,         &
                                    resolutionHydrology , &
                                    level1%nrows(iBasin), level1%ncols(iBasin), level1%xllcorner(iBasin), &
                                    level1%yllcorner(iBasin), level1%cellsize, level1%nodata_value        )

    ! level-1 information
    call get_basin_info( iBasin, 1, nrows1, ncols1 ) 

    ! cellfactor = leve1-1 / level-0
    cellFactorHydro = level1%cellsize / level0%cellsize

    ! allocation and initalization of mask at level-1
    allocate( mask1(nrows1, ncols1) )
    mask1(:,:) = .FALSE.
 
    ! create mask at level-1
    do j=1,ncols0
       jc = ceiling( real(j,dp)/cellFactorHydro )
       do i=1,nrows0
          if ( .NOT. mask0(i,j) ) cycle
          ic = ceiling( real(i,dp)/cellFactorHydro )
          mask1(ic,jc) = .TRUE.
       end do
    end do
    
    
    ! level-0 cell area
    allocate( areaCell0_2D(nrows0,ncols0) )
    areaCell0_2D(:,:) = UNPACK( L0_areaCell(iStart0:iEnd0), mask0, nodata_dp )

    ! estimate ncells and initalize related variables
    nCells = count( mask1 )

    ! allocate and initalize cell1 related variables
    allocate( Id        (nCells   ) )
    allocate( cellCoor  (nCells, 2) )
    allocate( upBound   (nCells   ) )
    allocate( downBound (nCells   ) )
    allocate( leftBound (nCells   ) )
    allocate( rightBound(nCells   ) )
    allocate( areaCell  (nCells   ) )   
    allocate( nTCells   (nCells  ) )

    k   = 0
    do jc=1,ncols1
       do ic=1,nrows1
          if ( .NOT. mask1(ic,jc) ) cycle
          k = k + 1

          Id(k)         = k
          cellCoor(k,1) = ic
          cellCoor(k,2) = jc

          ! coord. of all corners -> of finer scale level-0
          iup   = (ic-1) * nint(cellFactorHydro,i4) + 1
          idown =    ic  * nint(cellFactorHydro,i4)
          jl    = (jc-1) * nint(cellFactorHydro,i4) + 1
          jr    =    jc  * nint(cellFactorHydro,i4)
  
          ! constrain the range of up, down, left, and right boundaries
          if(iup   < 1      ) iup =  1
          if(idown > nrows0 ) idown =  nrows0
          if(jl    < 1      ) jl =  1
          if(jr    > ncols0 ) jr =  ncols0

          upBound   (k) =  iup 
          downBound (k) =  idown 
          leftBound (k) =  jl 
          rightBound(k) =  jr 

          ! effective area [km2] & total no. of L0 cells within a given L1 cell
          areaCell(k) =   sum( areacell0_2D(iup:idown, jl:jr), mask0(iup:idown, jl:jr) )*1.0E-6
          nTCells(k)  = count(        mask0(iup:idown, jl:jr)                          )

       end do
    end do


    !--------------------------------------------------------
    ! Start padding up local variables to global variables
    !--------------------------------------------------------
    if( iBasin .eq. 1 ) then

       allocate(basin%L1_iStart(nBasins))
       allocate(basin%L1_iEnd  (nBasins))
       allocate(basin%L1_iStartMask(nBasins))
       allocate(basin%L1_iEndMask   (nBasins))    

       ! basin information
       basin%L1_iStart(iBasin) = 1
       basin%L1_iEnd  (iBasin) = basin%L1_iStart(iBasin) + nCells - 1

       basin%L1_iStartMask(iBasin) = 1
       basin%L1_iEndMask  (iBasin) = basin%L1_iStartMask(iBasin) + nrows1*ncols1 - 1

    else

       ! basin information
       basin%L1_iStart(iBasin) = basin%L1_iEnd(iBasin-1) + 1
       basin%L1_iEnd  (iBasin) = basin%L1_iStart(iBasin) + nCells - 1

       basin%L1_iStartMask(iBasin) = basin%L1_iEndMask(iBasin-1) + 1
       basin%L1_iEndMask  (iBasin) = basin%L1_iStartMask(iBasin) + nrows1*ncols1 - 1

    end if

    call append( basin%L1_Mask,  RESHAPE( mask1, (/nrows1*ncols1/)  )  )
    call append( L1_Id           , Id         )
    call append( L1_cellCoor     , cellCoor   )
    call append( L1_upBound_L0   , upBound    )
    call append( L1_downBound_L0 , downBound  )
    call append( L1_leftBound_L0 , leftBound  )
    call append( L1_rightBound_L0, rightBound )
    call append( L1_areaCell     , areaCell   )
    call append( L1_nTCells_L0   , nTCells    )

    L1_nCells = size( L1_Id, 1 )

    ! free space
    deallocate( mask0, areaCell0_2D, mask1, areaCell, &
                cellCoor, Id, upBound, downBound,     &
                leftBound, rightBound, nTCells        )

  end subroutine L1_variable_init

END MODULE mo_startup
