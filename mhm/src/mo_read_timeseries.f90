!> \file mo_read_timeseries.f90

!> \brief Routines to read files containing timeseries data.

!> \details This routine is reading time series input data for a particular time period. The files need to have a
!> specific header specified in the different routines.

!> \authors Matthias Zink, Juliane Mai
!> \date Jan 2013

MODULE mo_read_timeseries

  USE mo_kind, ONLY: i4, dp

  IMPLICIT NONE

  PUBLIC :: read_timeseries

CONTAINS

  ! -----------------------------------------------------------------
  !     NAME
  !         read_timeseries

  !     PURPOSE
  !>        \brief Reads time series in ASCII format.
                 
  !>        \details Reads time series in ASCII format.
  !>        Needs specific header lines:\n
  !>        \verbatim
  !>           <description> 
  !>           nodata  <nodata value> 
  !>           n  <number of measurements per day>  measurements per day [1, 1440] 
  !>           start  <YYYY_i4> <MM_i4> <DD_i4> <HH_i4> <MM_i4> (YYYY MM DD HH MM) 
  !>           end    <YYYY_i4> <MM_i4> <DD_i4> <HH_i4> <MM_i4> (YYYY MM DD HH MM) 
  !>        \endverbatim
  !>        Line 6 is the first line with data in the following format:\n
  !>        \verbatim
  !>           <YYYY_i4> <MM_i4> <DD_i4> <HH_i4> <MM_i4> <data_dp>
  !>        \endverbatim
  !>        The routine checks for missing data points and if data points are equal distanced.
  !>        The first data point at each day has to be at HH:MM = 00:00.\n

  !     CALLING SEQUENCE
  !         call read_timeseries(filename, fileunit, &
  !                              (/start_YYYY, start_MM, start_DD/), &
  !                              (/end_YYYY,   end_MM,   end_DD/),   &
  !                              data, mask=mask, nMeasPerDay=nMeasPerDay)

  !     INTENT(IN)
  !>        \param[in] "character(len=*)                              :: filename"     File name
  !>        \param[in] "integer(i4)                                   :: fileunit"     Unit to open file
  !>        \param[in] "integer(i4), dimension(3)                     :: periodStart"  Start day of reading (YYYY,MM,DD)
  !>        \param[in] "integer(i4), dimension(3)                     :: periodEnd"    End   day of reading (YYYY,MM,DD) 
  !>        \param[in] "logical                                       :: optimize"     optimization flag

  !     INTENT(INOUT)
  !         None

  !     INTENT(OUT)
  !>        \param[out] "real(dp), dimension(:), allocatable          :: data"         Data vector

  !     INTENT(IN), OPTIONAL
  !         None

  !     INTENT(INOUT), OPTIONAL
  !         None

  !     INTENT(OUT), OPTIONAL
  !>        \param[out] "logical, dimension(:), allocatable, optional :: mask"         Mask for nodata values in data
  !>        \param[out] "integer(i4), optional                        :: nMeasPerDay"  Number of data points per day

  !     RETURN
  !         None 

  !     RESTRICTIONS
  !>        \note Routine reads in only whole days with equidistant time steps between data points.\n
  !>        The first data point has to be at HH:MM = 00:00.


  !     EXAMPLE
  !         call read_timeseries('old_code/sub_00020/input/gauge/0000411.txt', 127, &
  !                              (/1901, 01, 03/), &
  !                              (/1901, 01, 08/), &
  !                              data, mask=maske, nMeasPerDay=nMeasPerDay) 

  !     LITERATURE
  !         None

  !     HISTORY
  !>        \authors Matthias Zink, Juliane Mai
  !>        \date Jan 2013
  !          Modified, 
  !                    Stephan Thober, Mar 2014 read data even if shorter than model period
  !                    MatthiasZink,   Mar 2014 enable read in of nodata periods, e.g. forecast mode

  subroutine read_timeseries(filename, fileunit, periodStart, periodEnd, optimize, data, mask, nMeasPerDay)

    use mo_julian,        only: julday
    use mo_message,       only: message

    implicit none

    character(len=*),                                 intent(in)  :: filename         ! name of input file
    integer(i4),                                      intent(in)  :: fileunit         ! file unit
    integer(i4), dimension(3),                        intent(in)  :: periodStart      ! format (/YYYY, MM, DD/)
    integer(i4), dimension(3),                        intent(in)  :: periodEnd        ! format (/YYYY, MM, DD/)
    logical,                                          intent(in)  :: optimize         ! optimization on or off (.TRUE. or .FALSE.)
    real(dp),    dimension(:), allocatable,           intent(out) :: data             ! time series output (periodStart:periodEnd)
    logical,     dimension(:), allocatable, optional, intent(out) :: mask             ! indicating valid data (false
    !                                                                                 ! at no data value points)
    integer(i4),                            optional, intent(out) :: nMeasPerDay      ! Number of data points read in
    !                                                                                 ! per day, e.g. hourly = 24

    ! local vaiables
    real(dp)                                                      :: nodata_file      ! no data value of data
    integer(i4)                                                   :: timestep_file    ! [h] time resolution of input data
    integer(i4),  dimension(3)                                    :: periodStart_file ! starting date of timeseries data within file
    integer(i4),  dimension(3)                                    :: periodEnd_file   ! ending date of timeseries data within file
    integer(i4),  dimension(5)                                    :: time_file        ! year, month, day, hour, minute

    integer(i4)                                                   :: i, j
    integer(i4)                                                   :: idx_st_period    ! index to put data from file to data
    integer(i4)                                                   :: idx_en_period    ! index to put data from file to data
    integer(i4)                                                   :: idx_st_file      ! index to put data from file to data 
    integer(i4)                                                   :: idx_en_file      ! index to put data from file to data
    integer(i4)                                                   :: startJul_file    ! start julian day of available data
    integer(i4)                                                   :: endJul_file      ! end   julian day of available data
    integer(i4)                                                   :: startJul_period  ! start julian day of needed data
    integer(i4)                                                   :: endJul_period    ! end   julian day of needed data
    real(dp),    dimension(:), allocatable                        :: data_file        ! time series output (fileStart:fileEnd)
    !                                                                                 ! points --> 0.25 [d-1]
    character(256)                                                :: dummy            ! dummy for char read in

    open(unit=fileunit, file=filename, action='read', status='old')
      ! read header
      read(fileunit,'(a256)') dummy
      read(fileunit,*)        dummy, nodata_file
      read(fileunit,*)        dummy, timestep_file
      read(fileunit,*)        dummy, (periodStart_file(i), i = 1, 3)
      read(fileunit,*)        dummy, (periodEnd_file(i),   i = 1, 3)
      dummy = dummy//''   ! only to avoid warning
      if ((timestep_file .lt. 1_i4) .or. (timestep_file .gt. 1440_i4)) then 
         call message('***ERROR: Number of measurements per day has to be between 1 (daily) and 1440 (every minute)! ',&
                       trim(filename))
         stop
      end if

      ! checking if period is covered by data in file
      startJul_period = julday(periodStart(3),      periodStart(2),      periodStart(1))
      endJul_period   = julday(periodEnd(3)  ,      periodEnd(2)  ,      periodEnd(1)  )
      startJul_file   = julday(periodStart_file(3), periodStart_file(2), periodStart_file(1) )
      endJul_file     = julday(periodEnd_file(3),   periodEnd_file(2),   periodEnd_file(1) )

      if (((startJul_period < startJul_file) .OR. (endJul_period > endJul_file )) .AND. optimize) then
         call message('***ERROR: Simulation period is not covered by observations! ', trim(filename))
         stop
      end if

      ! allocation of arrays 
      allocate( data     (      (endJul_period - startJul_period + 1_i4) * timestep_file))
      data      = nodata_file
      allocate( data_file(      (endJul_file   - startJul_file + 1_i4)   * timestep_file))
      data_file = nodata_file

      if (present(mask)) then
         allocate( mask(      (endJul_period - startJul_period + 1_i4) * timestep_file))
         mask = .true.
      end if

      ! read data from file to temporal array
      do i=1, (endJul_file - startJul_file + 1_i4) * timestep_file
         ! read date and data
         read(fileunit, *) (time_file(j),j=1,5), data_file(i)
      end do
      time_file(1) = time_file(2) + 1   ! only to avoid warning

      ! put data in final array
      ! period in file smaller than modelling period
      idx_en_period = (endJul_file     - startJul_period + 1) * timestep_file
      !
      idx_en_file = (endJul_period   - startJul_file + 1) * timestep_file
      !     
      if   ( startJul_period .gt. startJul_file ) then
         idx_st_period = 1
         idx_st_file   = (startJul_period - startJul_file      ) * timestep_file + 1
      else ! ( startJul_period .le. startJul_file )
         idx_st_period = (startJul_file   - startJul_period    ) * timestep_file + 1
         idx_st_file   = 1
      end if

      if   ( endJul_period .lt. endJul_file ) then
         idx_en_period = size(data, dim=1)
         idx_en_file   = (endJul_period   - startJul_file + 1  ) * timestep_file
      else ! ( endJul_period .ge. endJul_file )
         idx_en_period = (endJul_file     - startJul_period + 1) * timestep_file
         idx_en_file   = size(data, dim=1)
      end if

      data(idx_st_period:idx_en_period) = data_file(idx_st_file:idx_en_file)

      if (present(mask)) then
         where ( abs(data-nodata_file) .lt. tiny(1.0_dp) )
            mask = .false.
         end where
      end if

      if (present(nMeasPerDay)) then
         nMeasPerDay = timestep_file
      end if

    close(fileunit)

  end subroutine read_timeseries

END MODULE mo_read_timeseries
