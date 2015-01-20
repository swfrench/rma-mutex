# Mutex locks based on MPI RMA

A mutual exclusion lock implemention based on MPI RMA, following the algorithm
of Latham et al., 2007 [1].

## Notes

All MPI errors (e.g. an MPI routine returning a value other than `MPI_SUCCESS`)
are fatal. This may change in the future if there us a clear need for external
error handling.

## References

[1] Latham, R., Ross, R., and Thakur, R. (2007). Implementing MPI-IO atomic
mode and shared file pointers using MPI one-sided communication. International
Journal of High Performance Computing Applications, 21(2), 132-143.
 
