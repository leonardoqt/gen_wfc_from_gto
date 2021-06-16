# DESCRIPTION
This code converts gto basis functions to PSWFC that can be attached to the corresponding blocks in the UPF psp file.
NOTE: need to use spherical harmonics, i.e., PURECART = 1

# USAGE
* Prepare gto basis in the Q-chem format as basis.gto
* Change Fortran double to scientific notation
* Copy the PP\_MESH/PP\_R block (only numbers, no header) of each element to the file "element".mesh
* Run the code
* Append "element"_wfc.dat to the PP_PSWFC block in the UPF file
* Append "element"_header.dat to the PP_HEADER block in the UPF file
* change "Number of Wavefuncitons" in the UPF file
