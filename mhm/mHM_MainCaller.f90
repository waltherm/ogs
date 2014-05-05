

! this gets the call from ogs to start the actual mHM program
subroutine call_main_mHM() bind(C, NAME="call_main_mHM")
    !use global
    use mhm_base
    implicit none

    print*, "|F| call_main_mHM"

    ! first, allocate otherwise not used vars
    !call allocate_vars
    ! set bool to true, ie request vars from c++ when reading input
    !get_var_from_cpp = .TRUE.
    !send_var_to_cpp = .TRUE.

    call mhm_driver

end subroutine call_main_mHM



