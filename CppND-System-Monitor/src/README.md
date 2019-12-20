## Notes about my implementation
* `process.CpuUtilization();` in function `vector<Process>& System::Processes()` is very important. It updates the `cpu_utilization_` of each process and set the attributes of those "invalid" processes to 0, which is useful for sorting.

* The implementation of the `vector<Process>& System::Processes();` function does not need to construct every process every time it is called which can save running time. But, I think this has a drawback that the vector `processes_` may store a lot of unused processes which waste memory and take more time for sorting. 
  * potential improvement could be put an extra private attribute under each process object indicating the existence of the process. Such that we could clear useless processes periodically.