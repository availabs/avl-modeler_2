jin@rush:~$ cd AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/examples
jin@rush:~/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/examples$ cd test_prototype_mtc_new/
jin@rush:~/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/examples/test_prototype_mtc_new$ conda activate ASIM_DEV_new
(ASIM_DEV_new) jin@rush:~/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/examples/test_prototype_mtc_new$ activitysim run -c configs -d data -o output
Configured logging using basicConfig
INFO:activitysim:Configured logging using basicConfig
[00:01.15] INFO: Read logging configuration from: configs/logging.yaml
[00:01.15] INFO: SETTING configs_dir: ['configs']
[00:01.15] INFO: SETTING settings_file_name: settings.yaml
[00:01.15] INFO: SETTING data_dir: ['data']
[00:01.15] INFO: SETTING output_dir: output
[00:01.15] INFO: SETTING households_sample_size: 100
[00:01.15] INFO: SETTING chunk_size: 0
[00:01.15] INFO: SETTING chunk_method: hybrid_uss
[00:01.15] INFO: SETTING chunk_training_mode: disabled
[00:01.15] INFO: SETTING multiprocess: None
[00:01.15] INFO: SETTING num_processes: None
[00:01.15] INFO: SETTING resume_after: None
[00:01.15] INFO: SETTING trace_hh_id: 982875
[00:01.15] INFO: ENV MKL_NUM_THREADS: None
[00:01.15] INFO: ENV OMP_NUM_THREADS: None
[00:01.15] INFO: ENV OPENBLAS_NUM_THREADS: None
[00:01.15] INFO: NUMPY blas_info libraries: ['cblas', 'blas', 'cblas', 'blas']
[00:01.15] INFO: NUMPY blas_opt_info libraries: ['cblas', 'blas', 'cblas', 'blas']
[00:01.15] INFO: NUMPY lapack_info libraries: ['lapack', 'blas', 'lapack', 'blas']
[00:01.15] INFO: NUMPY lapack_opt_info libraries: ['lapack', 'blas', 'lapack', 'blas', 'cblas', 'blas', 'cblas', 'blas']
[00:01.15] INFO: run single process simulation
[00:01.16] INFO: Time to execute open_pipeline : 0.011 seconds (0.0 minutes)
[00:01.17] INFO: preload_injectables
[00:01.17] INFO: Time to execute preload_injectables : 0.014 seconds (0.0 minutes)
[00:01.20] INFO: #run_model running step initialize_landuse
Running step 'initialize_landuse'
[00:01.21] INFO: Reading CSV file data/land_use.csv
[00:01.22] INFO: loaded land_use (25, 24)
[00:01.22] INFO: initialize_landuse.annotate_tables - annotating land_use SPEC annotate_landuse
[00:01.23] INFO: Network_LOS using skim_dict_factory: NumpyArraySkimFactory
[00:01.53] INFO: allocate_skim_buffer shared False taz shape (826, 25, 25) total size: 2_065_000 (2.1 MB)
[00:01.53] INFO: _read_skims_from_omx data/skims.omx
[00:02.02] INFO: _read_skims_from_omx loaded 826 skims from data/skims.omx
[00:02.02] INFO: writing skim cache taz (826, 25, 25) to output/cache/cached_taz.mmap
[00:02.03] INFO: load_skims_to_buffer taz shape (826, 25, 25)
[00:02.03] INFO: get_skim_data taz SkimData shape (826, 25, 25)
[00:02.03] INFO: SkimDict init taz
[00:02.03] INFO: SkimDict.build_3d_skim_block_offset_table registered 167 3d keys
Time to execute step 'initialize_landuse': 0.83 s
Total time to execute iteration 1 with iteration value None: 0.83 s
[00:02.11] INFO: #run_model running step initialize_households
Running step 'initialize_households'
[00:02.11] INFO: Reading CSV file data/households.csv
[00:02.13] INFO: full household list contains 5000 households
[00:02.13] INFO: sampling 100 of 5000 households
[00:02.13] INFO: loaded households (100, 7)
[00:02.13] INFO: tracing household id 982875 in 100 households
[00:02.14] INFO: Reading CSV file data/persons.csv
[00:02.16] INFO: loaded persons (167, 7)
[00:02.16] INFO: initialize_households.annotate_tables - annotating persons SPEC annotate_persons
[00:02.23] INFO: initialize_households.annotate_tables - annotating households SPEC annotate_households
[00:02.29] INFO: initialize_households.annotate_tables - annotating persons SPEC annotate_persons_after_hh
Time to execute step 'initialize_households': 0.22 s
Total time to execute iteration 1 with iteration value None: 0.22 s
[00:02.43] INFO: #run_model running step compute_accessibility
Running step 'compute_accessibility'
[00:02.43] INFO: Running compute_accessibility with 25 orig zones 25 dest zones
[00:02.43] INFO: compute_accessibility Running adaptive_chunked_choosers with 25 choosers
[00:02.43] INFO: Running chunk 1 of 1 with 25 of 25 choosers
[00:02.43] INFO: Running compute_accessibility with 25 orig zones 25 dest zones
[00:02.43] INFO: compute_accessibility: merge land_use_columns into od_df
[00:02.43] INFO: compute_accessibility: assign.assign_variables
[00:02.44] INFO: compute_accessibility.assign_variables _auPkTime = skim_od[('SOVTOLL_TIME', 'AM')] + skim_do[('SOVTOLL_TIME', 'PM')]
get item ('SOVTOLL_TIME', 'AM')
get item ('SOVTOLL_TIME', 'PM')
[00:02.44] INFO: compute_accessibility.assign_variables _decay = exp(_auPkTime * dispersion_parameter_automobile)
[00:02.44] INFO: compute_accessibility.assign_variables auPkRetail = df.RETEMPN * _decay
[00:02.44] INFO: compute_accessibility.assign_variables auPkTotal = df.TOTEMP * _decay
[00:02.44] INFO: compute_accessibility.assign_variables _auOpTime = skim_od[('SOVTOLL_TIME', 'MD')] + skim_do[('SOVTOLL_TIME', 'MD')]
get item ('SOVTOLL_TIME', 'MD')
get item ('SOVTOLL_TIME', 'MD')
[00:02.44] INFO: compute_accessibility.assign_variables _decay = exp(_auOpTime * dispersion_parameter_automobile)
[00:02.44] INFO: compute_accessibility.assign_variables auOpRetail = df.RETEMPN * _decay
[00:02.44] INFO: compute_accessibility.assign_variables auOpTotal = df.TOTEMP * _decay
[00:02.44] INFO: compute_accessibility.assign_variables _inVehicleTime = skim_od[('WLK_TRN_WLK_IVT', 'AM')]
get item ('WLK_TRN_WLK_IVT', 'AM')
[00:02.45] INFO: compute_accessibility.assign_variables _outOfVehicleTime = skim_od[('WLK_TRN_WLK_IWAIT', 'AM')] + skim_od[('WLK_TRN_WLK_XWAIT', 'AM')] + skim_od[('WLK_TRN_WLK_WACC', 'AM')] + skim_od[('WLK_TRN_WLK_WAUX', 'AM')] + skim_od[('WLK_TRN_WLK_WEGR', 'AM')]
get item ('WLK_TRN_WLK_IWAIT', 'AM')
get item ('WLK_TRN_WLK_XWAIT', 'AM')
get item ('WLK_TRN_WLK_WACC', 'AM')
get item ('WLK_TRN_WLK_WAUX', 'AM')
get item ('WLK_TRN_WLK_WEGR', 'AM')
[00:02.45] INFO: compute_accessibility.assign_variables _trPkTime_od = (_inVehicleTime + out_of_vehicle_time_weight * _outOfVehicleTime) / TRANSIT_SCALE_FACTOR
[00:02.45] INFO: compute_accessibility.assign_variables _inVehicleTime = skim_do[('WLK_TRN_WLK_IVT', 'PM')]
get item ('WLK_TRN_WLK_IVT', 'PM')
[00:02.45] INFO: compute_accessibility.assign_variables _outOfVehicleTime = skim_do[('WLK_TRN_WLK_IWAIT', 'PM')] + skim_do[('WLK_TRN_WLK_XWAIT', 'PM')] + skim_do[('WLK_TRN_WLK_WACC', 'PM')] + skim_do[('WLK_TRN_WLK_WAUX', 'PM')] + skim_do[('WLK_TRN_WLK_WEGR', 'PM')]
get item ('WLK_TRN_WLK_IWAIT', 'PM')
get item ('WLK_TRN_WLK_XWAIT', 'PM')
get item ('WLK_TRN_WLK_WACC', 'PM')
get item ('WLK_TRN_WLK_WAUX', 'PM')
get item ('WLK_TRN_WLK_WEGR', 'PM')
[00:02.45] INFO: compute_accessibility.assign_variables _trPkTime_do = (_inVehicleTime + out_of_vehicle_time_weight * _outOfVehicleTime) / TRANSIT_SCALE_FACTOR
[00:02.45] INFO: compute_accessibility.assign_variables _trPkTime = (_trPkTime_od + _trPkTime_do).clip(0)
[00:02.45] INFO: compute_accessibility.assign_variables _rt_available = (_trPkTime_od > 0) & (_trPkTime_do > 0)
[00:02.45] INFO: compute_accessibility.assign_variables _decay = _rt_available * exp(_trPkTime * dispersion_parameter_transit)
[00:02.45] INFO: compute_accessibility.assign_variables trPkRetail = df.RETEMPN * _decay
[00:02.45] INFO: compute_accessibility.assign_variables trPkTotal = df.TOTEMP * _decay
[00:02.45] INFO: compute_accessibility.assign_variables _inVehicleTime = skim_od[('WLK_TRN_WLK_IVT', 'MD')]
get item ('WLK_TRN_WLK_IVT', 'MD')
[00:02.46] INFO: compute_accessibility.assign_variables _outOfVehicleTime = skim_od[('WLK_TRN_WLK_IWAIT', 'MD')] + skim_od[('WLK_TRN_WLK_XWAIT', 'MD')] + skim_od[('WLK_TRN_WLK_WACC', 'MD')] + skim_od[('WLK_TRN_WLK_WAUX', 'MD')] + skim_od[('WLK_TRN_WLK_WEGR', 'MD')]
get item ('WLK_TRN_WLK_IWAIT', 'MD')
get item ('WLK_TRN_WLK_XWAIT', 'MD')
get item ('WLK_TRN_WLK_WACC', 'MD')
get item ('WLK_TRN_WLK_WAUX', 'MD')
get item ('WLK_TRN_WLK_WEGR', 'MD')
[00:02.46] INFO: compute_accessibility.assign_variables _trOpTime_od = (_inVehicleTime + out_of_vehicle_time_weight * _outOfVehicleTime) / TRANSIT_SCALE_FACTOR
[00:02.46] INFO: compute_accessibility.assign_variables _inVehicleTime = skim_do[('WLK_TRN_WLK_IVT', 'MD')]
get item ('WLK_TRN_WLK_IVT', 'MD')
[00:02.46] INFO: compute_accessibility.assign_variables _outOfVehicleTime = skim_do[('WLK_TRN_WLK_IWAIT', 'MD')] + skim_do[('WLK_TRN_WLK_XWAIT', 'MD')] + skim_do[('WLK_TRN_WLK_WACC', 'MD')] + skim_do[('WLK_TRN_WLK_WAUX', 'MD')] + skim_do[('WLK_TRN_WLK_WEGR', 'MD')]
get item ('WLK_TRN_WLK_IWAIT', 'MD')
get item ('WLK_TRN_WLK_XWAIT', 'MD')
get item ('WLK_TRN_WLK_WACC', 'MD')
get item ('WLK_TRN_WLK_WAUX', 'MD')
get item ('WLK_TRN_WLK_WEGR', 'MD')
[00:02.46] INFO: compute_accessibility.assign_variables _trOpTime_do = (_inVehicleTime + out_of_vehicle_time_weight * _outOfVehicleTime) / TRANSIT_SCALE_FACTOR
[00:02.46] INFO: compute_accessibility.assign_variables _trOpTime = (_trOpTime_od + _trOpTime_do).clip(0)
[00:02.46] INFO: compute_accessibility.assign_variables _rt_available = (_trOpTime_od > 0) & (_trOpTime_do > 0)
[00:02.46] INFO: compute_accessibility.assign_variables _decay = _rt_available * exp(_trOpTime * dispersion_parameter_transit)
[00:02.46] INFO: compute_accessibility.assign_variables trOpRetail = df.RETEMPN * _decay
[00:02.46] INFO: compute_accessibility.assign_variables trOpTotal = df.TOTEMP * _decay
[00:02.46] INFO: compute_accessibility.assign_variables _nmDist = skim_od['DISTWALK'] + skim_do['DISTWALK']
get item DISTWALK
get item DISTWALK
[00:02.47] INFO: compute_accessibility.assign_variables _rt_available = _nmDist <= maximum_walk_distance
[00:02.47] INFO: compute_accessibility.assign_variables _decay = _rt_available * exp(_nmDist * dispersion_parameter_walk)
[00:02.47] INFO: compute_accessibility.assign_variables nmRetail = df.RETEMPN * _decay
[00:02.47] INFO: compute_accessibility.assign_variables nmTotal = df.TOTEMP * _decay
[00:02.47] INFO: compute_accessibility: have results
[00:02.47] INFO: compute_accessibility computed accessibilities (25, 10)
Time to execute step 'compute_accessibility': 0.04 s
Total time to execute iteration 1 with iteration value None: 0.04 s
[00:02.54] INFO: #run_model running step school_location
Running step 'school_location'
[00:02.55] INFO: Running school_location.i1.sample.university with 17 persons
[00:02.56] INFO: school_location.i1.sample.university.interaction_sample Running adaptive_chunked_choosers with 17 choosers
[00:02.56] INFO: Running chunk 1 of 1 with 17 of 17 choosers
[00:02.57] INFO: Running eval_interaction_utilities on 102 rows
get item DIST
[00:02.61] INFO: Running school_location.i1.logsums.university with 44 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:02.79] INFO: school_location.i1.logsums.university.compute_logsums Running adaptive_chunked_choosers with 44 choosers
[00:02.79] INFO: Running chunk 1 of 1 with 44 of 44 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:03.29] INFO: Running school_location.i1.simulate.university with 17 persons
[00:03.29] INFO: school_location.i1.simulate.university.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 17 choosers and 44 alternatives
[00:03.29] INFO: Running chunk 1 of 1 with 17 of 17 choosers
[00:03.30] INFO: Running eval_interaction_utilities on 44 rows
get item DIST
[00:03.33] INFO: Running school_location.i1.sample.highschool with 5 persons
[00:03.34] INFO: school_location.i1.sample.highschool.interaction_sample Running adaptive_chunked_choosers with 5 choosers
[00:03.34] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:03.34] INFO: Running eval_interaction_utilities on 10 rows
get item DIST
[00:03.37] INFO: Running school_location.i1.logsums.highschool with 5 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:03.55] INFO: school_location.i1.logsums.highschool.compute_logsums Running adaptive_chunked_choosers with 5 choosers
[00:03.55] INFO: Running chunk 1 of 1 with 5 of 5 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:03.95] INFO: Running school_location.i1.simulate.highschool with 5 persons
[00:03.96] INFO: school_location.i1.simulate.highschool.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 5 choosers and 5 alternatives
[00:03.96] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:03.96] INFO: Running eval_interaction_utilities on 5 rows
get item DIST
[00:03.98] INFO: Running school_location.i1.sample.gradeschool with 17 persons
[00:03.98] INFO: school_location.i1.sample.gradeschool.interaction_sample Running adaptive_chunked_choosers with 17 choosers
[00:03.98] INFO: Running chunk 1 of 1 with 17 of 17 choosers
[00:03.98] INFO: Running eval_interaction_utilities on 425 rows
get item DIST
[00:04.06] INFO: Running school_location.i1.logsums.gradeschool with 168 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:04.24] INFO: school_location.i1.logsums.gradeschool.compute_logsums Running adaptive_chunked_choosers with 168 choosers
[00:04.24] INFO: Running chunk 1 of 1 with 168 of 168 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:04.61] INFO: Running school_location.i1.simulate.gradeschool with 17 persons
[00:04.63] INFO: school_location.i1.simulate.gradeschool.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 17 choosers and 168 alternatives
[00:04.63] INFO: Running chunk 1 of 1 with 17 of 17 choosers
[00:04.63] INFO: Running eval_interaction_utilities on 168 rows
get item DIST
[00:04.67] INFO: write_trace_files iteration 1
[00:04.70] INFO: school_location_logsum top 10 value counts:
10.893995    1
12.125021    1
20.447053    1
10.539866    1
20.920595    1
20.361189    1
19.950674    1
10.770754    1
10.165962    1
20.936072    1
Name: logsum, dtype: int64
Time to execute step 'school_location': 2.16 s
Total time to execute iteration 1 with iteration value None: 2.16 s
[00:04.77] INFO: #run_model running step workplace_location
Running step 'workplace_location'
[00:04.79] INFO: Running workplace_location.i1.sample.work_low with 37 persons
[00:04.79] INFO: workplace_location.i1.sample.work_low.interaction_sample Running adaptive_chunked_choosers with 37 choosers
[00:04.79] INFO: Running chunk 1 of 1 with 37 of 37 choosers
[00:04.80] INFO: Running eval_interaction_utilities on 925 rows
get item DIST
[00:04.84] INFO: Running workplace_location.i1.logsums.work_low with 504 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:05.00] INFO: workplace_location.i1.logsums.work_low.compute_logsums Running adaptive_chunked_choosers with 504 choosers
[00:05.00] INFO: Running chunk 1 of 1 with 504 of 504 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:05.39] INFO: Running workplace_location.i1.simulate.work_low with 37 persons
[00:05.40] INFO: workplace_location.i1.simulate.work_low.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 37 choosers and 504 alternatives
[00:05.40] INFO: Running chunk 1 of 1 with 37 of 37 choosers
[00:05.41] INFO: Running eval_interaction_utilities on 504 rows
get item DIST
[00:05.44] INFO: Running workplace_location.i1.sample.work_med with 26 persons
[00:05.45] INFO: workplace_location.i1.sample.work_med.interaction_sample Running adaptive_chunked_choosers with 26 choosers
[00:05.45] INFO: Running chunk 1 of 1 with 26 of 26 choosers
[00:05.46] INFO: Running eval_interaction_utilities on 650 rows
get item DIST
[00:05.51] INFO: Running workplace_location.i1.logsums.work_med with 367 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:05.68] INFO: workplace_location.i1.logsums.work_med.compute_logsums Running adaptive_chunked_choosers with 367 choosers
[00:05.68] INFO: Running chunk 1 of 1 with 367 of 367 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:06.21] INFO: Running workplace_location.i1.simulate.work_med with 26 persons
[00:06.22] INFO: workplace_location.i1.simulate.work_med.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 26 choosers and 367 alternatives
[00:06.22] INFO: Running chunk 1 of 1 with 26 of 26 choosers
[00:06.22] INFO: Running eval_interaction_utilities on 367 rows
get item DIST
[00:06.25] INFO: Running workplace_location.i1.sample.work_high with 16 persons
[00:06.26] INFO: workplace_location.i1.sample.work_high.interaction_sample Running adaptive_chunked_choosers with 16 choosers
[00:06.26] INFO: Running chunk 1 of 1 with 16 of 16 choosers
[00:06.26] INFO: Running eval_interaction_utilities on 400 rows
get item DIST
[00:06.30] INFO: Running workplace_location.i1.logsums.work_high with 226 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:06.46] INFO: workplace_location.i1.logsums.work_high.compute_logsums Running adaptive_chunked_choosers with 226 choosers
[00:06.46] INFO: Running chunk 1 of 1 with 226 of 226 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:06.83] INFO: Running workplace_location.i1.simulate.work_high with 16 persons
[00:06.84] INFO: workplace_location.i1.simulate.work_high.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 16 choosers and 226 alternatives
[00:06.84] INFO: Running chunk 1 of 1 with 16 of 16 choosers
[00:06.84] INFO: Running eval_interaction_utilities on 226 rows
get item DIST
[00:06.87] INFO: Running workplace_location.i1.sample.work_veryhigh with 18 persons
[00:06.88] INFO: workplace_location.i1.sample.work_veryhigh.interaction_sample Running adaptive_chunked_choosers with 18 choosers
[00:06.88] INFO: Running chunk 1 of 1 with 18 of 18 choosers
[00:06.89] INFO: Running eval_interaction_utilities on 450 rows
get item DIST
[00:06.93] INFO: Running workplace_location.i1.logsums.work_veryhigh with 253 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:07.09] INFO: workplace_location.i1.logsums.work_veryhigh.compute_logsums Running adaptive_chunked_choosers with 253 choosers
[00:07.09] INFO: Running chunk 1 of 1 with 253 of 253 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:07.46] INFO: Running workplace_location.i1.simulate.work_veryhigh with 18 persons
[00:07.47] INFO: workplace_location.i1.simulate.work_veryhigh.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 18 choosers and 253 alternatives
[00:07.47] INFO: Running chunk 1 of 1 with 18 of 18 choosers
[00:07.48] INFO: Running eval_interaction_utilities on 253 rows
get item DIST
[00:07.52] INFO: write_trace_files iteration 1
[00:07.58] INFO: workplace_location_logsum top 10 value counts:
15.437107    1
14.391639    1
15.663789    1
15.656738    1
15.699269    1
15.725096    1
15.549150    1
15.551084    1
13.812699    1
13.914080    1
Name: logsum, dtype: int64
Time to execute step 'workplace_location': 2.81 s
Total time to execute iteration 1 with iteration value None: 2.81 s
[00:07.67] INFO: #run_model running step auto_ownership_simulate
Running step 'auto_ownership_simulate'
[00:07.68] INFO: Running auto_ownership_simulate with 100 households
[00:07.68] INFO: auto_ownership_simulate.simple_simulate Running adaptive_chunked_choosers with 100 choosers
[00:07.68] INFO: Running chunk 1 of 1 with 100 of 100 choosers
before eval utilities                                                                                      cars0   cars1   cars2   cars3   cars4
Expression                                         Label                                                                  
num_drivers==2                                     util_drivers_2                      0.0  0.0000  3.0773  3.1962  2.6616
num_drivers==3                                     util_drivers_3                      0.0  0.0000  3.5401  5.5131  5.2080
num_drivers>3                                      util_drivers_4_up                   0.0  2.0107  6.3662  8.5148  9.5807
num_children_16_to_17                              util_persons_16_17                  0.0  0.0000 -0.8810 -1.7313 -1.7313
num_college_age                                    util_persons_18_24                  0.0 -0.4087 -1.0095 -1.0107 -1.0107
num_young_adults                                   util_persons_25_34                  0.0  0.0000 -0.4849 -0.8596 -0.8596
num_young_children>0                               util_presence_children_0_4          0.0  0.3669  0.7627  0.7627  0.7627
(num_children_5_to_15+num_children_16_to_17)>0     util_presence_children_5_17         0.0  0.0158  0.2936  0.4769  0.4769
@df.num_workers.clip(upper=3)                      util_num_workers_clip_3             0.0  0.0000  0.2936  0.6389  0.8797
@df.income_in_thousands.clip(0, 30)                util_hh_income_0_30k                0.0  0.0383  0.0540  0.0559  0.0619
@(df.income_in_thousands-30).clip(0, 45)           util_hh_income_30_75k               0.0  0.0000  0.0083  0.0110  0.0147
@(df.income_in_thousands-75).clip(0, 50)           util_hh_income_75k_up               0.0  0.0000  0.0083  0.0110  0.0147
@(df.num_workers==0)*df.density_index.clip(0, 10)  util_density_0_10_no_workers        0.0  0.0000 -0.2028 -0.3654 -0.3654
@(df.num_workers==0)*(df.density_index-10).clip(0) util_density_10_up_no_workers       0.0 -0.0152 -0.1106 -0.1766 -0.1766
@(df.num_workers>0)*df.density_index.clip(0, 10)   util_density_0_10_workers           0.0  0.0000 -0.2028 -0.3654 -0.3654
@(df.num_workers>0)*(df.density_index-10).clip(0)  util_density_10_up_workers          0.0 -0.0152 -0.1106 -0.1766 -0.1766
1                                                  util_asc                            0.0  1.1865 -1.0846 -3.2502 -5.3130
@df.county_id == ID_SAN_FRANCISCO                  util_asc_san_francisco              0.0  0.4259  0.4683  0.1458  0.1458
@df.county_id == ID_SOLANO                         util_asc_solano                     0.0 -0.5660 -0.4429 -0.2372 -0.2372
@df.county_id == ID_NAPA                           util_asc_napa                       0.0 -0.5660 -0.4429 -0.2372 -0.2372
@df.county_id == ID_SONOMA                         util_asc_sonoma                     0.0 -0.5660 -0.4429 -0.2372 -0.2372
@df.county_id == ID_MARIN                          util_asc_marin                      0.0 -0.2434  0.0000  0.0000  0.0000
(num_workers==0)*(0.66*auPkRetail+0.34*auOpRetail) util_retail_auto_no_workers         0.0  0.0626  0.0626  0.0626  0.0626
(num_workers>0)*(0.66*auPkRetail+0.34*auOpRetail)  util_retail_auto_workers            0.0  0.1646  0.1646  0.1646  0.1646
(num_workers==0)*(0.66*trPkRetail+0.34*trOpRetail) util_retail_transit_no_workers      0.0 -0.3053 -0.3053 -0.3053 -0.3053
(num_workers>0)*(0.66*trPkRetail+0.34*trOpRetail)  util_retail_transit_workers         0.0 -0.5117 -0.5117 -0.5117 -0.5117
(num_workers==0)*nmRetail                          util_retail_non_motor_no_workers    0.0 -0.0300 -0.0300 -0.0300 -0.0300
(num_workers>0)*nmRetail                           util_retail_non_motor_workers       0.0 -0.0300 -0.0300 -0.0300 -0.0300
@np.where(df.num_workers > 0, df.hh_work_auto_s... util_auto_time_saving_per_worker    0.0  0.4707  0.6142  0.5705  0.7693               home_zone_id  income  hhsize  HHT  auto_ownership  num_workers  ...  TOPOLOGY  TERMINAL  household_density  employment_density  density_index  is_cbd
household_id                                                                  ...                                                                                  
982875                  16   30900       2    5               0            2  ...         2   4.75017          71.898080          273.023745      56.911108   False
1810015                 16   99700       9    2               1            4  ...         2   4.75017          71.898080          273.023745      56.911108   False
386761                  16   21000       3    1               0            2  ...         2   4.75017          71.898080          273.023745      56.911108   False
112064                  16   18000       1    6               0            1  ...         2   4.75017          71.898080          273.023745      56.911108   False
2223759                 16  144100       2    1               2            2  ...         2   4.75017          71.898080          273.023745      56.911108   False
...                    ...     ...     ...  ...             ...          ...  ...       ...       ...                ...                 ...            ...     ...
823501                  12   30000       1    4               0            1  ...         3   5.46202          46.647059          893.176471      44.331786   False
2048204                  5  388000       1    4               1            1  ...         1   5.52555          38.187500          978.875000      36.753679   False
824434                  22   56250       1    4               1            1  ...         3   4.93813          21.727273          360.872727      20.493414   False
701664                   2   14800       2    1               0            0  ...         1   5.84871           5.195214         1631.374751       5.178722   False
2848131                  3       0       1    0               0            0  ...         1   5.53231          80.470405          736.891913      72.547987   False

[100 rows x 63 columns] {'ID_SAN_FRANCISCO': 1, 'ID_SAN_MATEO': 2, 'ID_SANTA_CLARA': 3, 'ID_ALAMEDA': 4, 'ID_CONTRA_COSTA': 5, 'ID_SOLANO': 6, 'ID_NAPA': 7, 'ID_SONOMA': 8, 'ID_MARIN': 9}
before logit.utils_to_probs 
               cars0     cars1      cars2      cars3      cars4
household_id                                                  
982875          0.0 -0.150164  -5.680333 -12.779770 -14.672508
1810015         0.0  0.991041  -4.290927  -9.206948  -9.008174
386761          0.0 -0.506447  -4.946135 -11.129859 -13.091492
112064          0.0 -0.593459  -8.715628 -15.556608 -17.223990
2223759         0.0 -0.134612  -4.879011 -11.725821 -13.263821
...             ...       ...        ...        ...        ...
823501          0.0 -0.492792  -7.250925 -13.231430 -14.860657
2048204         0.0 -0.110209  -5.328378 -10.560911 -11.799139
824434          0.0  0.332336  -3.923585  -8.262976  -9.781009
701664          0.0  0.233479   0.264195  -2.918946  -5.427546
2848131         0.0 -1.230268 -11.454046 -19.696312 -21.759113

[100 rows x 5 columns]
[00:07.81] INFO: auto_ownership top 10 value counts:
0    60
1    40
Name: auto_ownership, dtype: int64
Time to execute step 'auto_ownership_simulate': 0.15 s
Total time to execute iteration 1 with iteration value None: 0.15 s
[00:07.88] INFO: #run_model running step free_parking
Running step 'free_parking'
[00:07.89] INFO: Running free_parking with 97 persons
[00:07.92] INFO: free_parking.simple_simulate Running adaptive_chunked_choosers with 97 choosers
[00:07.92] INFO: Running chunk 1 of 1 with 97 of 97 choosers
before eval utilities                                                                              free  pay
Expression                                  Label                                     
@df.workplace_county_id == ID_SAN_FRANCISCO util_asc_san_francisco        -2.6403  0.0
@df.workplace_county_id == ID_SANTA_CLARA   util_asc_santa_clara           0.2118  0.0
@df.workplace_county_id == ID_ALAMEDA       util_asc_alameda              -0.1092  0.0
@df.income>=100000                          util_income_very_high          0.2300  0.0
@(df.income>=60000) & (df.income<100000)    util_income_high               0.2300  0.0
@df.hhsize>3                                util_hh_size_4_up              0.2530  0.0
@df.auto_ownership>df.num_workers           util_more_autos_than_workers   0.2310  0.0
@df.auto_ownership<df.num_workers           util_fewer_autos_than_workers -1.4790  0.0            household_id  age  PNUM  sex  pemploy  pstudent  ptype  ...  TOPOLOGY  TERMINAL  household_density  employment_density  density_index  is_cbd  workplace_county_id
person_id                                                          ...                                                                                                       
107628           107628   42     1    1        2         3      2  ...         1   5.00004         149.333333          252.600000      93.850390   False                    1
110675           110675   30     1    1        2         3      2  ...         2   4.75017          71.898080          273.023745      56.911108   False                    1
112064           112064   48     1    2        2         3      2  ...         2   4.75017          71.898080          273.023745      56.911108   False                    1
112535           112535   25     1    2        1         3      1  ...         1   4.58156          68.092368          151.674705      46.994710    True                    1
264107           226869   28     1    1        2         3      2  ...         2   5.22542          97.634722          550.205552      82.920387   False                    1
...                 ...  ...   ...  ...      ...       ...    ...  ...       ...       ...                ...                 ...            ...     ...                  ...
7513075         2821740   22     1    2        2         2      3  ...         2   4.64648         196.395950          178.779465      93.587057   False                    1
7513432         2822097   39     1    2        2         3      2  ...         2   4.64648         196.395950          178.779465      93.587057   False                    1
7513546         2822211   21     1    1        2         3      2  ...         2   4.64648         196.395950          178.779465      93.587057   False                    1
7513554         2822219   18     1    2        1         3      1  ...         2   4.64648         196.395950          178.779465      93.587057   False                    1
7513565         2822230   19     1    2        2         2      3  ...         2   4.64648         196.395950          178.779465      93.587057   False                    1

[97 rows x 105 columns] {'ID_SAN_FRANCISCO': 1, 'ID_SAN_MATEO': 2, 'ID_SANTA_CLARA': 3, 'ID_ALAMEDA': 4, 'ID_CONTRA_COSTA': 5, 'ID_SOLANO': 6, 'ID_NAPA': 7, 'ID_SONOMA': 8, 'ID_MARIN': 9}
before logit.utils_to_probs 
              free  pay
person_id             
107628    -4.1193  0.0
110675    -4.1193  0.0
112064    -2.6403  0.0
112535    -4.1193  0.0
264107    -4.1193  0.0
...           ...  ...
7513075   -4.1193  0.0
7513432   -2.6403  0.0
7513546   -4.1193  0.0
7513554   -4.1193  0.0
7513565   -4.1193  0.0

[97 rows x 2 columns]
[00:08.03] INFO: free_parking top 10 value counts:
False    163
True       4
Name: free_parking_at_work, dtype: int64
Time to execute step 'free_parking': 0.14 s
Total time to execute iteration 1 with iteration value None: 0.14 s
[00:08.10] INFO: #run_model running step cdap_simulate
Running step 'cdap_simulate'
[00:08.13] INFO: Pre-building cdap specs
[00:08.23] INFO: Time to execute build_cdap_spec hh_size 2 : 0.101 seconds (0.0 minutes)
[00:08.53] INFO: Time to execute build_cdap_spec hh_size 3 : 0.297 seconds (0.0 minutes)
[00:09.26] INFO: Time to execute build_cdap_spec hh_size 4 : 0.734 seconds (0.0 minutes)
[00:10.83] INFO: Time to execute build_cdap_spec hh_size 5 : 1.552 seconds (0.0 minutes)
[00:10.89] INFO: Running cdap_simulate with 167 persons
[00:10.89] INFO: cdap.cdap Running chunk 1 of 1 with 100 of 100 choosers
[00:12.71] INFO: cdap_activity top 10 value counts:
M    89
N    48
H    30
Name: cdap_activity, dtype: int64
[00:12.73] INFO: cdap crosstabs:
cdap_activity   H   M   N  All
ptype                         
1               4  42   6   52
2               8  19   7   34
3               1  11   5   17
4               4   0  17   21
5               9   0  12   21
6               4   1   0    5
7               0  10   1   11
8               0   6   0    6
All            30  89  48  167
Time to execute step 'cdap_simulate': 4.63 s
Total time to execute iteration 1 with iteration value None: 4.63 s
[00:12.81] INFO: #run_model running step mandatory_tour_frequency
Running step 'mandatory_tour_frequency'
[00:12.83] INFO: Running mandatory_tour_frequency with 89 persons
[00:12.84] INFO: mandatory_tour_frequency.simple_simulate Running adaptive_chunked_choosers with 89 choosers
[00:12.84] INFO: Running chunk 1 of 1 with 89 of 89 choosers
before eval utilities                                                                                 work1     work2  school1   school2  work_and_school
Expression                Label                                                                                                    
ptype == 1                util_ft_worker                                        0.000   -3.3781      0.0    0.0000           0.0000
ptype == 2                util_pt_worker                                        0.000   -3.0476      0.0    0.0000           0.0000
ptype == 3                util_univ                                             2.166   -1.3965      0.0   -3.7429           0.1073
ptype == 6                util_driving_age_child                                0.000    0.0000      0.0   -3.1360          -4.4362
ptype == 7                util_pre_driving_age_child                            0.000    0.0000      0.0   -3.9703           0.0000
...                                                                               ...       ...      ...       ...              ...
ptype == 6 # (2)          util_availability_driving_age_child                -999.000 -999.0000      0.0    0.0000           0.0000
ptype == 7 # (2)          util_availability_pre_driving_age_student             0.000 -999.0000      0.0    0.0000        -999.0000
ptype == 8                util_availability_pre_driving_age_not_in_school    -999.000 -999.0000      0.0 -999.0000        -999.0000
~(workplace_zone_id > -1) util_availability_work_tours_no_usual_work_loca... -999.000 -999.0000      0.0    0.0000        -999.0000
~(school_zone_id > -1)    util_availability_school_tours_no_usual_school_...    0.000    0.0000   -999.0 -999.0000        -999.0000

[98 rows x 5 columns]            household_id  age  PNUM  sex  pemploy  pstudent  ptype  ...   COLLPTE  TOPOLOGY  TERMINAL  household_density  employment_density  density_index  is_cbd
person_id                                                          ...                                                                                            
264107           226869   28     1    1        2         3      2  ...  20.60887         2   5.22542          97.634722          550.205552      82.920387   False
264248           226939   23     2    1        1         3      1  ...   0.00000         3   4.73802         117.769796          246.205869      79.663609   False
323689           256660   22     1    2        2         3      2  ...   0.00000         3   4.73802         117.769796          246.205869      79.663609   False
324051           256841   64     1    1        1         3      1  ...   0.00000         2   4.75017          71.898080          273.023745      56.911108   False
324052           256841   58     2    2        1         3      1  ...   0.00000         2   4.75017          71.898080          273.023745      56.911108   False
...                 ...  ...   ...  ...      ...       ...    ...  ...       ...       ...       ...                ...                 ...            ...     ...
7513546         2822211   21     1    1        2         3      2  ...   0.00000         2   4.64648         196.395950          178.779465      93.587057   False
7513554         2822219   18     1    2        1         3      1  ...   0.00000         2   4.64648         196.395950          178.779465      93.587057   False
7513565         2822230   19     1    2        2         2      3  ...   0.00000         2   4.64648         196.395950          178.779465      93.587057   False
7539466         2848131   38     1    2        3         2      3  ...   0.00000         1   5.53231          80.470405          736.891913      72.547987   False
7539708         2848373   21     1    1        3         2      3  ...   0.00000         3   4.17010          19.846674          129.982278      17.217740    True

[89 rows x 117 columns] {}
before logit.utils_to_probs 
               work1        work2   school1      school2  work_and_school
person_id                                                               
264107       0.0000    -3.773365 -998.1569 -1998.000000      -999.576426
264248       0.0000    -4.109360 -998.4069 -1998.000000      -999.831764
323689       0.0000    -3.997290 -998.5051 -1998.000000     -1000.169096
324051       0.0000    -2.630965 -995.2531 -1998.000000      -995.338966
324052       0.0000    -2.863745 -995.0939 -1998.000000      -995.690238
...             ...          ...       ...          ...              ...
7513546      0.0000    -3.758560 -998.4143 -1998.000000      -999.562044
7513554      0.0000    -4.301155 -994.3721 -1998.000000      -996.010222
7513565      4.6077     0.677960    0.1592    -3.334936         2.193340
7539466   -996.9434 -1001.697200    0.1592    -4.603238     -1000.763317
7539708   -997.5800 -1001.609200    0.0000    -3.452472      -999.452348

[89 rows x 5 columns]
[00:13.26] INFO: mandatory_tour_frequency top 10 value counts:
                   78
work1              61
school1            22
work_and_school     4
work2               2
Name: mandatory_tour_frequency, dtype: int64
Time to execute step 'mandatory_tour_frequency': 0.45 s
Total time to execute iteration 1 with iteration value None: 0.45 s
[00:13.35] INFO: #run_model running step mandatory_tour_scheduling
Running step 'mandatory_tour_scheduling'
[00:13.38] DEBUG: @inject timetable
[00:13.42] INFO: Running mandatory_tour_scheduling with 95 tours
[00:13.42] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.work schedule_tours running 67 tour choices
[00:13.42] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.work Running adaptive_chunked_choosers with 67 choosers
[00:13.42] INFO: Running chunk 1 of 1 with 67 of 67 choosers
[00:13.42] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.work schedule_tours running 67 tour choices
[00:13.43] INFO: tdd_alt_segments specified for representative logsums
[00:13.45] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.work.compute_logsums compute_logsums deduped_alt_tdds reduced number of rows by 92.11% from 12730 to 1005 compared to USE_BRUTE_FORCE_TO_COMPUTE_LOGSUMS
[00:13.47] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.work.compute_logsums.logsums compute_logsums for 1005 choosers 1005 alts
get item DIST
get item DISTWALK
get item DISTBIKE
[00:13.65] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.work.compute_logsums.logsums Running adaptive_chunked_choosers with 1005 choosers
[00:13.65] INFO: Running chunk 1 of 1 with 1005 of 1005 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:14.23] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.work.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 67 choosers and 12730 alternatives
[00:14.23] INFO: Running chunk 1 of 1 with 67 of 67 choosers
[00:14.25] INFO: Running eval_interaction_utilities on 12730 rows
[00:14.47] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.school schedule_tours running 17 tour choices
[00:14.47] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.school Running adaptive_chunked_choosers with 17 choosers
[00:14.47] INFO: Running chunk 1 of 1 with 17 of 17 choosers
[00:14.47] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.school schedule_tours running 17 tour choices
[00:14.48] INFO: tdd_alt_segments specified for representative logsums
[00:14.49] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.school.compute_logsums compute_logsums deduped_alt_tdds reduced number of rows by 92.11% from 3230 to 255 compared to USE_BRUTE_FORCE_TO_COMPUTE_LOGSUMS
[00:14.50] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.school.compute_logsums.logsums compute_logsums for 255 choosers 255 alts
get item DIST
get item DISTWALK
get item DISTBIKE
[00:14.67] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.school.compute_logsums.logsums Running adaptive_chunked_choosers with 255 choosers
[00:14.67] INFO: Running chunk 1 of 1 with 255 of 255 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:15.07] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.school.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 17 choosers and 3230 alternatives
[00:15.08] INFO: Running chunk 1 of 1 with 17 of 17 choosers
[00:15.08] INFO: Running eval_interaction_utilities on 3230 rows
[00:15.23] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.univ schedule_tours running 5 tour choices
[00:15.23] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.univ Running adaptive_chunked_choosers with 5 choosers
[00:15.23] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:15.23] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.univ schedule_tours running 5 tour choices
[00:15.23] INFO: tdd_alt_segments specified for representative logsums
[00:15.24] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.univ.compute_logsums compute_logsums deduped_alt_tdds reduced number of rows by 92.11% from 950 to 75 compared to USE_BRUTE_FORCE_TO_COMPUTE_LOGSUMS
[00:15.26] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.univ.compute_logsums.logsums compute_logsums for 75 choosers 75 alts
get item DIST
get item DISTWALK
get item DISTBIKE
[00:15.42] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.univ.compute_logsums.logsums Running adaptive_chunked_choosers with 75 choosers
[00:15.42] INFO: Running chunk 1 of 1 with 75 of 75 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:15.80] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.univ.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 5 choosers and 950 alternatives
[00:15.81] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:15.81] INFO: Running eval_interaction_utilities on 950 rows
[00:15.94] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.work schedule_tours running 2 tour choices
[00:15.94] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.work Running adaptive_chunked_choosers with 2 choosers
[00:15.94] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:15.94] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.work schedule_tours running 2 tour choices
[00:15.94] INFO: tdd_alt_segments specified for representative logsums
[00:15.95] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.work.compute_logsums compute_logsums deduped_alt_tdds reduced number of rows by 89.71% from 175 to 18 compared to USE_BRUTE_FORCE_TO_COMPUTE_LOGSUMS
[00:15.96] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.work.compute_logsums.logsums compute_logsums for 18 choosers 18 alts
get item DIST
get item DISTWALK
get item DISTBIKE
[00:16.13] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.work.compute_logsums.logsums Running adaptive_chunked_choosers with 18 choosers
[00:16.13] INFO: Running chunk 1 of 1 with 18 of 18 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:16.52] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.work.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 2 choosers and 175 alternatives
[00:16.53] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:16.53] INFO: Running eval_interaction_utilities on 175 rows
[00:16.64] INFO: skipping empty segment school
[00:16.64] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.univ schedule_tours running 4 tour choices
[00:16.64] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.univ Running adaptive_chunked_choosers with 4 choosers
[00:16.64] INFO: Running chunk 1 of 1 with 4 of 4 choosers
[00:16.64] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.univ schedule_tours running 4 tour choices
[00:16.64] INFO: tdd_alt_segments specified for representative logsums
[00:16.65] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.univ.compute_logsums compute_logsums deduped_alt_tdds reduced number of rows by 90.16% from 366 to 36 compared to USE_BRUTE_FORCE_TO_COMPUTE_LOGSUMS
[00:16.66] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.univ.compute_logsums.logsums compute_logsums for 36 choosers 36 alts
get item DIST
get item DISTWALK
get item DISTBIKE
[00:16.82] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.univ.compute_logsums.logsums Running adaptive_chunked_choosers with 36 choosers
[00:16.82] INFO: Running chunk 1 of 1 with 36 of 36 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:17.17] INFO: mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.univ.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 4 choosers and 366 alternatives
[00:17.17] INFO: Running chunk 1 of 1 with 4 of 4 choosers
[00:17.17] INFO: Running eval_interaction_utilities on 366 rows
Time to execute step 'mandatory_tour_scheduling': 3.94 s
Total time to execute iteration 1 with iteration value None: 3.94 s
[00:17.36] INFO: #run_model running step joint_tour_frequency
Running step 'joint_tour_frequency'
[00:17.37] INFO: Running joint_tour_frequency with 36 multi-person households
[00:17.38] DEBUG: @inject timetable
[00:17.50] INFO: joint_tour_frequency.simple_simulate Running adaptive_chunked_choosers with 36 choosers
[00:17.50] INFO: Running chunk 1 of 1 with 36 of 36 choosers
before eval utilities                                                                                      0_tours  1_Shop  1_Main   1_Eat  1_Visit  ...     2_EV     2_ED     2_VV     2_VD     2_DD
Expression                                Label                                                                                ...                                             
1                                         util_alternative_specific_constants         0.0000 -6.0149 -5.7389 -6.3757  -5.8818  ... -14.4576 -14.4576 -14.4576 -14.4576 -14.4576
cdap_home_full_max3                       util_fullTimeHomeMaxThree_zero_tours        1.1750  0.0000  0.0000  0.0000   0.0000  ...   0.0000   0.0000   0.0000   0.0000   0.0000
cdap_home_part_max3                       util_partTimeHomeMaxThree_zero_tours        1.4470  0.0000  0.0000  0.0000   0.0000  ...   0.0000   0.0000   0.0000   0.0000   0.0000
cdap_home_nonwork_max3                    util_nonWorkerHomeMaxThree_zero_tours       1.5140  0.0000  0.0000  0.0000   0.0000  ...   0.0000   0.0000   0.0000   0.0000   0.0000
cdap_home_retired_max3                    util_retireeHomeMaxThree_zero_tours         0.6053  0.0000  0.0000  0.0000   0.0000  ...   0.0000   0.0000   0.0000   0.0000   0.0000
...                                                                                      ...     ...     ...     ...      ...  ...      ...      ...      ...      ...      ...
log_time_window_overlap_adult_child # (4) util_logTimeWindowOverlapAdultChild_disc    0.0000  0.0000  0.0000  0.0000   0.0000  ...   0.0000   0.2212   0.0000   0.2212   0.4424
income_between_50_and_100 # (2)           util_incomeBetween50And100_disc             0.0000  0.0000  0.0000  0.0000   0.0000  ...   0.0000   0.3167   0.0000   0.3167   0.6334
income_greater_than_100 # (2)             util_incomeGreaterThan100_disc              0.0000  0.0000  0.0000  0.0000   0.0000  ...   0.0000   0.4860   0.0000   0.4860   0.9720
income_missing # (2)                      util_incomeMissing_dummy_always_zero_disc   0.0000  0.0000  0.0000  0.0000   0.0000  ...   0.0000   0.3723   0.0000   0.3723   0.7446
auto_ownership == 0 # (2)                 util_zeroAutomobiles_dis                    0.0000  0.0000  0.0000  0.0000   0.0000  ...   0.0000  -0.9090   0.0000  -0.9090  -1.8180

[70 rows x 21 columns]               home_zone_id  income  hhsize  ...  log_time_window_overlap_child  log_time_window_overlap_adult_child  non_motorized_retail_accessibility
household_id                                ...                                                                                                        
982875                  16   30900       2  ...                       0.000000                             0.000000                            8.247303
1810015                 16   99700       9  ...                       0.000000                             2.302734                            8.247303
1099626                 20   58160       3  ...                       0.000000                             1.946289                            7.494197
1196298                 25   31360       5  ...                       2.302734                             2.484375                            7.616518
1363467                 24   58300       2  ...                       0.000000                             0.000000                            7.713628
386761                  16   21000       3  ...                       0.000000                             2.080078                            8.247303
2223027                  9  133000       2  ...                       0.000000                             0.000000                            7.415630
2727273                 20  118800       2  ...                       0.000000                             0.000000                            7.494197
2223759                 16  144100       2  ...                       0.000000                             0.000000                            8.247303
1173905                  8   45000       4  ...                       1.946289                             2.398438                            7.981950
386699                   7   21000       3  ...                       0.000000                             2.197266                            8.016915
1594621                 10   69200       2  ...                       0.000000                             0.000000                            7.567826
257531                  16   12000       2  ...                       0.000000                             0.000000                            8.247303
1645132                  9   92700       2  ...                       0.000000                             2.996094                            7.415630
1402945                 25   37200       4  ...                       0.000000                             0.000000                            7.616518
1747467                 16   76000       3  ...                       0.000000                             2.398438                            8.247303
1511234                  8   88600       2  ...                       0.000000                             0.000000                            7.981950
703381                  25   17210       2  ...                       0.000000                             0.000000                            7.616518
226869                   9    4000       2  ...                       0.000000                             0.000000                            7.415630
1259353                 10   43000       6  ...                       2.302734                             2.996094                            7.567826
1024353                  9   58000       2  ...                       0.000000                             2.398438                            7.415630
2222764                  9  141200       2  ...                       0.000000                             0.000000                            7.415630
983048                  17   59000       2  ...                       0.000000                             0.000000                            7.667142
932147                   7   47900       2  ...                       0.000000                             0.000000                            8.016915
2223022                  9  112000       2  ...                       0.000000                             0.000000                            7.415630
304036                  10   23700       2  ...                       0.000000                             2.080078                            7.567826
727854                   7   24600       2  ...                       0.000000                             0.000000                            8.016915
1594490                  9   75000       2  ...                       0.000000                             0.000000                            7.415630
256841                  16   16500       2  ...                       0.000000                             0.000000                            8.247303
257627                  16   18100       2  ...                       0.000000                             0.000000                            8.247303
2224629                 17  385000       2  ...                       0.000000                             0.000000                            7.667142
1119939                 16   45000       3  ...                       0.000000                             1.946289                            8.247303
287846                   9   22600       2  ...                       0.000000                             2.302734                            7.415630
1594311                  6   69200       2  ...                       0.000000                             0.000000                            7.838241
287819                   9    6500       2  ...                       0.000000                             2.484375                            7.415630
2222549                  7  112500       2  ...                       0.000000                             0.000000                            8.016915

[36 rows x 57 columns] {}
before logit.utils_to_probs 
               0_tours    1_Shop    1_Main     1_Eat  1_Visit    1_Disc       2_SS  ...       2_MD       2_EE       2_EV       2_ED     2_VV       2_VD       2_DD
household_id                                                                       ...                                                                           
982875         0.0000 -3.896531 -4.346448 -4.133049  -5.1101 -3.560912 -10.220862  ... -11.145460  -9.972298 -11.443249 -10.295261 -12.9142 -11.766212 -10.618224
1810015        3.4625 -2.092746 -2.905293 -3.353682  -2.8765 -2.186774  -6.613290  ...  -8.330167  -8.413564  -8.430282  -8.141756  -8.4470  -8.158474  -7.869948
1099626        0.0000 -4.636564 -4.474270 -4.488989  -5.3706 -3.486793 -11.700928  ... -11.199162 -10.684177 -12.059688 -10.577081 -13.4352 -11.952593 -10.469986
1196298        1.5140 -4.424823 -3.565345 -4.664579  -6.0802 -3.700828 -11.277446  ... -10.504272 -11.035358 -12.944879 -10.966706 -14.8544 -12.876227 -10.898055
1363467        0.0000 -3.344639 -3.450203 -3.966943  -4.6683 -3.698656  -9.117078  ... -10.386959  -9.640085 -10.835342 -10.266898 -12.0306 -11.462156 -10.893712
386761         0.0000 -4.094897 -3.449996 -4.435206  -4.6409 -3.793736 -10.617594  ... -10.481832 -10.576612 -11.276206 -10.830242 -11.9758 -11.529836 -11.083872
2223027        0.0000 -5.263094 -5.568159 -4.456645  -6.4446 -5.190549 -12.953987  ... -13.996808 -10.619490 -13.101345 -12.248494 -15.5832 -14.730349 -13.877498
2727273        0.0000 -4.605531 -4.555048 -4.516742  -5.5242 -4.327412 -11.638862  ... -12.120560 -10.739683 -12.241041 -11.445454 -13.7424 -12.946812 -12.151224
2223759        0.0000 -5.542927 -5.742978 -4.633655  -6.6234 -5.351906 -13.513653  ... -14.332984 -10.973509 -13.457154 -12.586861 -15.9408 -15.070506 -14.200212
1173905        0.0000 -2.332458 -1.848266 -3.458520  -3.3432 -2.350246  -7.092715  ...  -7.436611  -8.623239  -9.001820  -8.410065  -9.3804  -8.788646  -8.196891
386699         0.0000 -5.194273 -4.758081 -5.002893  -6.4782 -5.041998 -12.816345  ... -13.038178 -11.711985 -13.681192 -12.646190 -15.6504 -14.615397 -13.580395
1594621        0.0000 -5.020694 -5.267259 -4.598709  -6.4446 -5.359849 -12.469187  ... -13.865208 -10.903617 -13.243409 -12.559858 -15.5832 -14.899649 -14.216098
257531         0.0000 -4.591694 -4.721159 -4.477781  -6.3114 -5.178649 -11.611187  ... -13.137908 -10.661762 -12.989281 -12.257730 -15.3168 -14.585249 -13.853698
1645132        0.0000 -4.377324 -4.094455 -4.547491  -5.4937 -4.122464 -11.182448  ... -11.455018 -10.801181 -12.241290 -11.271254 -13.6814 -12.711364 -11.741328
1402945        0.0000 -4.525994 -5.152759 -4.893390  -5.4646 -4.381149 -11.479787  ... -12.772008 -11.492980 -12.558090 -11.875839 -13.6232 -12.940949 -12.258698
1747467        0.0000 -4.503955 -4.162697 -3.902279  -5.9958 -3.905043 -11.435710  ... -11.305840  -9.510758 -12.098179 -10.408621 -14.6856 -12.996043 -11.306485
1511234        0.0000 -4.255894 -5.002959 -4.163333  -6.1637 -4.605249 -10.939587  ... -12.846308 -10.032866 -12.527133 -11.369882 -15.0214 -13.864149 -12.706898
703381         0.0000 -2.402123 -2.355151 -3.213273  -3.6336 -3.226339  -7.232044  ...  -8.819590  -8.132745  -9.046972  -9.040911  -9.9612  -9.955139  -9.949078
226869         0.0000 -4.241761 -4.609054 -4.863740  -6.0163 -5.488205 -10.911321  ... -13.335359 -11.433679 -13.080139 -12.953244 -14.7266 -14.599705 -14.472810
1259353        5.2060 -3.329980 -3.121724 -4.175651  -4.1802 -2.943825  -9.087758  ...  -9.303649 -10.057503 -10.555951  -9.720776 -11.0544 -10.219225  -9.384050
1024353        0.0000 -5.046630 -4.458762 -5.215335  -6.0583 -4.869065 -12.521059  ... -12.565927 -12.136869 -13.473735 -12.685700 -14.8106 -14.022565 -13.234531
2222764        0.0000 -4.745826 -4.906436 -4.172239  -5.7405 -5.022877 -11.919451  ... -13.167413 -10.050677 -12.112838 -11.796416 -14.1750 -13.858577 -13.542154
983048         0.0000 -5.180596 -5.387473 -4.929247  -5.7030 -4.495333 -12.788991  ... -13.120905 -11.564693 -12.832346 -12.025879 -14.1000 -13.293532 -12.487065
932147         0.0000 -4.847931 -5.179648 -4.933533  -5.5242 -4.620212 -12.123662  ... -13.037960 -11.573266 -12.657833 -12.155045 -13.7424 -13.239612 -12.736824
2223022        0.0000 -4.683125 -4.867265 -4.121023  -5.6809 -4.986723 -11.794048  ... -13.092087  -9.948246 -12.002023 -11.709045 -14.0558 -13.762822 -13.469845
304036         0.0000 -6.031404 -5.301737 -5.714920  -5.7026 -4.634287 -14.490607  ... -13.174123 -13.136039 -13.617619 -12.950506 -14.0992 -13.432086 -12.764973
727854         0.0000 -4.070494 -4.502059 -4.686565  -5.8971 -5.389449 -10.568787  ... -13.129608 -11.079330 -12.783765 -12.677314 -14.4882 -14.381749 -14.275298
1594490        0.0000 -5.182061 -5.214154 -4.748040  -5.5838 -4.549605 -12.791921  ... -13.001859 -11.202279 -12.531939 -11.898944 -13.8616 -13.228605 -12.595610
256841         0.0000 -5.090331 -5.156848 -4.919249  -5.5242 -4.813412 -12.608462  ... -13.208360 -11.544698 -12.643549 -12.333961 -13.7424 -13.432812 -13.123224
257627         0.0000 -4.847931 -5.179648 -4.919249  -5.5242 -4.620212 -12.123662  ... -13.037960 -11.544698 -12.643549 -12.140761 -13.7424 -13.239612 -12.736824
2224629        0.0000 -5.182061 -5.214154 -4.580946  -5.5838 -4.380305 -12.791921  ... -12.832559 -10.868091 -12.364845 -11.562551 -13.8616 -13.059305 -12.257010
1119939        0.0000 -4.878964 -4.451470 -4.739996  -5.3706 -3.996693 -12.185728  ... -11.686262 -11.186192 -12.310696 -11.337989 -13.4352 -12.462493 -11.489786
287846         0.0000 -5.107423 -4.574742 -5.139349  -5.1039 -4.490935 -12.642646  ... -12.303777 -11.984898 -12.443349 -12.231584 -12.9018 -12.690035 -12.478270
1594311        0.0000 -3.841922 -4.063651 -3.533126  -4.9517 -4.420439 -10.111644  ... -11.722189  -8.772452 -10.684926 -10.554865 -12.5974 -12.467339 -12.337278
287819         0.0000 -5.282997 -4.637767 -4.894920  -5.3193 -3.790256 -12.993793  ... -11.666123 -11.496040 -12.414320 -11.286476 -13.3326 -12.204756 -11.076912
2222549        0.0000 -4.069294 -4.757759 -3.633165  -6.0305 -3.938049 -10.566387  ... -11.933908  -8.972530 -11.863765 -10.172514 -14.7550 -13.063749 -11.372498

[36 rows x 21 columns]
[00:17.74] WARNING: register tours: no rows with household_id in [982875].
[00:17.74] INFO: joint_tour_frequency top 10 value counts:
0_tours    97
1_Eat       1
1_Shop      1
1_Disc      1
Name: joint_tour_frequency, dtype: int64
Time to execute step 'joint_tour_frequency': 0.38 s
Total time to execute iteration 1 with iteration value None: 0.38 s
[00:17.83] INFO: #run_model running step joint_tour_composition
Running step 'joint_tour_composition'
[00:17.83] INFO: Running joint_tour_composition with 3 joint tours
[00:17.85] DEBUG: @inject timetable
[00:17.91] INFO: joint_tour_composition.simple_simulate Running adaptive_chunked_choosers with 3 choosers
[00:17.91] INFO: Running chunk 1 of 1 with 3 of 3 choosers
before eval utilities                                                                                                      adults  children     mixed
Expression                                         Label                                                                       
1                                                  util_asc                                          0.0000    5.3517    5.6290
tour_type=='eat'                                   util_tour_purpose_is_eating_out                   0.0000   -0.9678   -0.8027
tour_type=='disc'                                  util_tour_purpose_is_discretionary                0.7648    0.5101    0.0000
num_full_max3                                      util_number_of_full_time_workers                  1.0240    0.0000    0.3624
num_part_max3                                      util_number_of_part_time_workers                  0.5412    0.0000    0.3164
num_univ_max3                                      util_number_of_university_students                0.8245    0.0000    0.0000
num_nonwork_max3                                   util_number_of_non_workers                        0.6263    0.0000   -0.3724
num_preschool_max3                                 util_number_of_children_too_young_for_school      0.0000    0.7306    0.7906
num_school_max3                                    util_number_of_pre_driving_age_children           0.0000    0.7306    0.3532
num_driving_max3                                   util_number_of_driving_age_children               0.0000   -0.2667   -0.9399
income_in_thousands<30                             util_low_income_households                        1.2480    0.0000    0.5755
(income_in_thousands>=30) & (income_in_thousand... util_medium_income_households                     0.8369    0.0000    0.0000
more_cars_than_workers                             util_household_has_more_cars_than_workers         1.3860    0.0000    0.7510
home_is_urban                                      util_household_in_urban_area                      0.5741    0.0000    0.0000
~(home_is_urban | home_is_rural)                   util_household_in_suburban_area                   0.5105    0.0000    0.1283
log_time_window_overlap_adult                      util_log_max_overlap_of_adults_time_windows       1.1920    0.0000    0.0000
log_time_window_overlap_child                      util_log_max_overlap_of_childrens_time_windows    0.0000    1.8410    0.0000
log_time_window_overlap_adult_child                util_log_max_overlap_of_time_windows              0.0000    0.0000    1.9580
num_travel_active_adults<2                         util_two_acive_adults                          -999.0000    0.0000    0.0000
num_travel_active_children<2                       util_two_active_children                          0.0000 -999.0000    0.0000
(num_travel_active_adults == 0) | (num_travel_a... util_travel_active_adult                          0.0000    0.0000 -999.0000            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_nonwork_max3 num_preschool_max3  num_school_max3  num_driving_max3  more_cars_than_workers
tour_id                                                                   ...                                                                                                
220958279    5389226    eatout                1              1         1  ...               0.0                0.0              0.0               0.0                   False
100798519    2458500  shopping                1              1         1  ...               2.0                0.0              2.0               0.0                    True
130727777    3188482  othdiscr                1              1         1  ...               0.0                0.0              0.0               0.0                   False

[3 rows x 63 columns] {}
before logit.utils_to_probs 
              adults    children       mixed
tour_id                                    
220958279  4.540475 -993.648300 -992.646200
100798519  7.620944   10.396018   11.037741
130727777  6.621853 -993.648300 -992.738200
[00:18.00] INFO: joint_tour_composition top 10 value counts:
adults      2
children    1
Name: composition, dtype: int64
Time to execute step 'joint_tour_composition': 0.17 s
Total time to execute iteration 1 with iteration value None: 0.17 s
[00:18.07] INFO: #run_model running step joint_tour_participation
Running step 'joint_tour_participation'
[00:18.09] WARNING: register joint_tour_participants: no rows with household_id in [982875].
[00:18.09] INFO: Running joint_tours_participation with 8 potential participants (candidates)
[00:18.11] DEBUG: @inject timetable
[00:18.16] INFO: joint_tour_participation Running chunk 1 of 1 with 3 of 3 choosers
before eval utilities                                                                                                        participate  not_participate
Expression                                         Label                                                                           
person_is_full & tour_composition_is_mixed         util_full_time_worker_mixed_party                      -3.56600              0.5
person_is_part & tour_composition_is_adults        util_part_time_worker_adults_only_party                -3.56600              0.5
person_is_part & tour_composition_is_mixed         util_part_time_worker_mixed_party                      -0.36550              0.0
person_is_univ & tour_composition_is_mixed         util_university_student_mixed_party                    -3.04100              0.0
person_is_nonwork & tour_composition_is_adults     util_non_worker_adults_only_party                      -3.16400              0.0
person_is_nonwork & tour_composition_is_mixed      util_non_worker_mixed_party                             0.71520              0.0
person_is_preschool & tour_composition_is_children util_child_too_young_for_school_children_only_p...     -2.78600              0.0
person_is_preschool & tour_composition_is_mixed    util_child_too_young_for_school_mixed_party            -1.89300              0.0
person_is_school & tour_composition_is_children    util_pre_driving_age_student_children_only_party       -0.72170              0.0
person_is_school & tour_composition_is_mixed       util_pre_driving_age_student_mixed_party               -1.75200              0.0
person_is_driving & tour_composition_is_children   util_driving_age_student_children_only_party           -1.82200              0.0
person_is_driving & tour_composition_is_mixed      util_driving_age_student_mixed_party                   -1.35300              0.0
person_is_full & tour_type_is_eat                  util_full_time_worker_specific_to_eating_out_jo...      0.71570              0.5
person_is_full & tour_type_is_disc                 util_full_time_worker_specific_to_discretionary...      0.43920              0.5
person_is_part & tour_type_is_eat                  util_part_time_worker_specific_to_eating_out_jo...      2.18800              0.0
person_is_part & tour_type_is_disc                 util_part_time_worker_specific_to_discretionary...      0.28500              0.0
person_is_univ & tour_type_is_eat                  util_university_student_specific_to_eating_out_...     -0.82000              0.0
person_is_nonwork & tour_type_is_eat               util_non_worker_specific_to_eating_out_joint_tours      0.16170              0.0
person_is_nonwork & tour_type_is_disc              util_non_worker_specific_to_discretionary_joint...     -0.18350              0.0
person_is_preschool & tour_type_is_eat             util_child_too_young_for_school_specific_to_eat...      0.65890              0.0
person_is_preschool & tour_type_is_disc            util_child_too_young_for_school_specific_to_dis...      0.12840              0.0
person_is_school & tour_type_is_eat                util_pre_driving_age_student_specific_to_eating...      1.39100              0.0
person_is_school & tour_type_is_disc               util_pre_driving_age_student_specific_to_discre...      0.66260              0.0
person_is_driving & tour_type_is_eat               util_driving_age_student_specific_to_eating_out...      2.34400              0.0
person_is_driving & tour_type_is_disc              util_driving_age_student_specific_to_discretion...     -0.66750              0.0
home_is_urban & adult & tour_composition_is_mixed  util_household_in_urban_area_adult_mixed_party         -0.13700              0.0
home_is_urban & ~adult & tour_composition_is_ch... util_household_in_urban_area_child_child_only_p...      1.21000              0.0
home_is_urban & ~adult & tour_composition_is_mixed util_household_in_urban_area_child_mixed_party          0.62650              0.0
home_is_suburban & adult & tour_composition_is_... util_household_in_suburban_area_adult_mixed_party      -0.06007              0.0
adult & more_cars_than_workers & tour_compositi... util_adult_more_automobiles_than_workers_adult_...     -0.21330              0.0
adult & more_cars_than_workers & tour_compositi... util_adult_more_automobiles_than_workers_mixed_...     -0.60310              0.0
adult & more_cars_than_workers & tour_compositi... util_child_more_automobiles_than_workers_child_...     -0.42140              0.0
adult & more_cars_than_workers & tour_compositi... util_child_more_automobiles_than_workers_mixed_...     -0.37760              0.0
high_income & tour_composition_is_adults           util_dummy_for_high_income_for_adult_in_adult_p...     -0.16820              0.0
high_income & tour_composition_is_mixed            util_dummy_for_high_income_for_adult_in_mixed_p...     -0.02613              0.0
high_income & tour_composition_is_children         util_dummy_for_high_income_for_child_in_childre...     -0.56190              0.0
high_income & tour_composition_is_mixed # (2)      util_dummy_for_high_income_for_child_in_mixed_p...     -0.15280              0.0
(adult & tour_composition_is_adults) * num_hh_j... util_adult_number_of_joint_tours_adult_only            -0.32420              0.0
(adult & tour_composition_is_mixed) * num_hh_jo... util_adult_number_of_joint_tours_mixed                 -0.35840              0.0
(~adult & tour_composition_is_children) * num_h... util_child_number_of_joint_tours_child_only             0.10470              0.0
(~adult & tour_composition_is_mixed) * num_hh_j... util_child_number_of_joint_tours_mixed                 -0.50890              0.0
(adult & tour_composition_is_adults) * log_time... util_adult_log_of_max_window_overlap_with_an_ad...      0.84360              0.0
(adult & tour_composition_is_mixed) * log_time_... util_adult_log_of_max_window_overlap_with_a_chi...      2.18900              0.0
(~adult & tour_composition_is_mixed) * log_time... util_child_log_of_max_window_overlap_with_an_ad...      1.53800              0.0
(~adult & tour_composition_is_children) * log_t... util_child_log_of_max_window_overlap_with_a_chi...      1.29600              0.0
adult & tour_composition_is_children               util_adults_are_prohibited_in_participating_in_...   -999.00000              0.0
~adult & tour_composition_is_adults                util_children_are_prohibited_in_participating_i...   -999.00000              0.0
~travel_active                                     util_persons_with_home_activity_patterns_are_pr...   -999.00000              0.0
adult & travel_active & tour_composition_is_adu... util_if_only_two_available_adults_both_must_par...      0.00000           -999.0
adult & travel_active & tour_composition_is_mix... util_if_only_one_available_adult_traveler_must_...      0.00000           -999.0
~adult & travel_active & tour_composition_is_ch... util_if_only_two_available_children_both_must_p...      0.00000           -999.0
~adult & travel_active & tour_composition_is_mi... util_if_only_one_available_child_traveler_must_...      0.00000           -999.0                   tour_id  point_person_id tour_type  tour_type_count  ...  home_is_suburban  high_income  more_cars_than_workers chunk_id
participant_id                                                         ...                                                                
22095827901     220958279          5389226    eatout                1  ...             False         True                   False        0
22095827902     220958279          5389226    eatout                1  ...             False         True                   False        0
10079851903     100798519          2458500  shopping                1  ...             False        False                    True        1
10079851904     100798519          2458500  shopping                1  ...             False        False                    True        1
13072777701     130727777          3188482  othdiscr                1  ...             False        False                   False        2
13072777702     130727777          3188482  othdiscr                1  ...             False        False                   False        2
13072777703     130727777          3188482  othdiscr                1  ...             False        False                   False        2
13072777704     130727777          3188482  othdiscr                1  ...             False        False                   False        2

[8 rows x 163 columns] {}
before logit.utils_to_probs 
                 participate  not_participate
participant_id                              
22095827901        0.865269           -999.0
22095827902        0.865269           -999.0
10079851903        0.593000           -999.0
10079851904        0.593000           -999.0
13072777701       -2.135446              0.5
13072777702       -2.135446              0.5
13072777703        1.430554              0.0
13072777704        1.187525              0.0
[00:18.44] INFO: joint_tour_participation.eval_mnl.participants_chooser 3 joint tours to satisfy.
[00:18.45] INFO: joint_tour_participation.eval_mnl.participants_chooser 1 iterations to satisfy all joint tours.
Time to execute step 'joint_tour_participation': 0.41 s
Total time to execute iteration 1 with iteration value None: 0.41 s
[00:18.57] INFO: #run_model running step joint_tour_destination
Running step 'joint_tour_destination'
[00:18.60] INFO: running joint_tour_destination.shopping.sample with 1 tours
[00:18.60] INFO: joint_tour_destination.shopping.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:18.60] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:18.61] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DIST
[00:18.64] INFO: Running joint_tour_destination.shopping.logsums with 11 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:18.79] INFO: joint_tour_destination.shopping.logsums.compute_logsums Running adaptive_chunked_choosers with 11 choosers
[00:18.79] INFO: Running chunk 1 of 1 with 11 of 11 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:19.14] INFO: Running tour_destination_simulate with 1 persons
[00:19.14] INFO: joint_tour_destination.shopping.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 11 alternatives
[00:19.14] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:19.15] INFO: Running eval_interaction_utilities on 11 rows
get item DIST
get item DIST
get item DIST
[00:19.16] INFO: joint_tour_destination skipping segment othmaint: no choosers
[00:19.17] INFO: running joint_tour_destination.othdiscr.sample with 1 tours
[00:19.17] INFO: joint_tour_destination.othdiscr.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:19.17] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:19.17] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:19.20] INFO: Running joint_tour_destination.othdiscr.logsums with 15 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:19.37] INFO: joint_tour_destination.othdiscr.logsums.compute_logsums Running adaptive_chunked_choosers with 15 choosers
[00:19.37] INFO: Running chunk 1 of 1 with 15 of 15 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:19.71] INFO: Running tour_destination_simulate with 1 persons
[00:19.71] INFO: joint_tour_destination.othdiscr.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 15 alternatives
[00:19.71] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:19.71] INFO: Running eval_interaction_utilities on 15 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:19.73] INFO: running joint_tour_destination.eatout.sample with 1 tours
[00:19.73] INFO: joint_tour_destination.eatout.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:19.73] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:19.74] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:19.81] INFO: Running joint_tour_destination.eatout.logsums with 16 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:19.98] INFO: joint_tour_destination.eatout.logsums.compute_logsums Running adaptive_chunked_choosers with 16 choosers
[00:19.98] INFO: Running chunk 1 of 1 with 16 of 16 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:20.32] INFO: Running tour_destination_simulate with 1 persons
[00:20.32] INFO: joint_tour_destination.eatout.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 16 alternatives
[00:20.32] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:20.32] INFO: Running eval_interaction_utilities on 16 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:20.34] INFO: joint_tour_destination skipping segment social: no choosers
[00:20.34] INFO: joint_tour_destination skipping segment escort: no choosers
[00:20.34] INFO: destination summary:
count     3.000000
mean     14.000000
std       9.539392
min       5.000000
25%       9.000000
50%      13.000000
75%      18.500000
max      24.000000
Name: destination, dtype: float64
Time to execute step 'joint_tour_destination': 1.77 s
Total time to execute iteration 1 with iteration value None: 1.77 s
[00:20.41] INFO: #run_model running step joint_tour_scheduling
Running step 'joint_tour_scheduling'
[00:20.42] INFO: Running joint_tour_scheduling with 3 joint tours
[00:20.44] DEBUG: @inject timetable
[00:20.49] INFO: schedule_tours %s tours not monotonic_increasing - sorting df
[00:20.49] INFO: joint_tour_scheduling.vectorize_joint_tour_scheduling.tour_1 schedule_tours running 3 tour choices
[00:20.49] INFO: joint_tour_scheduling.vectorize_joint_tour_scheduling.tour_1 Running adaptive_chunked_choosers with 3 choosers
[00:20.49] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:20.49] INFO: joint_tour_scheduling.vectorize_joint_tour_scheduling.tour_1 schedule_tours running 3 tour choices
[00:20.50] INFO: joint_tour_scheduling.vectorize_joint_tour_scheduling.tour_1.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 3 choosers and 65 alternatives
[00:20.50] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:20.50] INFO: Running eval_interaction_utilities on 65 rows
Time to execute step 'joint_tour_scheduling': 0.27 s
Total time to execute iteration 1 with iteration value None: 0.27 s
[00:20.76] INFO: #run_model running step non_mandatory_tour_frequency
Running step 'non_mandatory_tour_frequency'
[00:20.79] DEBUG: @inject timetable
[00:20.85] INFO: Running non_mandatory_tour_frequency with 137 persons
[00:20.86] INFO: Running segment 'PTYPE_FULL' of size 48
[00:20.86] INFO: non_mandatory_tour_frequency.PTYPE_FULL.interaction_simulate Running adaptive_chunked_choosers with 48 choosers
[00:20.86] INFO: Running chunk 1 of 1 with 48 of 48 choosers
[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.89] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.90] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.91] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.91] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.91] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.91] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:20.91] INFO: Running eval_interaction_utilities on 4608 rows
[00:21.31] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/tracing.py:836: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  trace_results[slicer_column_name] = trace_ids[1]

[00:21.36] INFO: Running segment 'PTYPE_PART' of size 26
[00:21.36] INFO: non_mandatory_tour_frequency.PTYPE_PART.interaction_simulate Running adaptive_chunked_choosers with 26 choosers
[00:21.36] INFO: Running chunk 1 of 1 with 26 of 26 choosers
[00:21.38] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.38] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.38] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.38] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.38] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.38] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.39] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.40] INFO: Running eval_interaction_utilities on 2496 rows
[00:21.71] INFO: Running segment 'PTYPE_UNIVERSITY' of size 16
[00:21.72] INFO: non_mandatory_tour_frequency.PTYPE_UNIVERSITY.interaction_simulate Running adaptive_chunked_choosers with 16 choosers
[00:21.72] INFO: Running chunk 1 of 1 with 16 of 16 choosers
[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.75] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.76] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.76] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.76] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.76] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.76] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.76] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.76] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.76] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:21.77] INFO: Running eval_interaction_utilities on 1536 rows
[00:22.14] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/tracing.py:836: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  trace_results[slicer_column_name] = trace_ids[1]

[00:22.19] INFO: Running segment 'PTYPE_NONWORK' of size 17
[00:22.19] INFO: non_mandatory_tour_frequency.PTYPE_NONWORK.interaction_simulate Running adaptive_chunked_choosers with 17 choosers
[00:22.19] INFO: Running chunk 1 of 1 with 17 of 17 choosers
[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.23] INFO: Running eval_interaction_utilities on 1632 rows
[00:22.59] INFO: Running segment 'PTYPE_RETIRED' of size 12
[00:22.60] INFO: non_mandatory_tour_frequency.PTYPE_RETIRED.interaction_simulate Running adaptive_chunked_choosers with 12 choosers
[00:22.60] INFO: Running chunk 1 of 1 with 12 of 12 choosers
[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.62] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.63] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.64] INFO: Running eval_interaction_utilities on 1152 rows
[00:22.89] INFO: Running segment 'PTYPE_DRIVING' of size 1
[00:22.90] INFO: non_mandatory_tour_frequency.PTYPE_DRIVING.interaction_simulate Running adaptive_chunked_choosers with 1 choosers
[00:22.90] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.92] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:22.93] INFO: Running eval_interaction_utilities on 96 rows
[00:23.18] INFO: Running segment 'PTYPE_SCHOOL' of size 11
[00:23.19] INFO: non_mandatory_tour_frequency.PTYPE_SCHOOL.interaction_simulate Running adaptive_chunked_choosers with 11 choosers
[00:23.19] INFO: Running chunk 1 of 1 with 11 of 11 choosers
[00:23.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.21] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.22] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.23] INFO: Running eval_interaction_utilities on 1056 rows
[00:23.60] INFO: Running segment 'PTYPE_PRESCHOOL' of size 6
[00:23.60] INFO: non_mandatory_tour_frequency.PTYPE_PRESCHOOL.interaction_simulate Running adaptive_chunked_choosers with 6 choosers
[00:23.60] INFO: Running chunk 1 of 1 with 6 of 6 choosers
[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.64] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.65] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/logit.py:333: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  alts_sample[c_chooser] = np.repeat(choosers[c].values, sample_size)

[00:23.66] INFO: Running eval_interaction_utilities on 576 rows
[00:23.80] INFO: extend_tour_counts increased tour count by 5 from 90 to 95
[00:23.84] INFO: non_mandatory_tour_frequency top 10 value counts:
0     102
16     17
1       8
8       8
4       7
9       4
17      3
20      3
2       3
12      2
Name: non_mandatory_tour_frequency, dtype: int64
Time to execute step 'non_mandatory_tour_frequency': 3.09 s
Total time to execute iteration 1 with iteration value None: 3.09 s
[00:23.93] INFO: #run_model running step non_mandatory_tour_destination
Running step 'non_mandatory_tour_destination'
[00:23.97] INFO: running non_mandatory_tour_destination.shopping.sample with 30 tours
[00:23.97] INFO: non_mandatory_tour_destination.shopping.sample.interaction_sample Running adaptive_chunked_choosers with 30 choosers
[00:23.97] INFO: Running chunk 1 of 1 with 30 of 30 choosers
[00:23.97] INFO: Running eval_interaction_utilities on 750 rows
get item DIST
get item DIST
get item DIST
[00:24.01] INFO: Running non_mandatory_tour_destination.shopping.logsums with 366 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:24.20] INFO: non_mandatory_tour_destination.shopping.logsums.compute_logsums Running adaptive_chunked_choosers with 366 choosers
[00:24.20] INFO: Running chunk 1 of 1 with 366 of 366 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:24.61] INFO: Running tour_destination_simulate with 30 persons
[00:24.61] INFO: non_mandatory_tour_destination.shopping.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 30 choosers and 366 alternatives
[00:24.61] INFO: Running chunk 1 of 1 with 30 of 30 choosers
[00:24.61] INFO: Running eval_interaction_utilities on 366 rows
get item DIST
get item DIST
get item DIST
[00:24.66] INFO: running non_mandatory_tour_destination.othmaint.sample with 20 tours
[00:24.66] INFO: non_mandatory_tour_destination.othmaint.sample.interaction_sample Running adaptive_chunked_choosers with 20 choosers
[00:24.66] INFO: Running chunk 1 of 1 with 20 of 20 choosers
[00:24.66] INFO: Running eval_interaction_utilities on 500 rows
get item DIST
get item DIST
get item DIST
[00:24.70] INFO: Running non_mandatory_tour_destination.othmaint.logsums with 286 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:24.87] INFO: non_mandatory_tour_destination.othmaint.logsums.compute_logsums Running adaptive_chunked_choosers with 286 choosers
[00:24.87] INFO: Running chunk 1 of 1 with 286 of 286 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:25.27] INFO: Running tour_destination_simulate with 20 persons
[00:25.27] INFO: non_mandatory_tour_destination.othmaint.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 20 choosers and 286 alternatives
[00:25.27] INFO: Running chunk 1 of 1 with 20 of 20 choosers
[00:25.27] INFO: Running eval_interaction_utilities on 286 rows
get item DIST
get item DIST
get item DIST
[00:25.32] INFO: running non_mandatory_tour_destination.othdiscr.sample with 20 tours
[00:25.32] INFO: non_mandatory_tour_destination.othdiscr.sample.interaction_sample Running adaptive_chunked_choosers with 20 choosers
[00:25.32] INFO: Running chunk 1 of 1 with 20 of 20 choosers
[00:25.32] INFO: Running eval_interaction_utilities on 500 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:25.36] INFO: Running non_mandatory_tour_destination.othdiscr.logsums with 291 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:25.52] INFO: non_mandatory_tour_destination.othdiscr.logsums.compute_logsums Running adaptive_chunked_choosers with 291 choosers
[00:25.52] INFO: Running chunk 1 of 1 with 291 of 291 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:25.93] INFO: Running tour_destination_simulate with 20 persons
[00:25.93] INFO: non_mandatory_tour_destination.othdiscr.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 20 choosers and 291 alternatives
[00:25.93] INFO: Running chunk 1 of 1 with 20 of 20 choosers
[00:25.94] INFO: Running eval_interaction_utilities on 291 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:25.98] INFO: running non_mandatory_tour_destination.eatout.sample with 15 tours
[00:25.99] INFO: non_mandatory_tour_destination.eatout.sample.interaction_sample Running adaptive_chunked_choosers with 15 choosers
[00:25.99] INFO: Running chunk 1 of 1 with 15 of 15 choosers
[00:25.99] INFO: Running eval_interaction_utilities on 375 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:26.08] INFO: Running non_mandatory_tour_destination.eatout.logsums with 203 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:26.27] INFO: non_mandatory_tour_destination.eatout.logsums.compute_logsums Running adaptive_chunked_choosers with 203 choosers
[00:26.27] INFO: Running chunk 1 of 1 with 203 of 203 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:26.77] INFO: Running tour_destination_simulate with 15 persons
[00:26.77] INFO: non_mandatory_tour_destination.eatout.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 15 choosers and 203 alternatives
[00:26.77] INFO: Running chunk 1 of 1 with 15 of 15 choosers
[00:26.78] INFO: Running eval_interaction_utilities on 203 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:26.81] INFO: running non_mandatory_tour_destination.social.sample with 7 tours
[00:26.81] INFO: non_mandatory_tour_destination.social.sample.interaction_sample Running adaptive_chunked_choosers with 7 choosers
[00:26.81] INFO: Running chunk 1 of 1 with 7 of 7 choosers
[00:26.82] INFO: Running eval_interaction_utilities on 175 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:26.85] INFO: Running non_mandatory_tour_destination.social.logsums with 97 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:27.01] INFO: non_mandatory_tour_destination.social.logsums.compute_logsums Running adaptive_chunked_choosers with 97 choosers
[00:27.01] INFO: Running chunk 1 of 1 with 97 of 97 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:27.40] INFO: Running tour_destination_simulate with 7 persons
[00:27.40] INFO: non_mandatory_tour_destination.social.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 7 choosers and 97 alternatives
[00:27.40] INFO: Running chunk 1 of 1 with 7 of 7 choosers
[00:27.40] INFO: Running eval_interaction_utilities on 97 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:27.45] INFO: running non_mandatory_tour_destination.escort.sample with 3 tours
[00:27.45] INFO: non_mandatory_tour_destination.escort.sample.interaction_sample Running adaptive_chunked_choosers with 3 choosers
[00:27.45] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:27.46] INFO: Running eval_interaction_utilities on 75 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:27.49] INFO: Running non_mandatory_tour_destination.escort.logsums with 41 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:27.65] INFO: non_mandatory_tour_destination.escort.logsums.compute_logsums Running adaptive_chunked_choosers with 41 choosers
[00:27.65] INFO: Running chunk 1 of 1 with 41 of 41 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:28.00] INFO: Running tour_destination_simulate with 3 persons
[00:28.00] INFO: non_mandatory_tour_destination.escort.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 3 choosers and 41 alternatives
[00:28.00] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:28.00] INFO: Running eval_interaction_utilities on 41 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
Time to execute step 'non_mandatory_tour_destination': 4.09 s
Total time to execute iteration 1 with iteration value None: 4.09 s
[00:28.09] INFO: #run_model running step non_mandatory_tour_scheduling
Running step 'non_mandatory_tour_scheduling'
[00:28.11] DEBUG: @inject timetable
[00:28.15] INFO: Running non_mandatory_tour_scheduling with 95 tours
[00:28.15] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1 schedule_tours running 65 tour choices
[00:28.15] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1 Running adaptive_chunked_choosers with 65 choosers
[00:28.15] INFO: Running chunk 1 of 1 with 65 of 65 choosers
[00:28.15] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1 schedule_tours running 65 tour choices
[00:28.16] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_1.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 65 choosers and 9990 alternatives
[00:28.16] INFO: Running chunk 1 of 1 with 65 of 65 choosers
[00:28.17] INFO: Running eval_interaction_utilities on 9990 rows
[00:28.45] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2 schedule_tours running 23 tour choices
[00:28.45] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2 Running adaptive_chunked_choosers with 23 choosers
[00:28.45] INFO: Running chunk 1 of 1 with 23 of 23 choosers
[00:28.45] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2 schedule_tours running 23 tour choices
[00:28.46] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_2.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 23 choosers and 1758 alternatives
[00:28.46] INFO: Running chunk 1 of 1 with 23 of 23 choosers
[00:28.46] INFO: Running eval_interaction_utilities on 1758 rows
[00:28.62] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_3 schedule_tours running 6 tour choices
[00:28.62] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_3 Running adaptive_chunked_choosers with 6 choosers
[00:28.62] INFO: Running chunk 1 of 1 with 6 of 6 choosers
[00:28.62] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_3 schedule_tours running 6 tour choices
[00:28.63] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_3.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 6 choosers and 200 alternatives
[00:28.63] INFO: Running chunk 1 of 1 with 6 of 6 choosers
[00:28.63] INFO: Running eval_interaction_utilities on 200 rows
[00:28.79] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_4 schedule_tours running 1 tour choices
[00:28.79] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_4 Running adaptive_chunked_choosers with 1 choosers
[00:28.79] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:28.79] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_4 schedule_tours running 1 tour choices
[00:28.80] INFO: non_mandatory_tour_scheduling.vectorize_tour_scheduling.tour_4.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 18 alternatives
[00:28.80] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:28.80] INFO: Running eval_interaction_utilities on 18 rows
Time to execute step 'non_mandatory_tour_scheduling': 0.89 s
Total time to execute iteration 1 with iteration value None: 0.89 s
[00:29.06] INFO: #run_model running step tour_mode_choice_simulate
Running step 'tour_mode_choice_simulate'
[00:29.09] INFO: Running tour_mode_choice with 193 tours
[00:29.09] INFO: tour_types top 10 value counts:
work        69
shopping    31
school      26
othdiscr    21
othmaint    20
eatout      16
social       7
escort       3
Name: tour_type, dtype: int64
[00:29.10] INFO: tour_mode_choice_simulate tour_type 'eatout' (16 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:29.31] INFO: tour_mode_choice.eatout.simple_simulate Running adaptive_chunked_choosers with 16 choosers
[00:29.31] INFO: Running chunk 1 of 1 with 16 of 16 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:29.83] INFO: tour_mode_choice_simulate eatout choices_df top 10 value counts:
WALK              12
WALK_LRF           3
DRIVEALONEFREE     1
Name: tour_mode, dtype: int64
[00:29.83] INFO: tour_mode_choice_simulate tour_type 'escort' (3 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:29.99] INFO: tour_mode_choice.escort.simple_simulate Running adaptive_chunked_choosers with 3 choosers
[00:29.99] INFO: Running chunk 1 of 1 with 3 of 3 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:30.43] INFO: tour_mode_choice_simulate escort choices_df top 10 value counts:
SHARED2FREE    1
TNC_SINGLE     1
WALK_LOC       1
Name: tour_mode, dtype: int64
[00:30.43] INFO: tour_mode_choice_simulate tour_type 'othdiscr' (21 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:30.60] INFO: tour_mode_choice.othdiscr.simple_simulate Running adaptive_chunked_choosers with 21 choosers
[00:30.60] INFO: Running chunk 1 of 1 with 21 of 21 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:31.04] INFO: tour_mode_choice_simulate othdiscr choices_df top 10 value counts:
WALK              10
WALK_LRF           3
DRIVEALONEFREE     2
TNC_SINGLE         2
WALK_LOC           2
WALK_HVY           1
SHARED3FREE        1
Name: tour_mode, dtype: int64
[00:31.04] INFO: tour_mode_choice_simulate tour_type 'othmaint' (20 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:31.21] INFO: tour_mode_choice.othmaint.simple_simulate Running adaptive_chunked_choosers with 20 choosers
[00:31.21] INFO: Running chunk 1 of 1 with 20 of 20 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:31.64] INFO: tour_mode_choice_simulate othmaint choices_df top 10 value counts:
WALK              9
BIKE              4
TNC_SINGLE        4
WALK_LOC          1
TAXI              1
DRIVEALONEFREE    1
Name: tour_mode, dtype: int64
[00:31.64] INFO: tour_mode_choice_simulate tour_type 'school' (17 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:31.81] INFO: tour_mode_choice.school.simple_simulate Running adaptive_chunked_choosers with 17 choosers
[00:31.81] INFO: Running chunk 1 of 1 with 17 of 17 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:32.25] INFO: tour_mode_choice_simulate school choices_df top 10 value counts:
WALK           7
WALK_LOC       5
WALK_LRF       3
WALK_HVY       1
SHARED3FREE    1
Name: tour_mode, dtype: int64
[00:32.25] INFO: tour_mode_choice_simulate tour_type 'shopping' (31 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:32.42] INFO: tour_mode_choice.shopping.simple_simulate Running adaptive_chunked_choosers with 31 choosers
[00:32.42] INFO: Running chunk 1 of 1 with 31 of 31 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:32.86] INFO: tour_mode_choice_simulate shopping choices_df top 10 value counts:
WALK              13
WALK_LRF           7
WALK_LOC           5
TAXI               2
DRIVEALONEFREE     2
SHARED2FREE        1
TNC_SINGLE         1
Name: tour_mode, dtype: int64
[00:32.86] INFO: tour_mode_choice_simulate tour_type 'social' (7 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:33.06] INFO: tour_mode_choice.social.simple_simulate Running adaptive_chunked_choosers with 7 choosers
[00:33.06] INFO: Running chunk 1 of 1 with 7 of 7 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:33.52] INFO: tour_mode_choice_simulate social choices_df top 10 value counts:
WALK          2
WALK_LRF      2
TAXI          1
TNC_SINGLE    1
WALK_LOC      1
Name: tour_mode, dtype: int64
[00:33.52] INFO: tour_mode_choice_simulate tour_type 'univ' (9 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:33.69] INFO: tour_mode_choice.univ.simple_simulate Running adaptive_chunked_choosers with 9 choosers
[00:33.69] INFO: Running chunk 1 of 1 with 9 of 9 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:34.13] INFO: tour_mode_choice_simulate univ choices_df top 10 value counts:
WALK              3
WALK_LRF          2
TNC_SHARED        1
DRIVEALONEFREE    1
TAXI              1
WALK_LOC          1
Name: tour_mode, dtype: int64
[00:34.13] INFO: tour_mode_choice_simulate tour_type 'work' (69 tours)
get item DIST
get item DISTWALK
get item DISTBIKE
[00:34.31] INFO: tour_mode_choice.work.simple_simulate Running adaptive_chunked_choosers with 69 choosers
[00:34.31] INFO: Running chunk 1 of 1 with 69 of 69 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:34.88] INFO: tour_mode_choice_simulate work choices_df top 10 value counts:
WALK              24
WALK_LOC          14
WALK_LRF          12
TNC_SINGLE         7
BIKE               4
DRIVEALONEFREE     3
WALK_HVY           3
SHARED3FREE        1
SHARED2FREE        1
Name: tour_mode, dtype: int64
[00:34.88] INFO: tour_mode_choice_simulate all tour type choices top 10 value counts:
WALK              80
WALK_LRF          32
WALK_LOC          30
TNC_SINGLE        16
DRIVEALONEFREE    10
BIKE               8
WALK_HVY           5
TAXI               5
SHARED2FREE        3
SHARED3FREE        3
Name: tour_mode, dtype: int64
Time to execute step 'tour_mode_choice_simulate': 5.82 s
Total time to execute iteration 1 with iteration value None: 5.82 s
[00:34.96] INFO: #run_model running step atwork_subtour_frequency
Running step 'atwork_subtour_frequency'
[00:34.98] INFO: Running atwork_subtour_frequency with 69 work tours
[00:35.01] INFO: atwork_subtour_frequency.simple_simulate Running adaptive_chunked_choosers with 69 choosers
[00:35.02] INFO: Running chunk 1 of 1 with 69 of 69 choosers
before eval utilities                                                                                                       no_subtours     eat  business1   maint  business2  eat_business
Expression                                        Label                                                                                                              
pemploy==1                                        util_dummy_for_full_time_worker                            -0.6 -7.2800    -7.3750 -8.0930   -14.2800      -14.7900
pemploy!=1                                        util_dummy_for_non_full_time_worker                        -0.6 -8.6040    -8.3190 -8.2140   -14.2800      -14.7900
ptype in [4, 5]                                   util_dummy_for_non_workers                                  0.0  0.0000    -5.0000 -5.0000    -5.0000       -5.0000
income_segment == 2                               util_medium_hh_income_dummy                                 0.0  0.6100     0.5555  0.1527     1.1110        1.1655
(income_segment > 2) & (income_segment < 5)       util_high_hh_income_dummy                                   0.0  0.8693     1.0660  0.1651     2.1320        1.9353
 auto_ownership == 0                              util_zero_cars_owned_by_hh_dummy                            0.0  0.0000    -0.3391  0.1762     0.0000       -0.3391
@(df.pemploy==1)*df.num_discr_tours               util_individual_discretionary_tours_made_by_ful...          0.0  0.2334     0.7045  0.5061     1.4090        0.9379
@(df.pemploy==2)*df.num_discr_tours               util_individual_discretionary_tours_made_by_par...          0.0  0.6776     0.7045  0.5061     1.4090        1.3821
num_eatout_tours                                  util_individual_eating_out_tours_made_by_person             0.0  0.5491     0.5434  0.9166     1.0868        1.0925
@(df.pemploy==1)*df.num_maint_shop_escort         util_main_shop_escort_tours_allocated_to_full_t...          0.0  0.0520    -0.1903  0.1446    -0.3806       -0.2423
@(df.pemploy==2)*df.num_maint_shop_escort         util_main_shop_escort_tours_allocated_to_part_t...          0.0 -0.3099    -0.1903 -0.2723    -0.3806       -0.5002
num_joint_maint_shop_eat                          util_participation_in_joint_shop_main_eat_tours             0.0  0.2458     0.0830  0.0803     0.1660        0.3288
num_joint_discr                                   util_participation_in_joint_discretionary_tours             0.0  0.3588    -0.2637  0.5822    -0.5274        0.0951
@np.log(df.duration+0.5)                          util_log_of_the_work_tour_duration                          0.0  1.5500     1.1420  1.6590     2.2840        2.6920
work_tour_is_SOV                                  util_dummy_for_drive_alone_mode_for_work_tour               0.0  0.4804     0.9901  1.1530     1.9802        1.4705
num_work_tours==2                                 util_two_work_tours_by_person                               0.0 -0.9862     0.3753 -0.2312     0.7506       -0.6109
work_zone_area_type<4                             util_workplace_urban_area_dummy                             0.0 -0.4182    -0.2235 -0.1479    -0.4470       -0.6417
(work_zone_area_type>3) & (work_zone_area_type<6) util_workplace_suburban_area_dummy                          0.0 -0.2916    -0.1102  0.0000    -0.2204       -0.4018
auOpRetail                                        util_auto_accessibility_to_retail_for_work_taz              0.0  0.0150     0.0534  0.0265     0.1067        0.0683
nmRetail                                          util_walk_accessibility_to_retail_for_work_taz              0.0  0.0600     0.0000  0.0400     0.0000        0.0600
(is_worker | is_student) * num_non_mand           util_dummy_for_worker_or_student_with_non_manda...          0.0  0.0000     0.0000 -0.3573     0.0000        0.0000
1                                                 util_at_work_sub_tour_alternative_specific_cons...          0.0  0.8576    -0.5372 -0.6198    -2.1337       -0.9721            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  is_cbd num_maint_shop_escort  num_joint_discr  num_joint_maint_shop_eat  work_tour_is_SOV
tour_id                                                                   ...                                                                                           
10828426      264107      work                1              1         1  ...   False                     0                0                         0             False
10834207      264248      work                1              1         1  ...   False                     0                0                         0             False
13271288      323689      work                1              1         1  ...   False                     0                0                         0             False
13286130      324051      work                1              1         1  ...   False                     0                0                         0             False
13286171      324052      work                1              1         1  ...   False                     0                0                         0             False
...              ...       ...              ...            ...       ...  ...     ...                   ...              ...                       ...               ...
308016475    7512596      work                1              1         1  ...   False                     0                0                         0             False
308050751    7513432      work                1              1         1  ...   False                     0                0                         0             False
308055425    7513546      work                1              1         1  ...   False                     0                0                         0             False
308055753    7513554      work                1              1         1  ...   False                     0                0                         0             False
308056204    7513565      work                1              1         1  ...   False                     0                0                         0             False

[69 rows x 157 columns] {}
before logit.utils_to_probs 
            no_subtours       eat  business1     maint  business2  eat_business
tour_id                                                                       
10828426          -0.6 -3.667023  -6.043991 -4.075314 -10.111999     -8.871431
10834207          -0.6 -2.333940  -5.100165 -3.948313 -10.112348     -8.862523
13271288          -0.6 -4.449720  -6.627528 -4.916772 -11.279074    -10.237666
13286130          -0.6 -2.291235  -4.754172 -4.093913 -10.098574     -8.473838
13286171          -0.6 -2.561483  -4.953283 -4.383166 -10.496798     -8.943197
...                ...       ...        ...       ...        ...           ...
308016475         -0.6 -3.761113  -6.135042 -4.188921 -10.294110     -9.056581
308050751         -0.6 -3.902120  -5.899832 -4.516044 -10.501889     -8.962377
308055425         -0.6 -4.423652  -6.623183 -4.898051 -11.270392    -10.207261
308055753         -0.6 -2.437113  -5.191042 -4.067922 -10.294110     -9.056581
308056204         -0.6 -4.057249  -6.353227 -4.505882 -10.730480     -9.570902

[69 rows x 6 columns]
[00:35.23] WARNING: register tours: no rows with household_id in [982875].
[00:35.23] INFO: atwork_subtour_frequency top 10 value counts:
               133
no_subtours     60
eat              8
maint            1
Name: atwork_subtour_frequency, dtype: int64
Time to execute step 'atwork_subtour_frequency': 0.28 s
Total time to execute iteration 1 with iteration value None: 0.28 s
[00:35.31] INFO: #run_model running step atwork_subtour_destination
Running step 'atwork_subtour_destination'
[00:35.33] INFO: running atwork_subtour_destination.atwork.sample with 9 tours
[00:35.33] INFO: atwork_subtour_destination.atwork.sample.interaction_sample Running adaptive_chunked_choosers with 9 choosers
[00:35.33] INFO: Running chunk 1 of 1 with 9 of 9 choosers
[00:35.33] INFO: Running eval_interaction_utilities on 225 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:35.37] INFO: Running atwork_subtour_destination.atwork.logsums with 124 rows
get item DIST
get item DISTWALK
get item DISTBIKE
[00:35.53] INFO: atwork_subtour_destination.atwork.logsums.compute_logsums Running adaptive_chunked_choosers with 124 choosers
[00:35.53] INFO: Running chunk 1 of 1 with 124 of 124 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:35.92] INFO: Running tour_destination_simulate with 9 persons
[00:35.92] INFO: atwork_subtour_destination.atwork.simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 9 choosers and 124 alternatives
[00:35.92] INFO: Running chunk 1 of 1 with 9 of 9 choosers
[00:35.92] INFO: Running eval_interaction_utilities on 124 rows
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:35.97] INFO: destination summary:
count     9.000000
mean     10.444444
std       7.333333
min       2.000000
25%       7.000000
50%       8.000000
75%      15.000000
max      25.000000
Name: destination, dtype: float64
Time to execute step 'atwork_subtour_destination': 0.67 s
Total time to execute iteration 1 with iteration value None: 0.67 s
[00:36.05] INFO: #run_model running step atwork_subtour_scheduling
Running step 'atwork_subtour_scheduling'
[00:36.07] INFO: Running atwork_subtour_scheduling with 9 tours
get item ('SOV_TIME', 'MD')
[00:36.11] INFO: atwork_subtour_scheduling.tour_1 schedule_tours running 9 tour choices
[00:36.11] INFO: atwork_subtour_scheduling.tour_1 Running adaptive_chunked_choosers with 9 choosers
[00:36.11] INFO: Running chunk 1 of 1 with 9 of 9 choosers
[00:36.11] INFO: atwork_subtour_scheduling.tour_1 schedule_tours running 9 tour choices
[00:36.12] INFO: atwork_subtour_scheduling.tour_1.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 9 choosers and 651 alternatives
[00:36.12] INFO: Running chunk 1 of 1 with 9 of 9 choosers
[00:36.12] INFO: Running eval_interaction_utilities on 651 rows
Time to execute step 'atwork_subtour_scheduling': 0.23 s
Total time to execute iteration 1 with iteration value None: 0.23 s
[00:36.35] INFO: #run_model running step atwork_subtour_mode_choice
Running step 'atwork_subtour_mode_choice'
[00:36.38] INFO: Running atwork_subtour_mode_choice with 9 subtours
[00:36.38] INFO: atwork_subtour_mode_choice tour_type top 10 value counts:
eat      8
maint    1
Name: tour_type, dtype: int64
get item DIST
get item DISTWALK
get item DISTBIKE
[00:36.54] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.54] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.54] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.54] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.54] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.54] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.54] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[00:36.55] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/abm/models/util/mode.py:137: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  choosers[trace_column_names] = choosers.index

[00:36.55] INFO: atwork_subtour_mode_choice.simple_simulate Running adaptive_chunked_choosers with 9 choosers
[00:36.55] INFO: Running chunk 1 of 1 with 9 of 9 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
[00:36.99] INFO: atwork_subtour_mode_choice choices top 10 value counts:
WALK    9
Name: tour_mode, dtype: int64
Time to execute step 'atwork_subtour_mode_choice': 0.64 s
Total time to execute iteration 1 with iteration value None: 0.64 s
[00:37.06] INFO: #run_model running step stop_frequency
Running step 'stop_frequency'
get item DIST
[00:37.18] INFO: stop_frequency segments top 10 value counts:
work        69
shopping    31
othdiscr    21
othmaint    20
school      17
eatout      16
atwork       9
univ         9
social       7
escort       3
Name: primary_purpose, dtype: int64
[00:37.18] INFO: stop_frequency running segment work with 69 chooser rows
[00:37.19] INFO: stop_frequency.work.simple_simulate Running adaptive_chunked_choosers with 69 choosers
[00:37.20] INFO: Running chunk 1 of 1 with 69 of 69 choosers
before eval utilities                                                                                                        0out_0in  0out_1in  0out_2in  ...  3out_1in  3out_2in  3out_3in
Expression                                         Label                                                                             ...                              
(income_in_thousands>19999) & (income_in_thousa... util_middle_to_low_income_hh                             0.0     0.170     0.170  ...     0.170     0.170     0.170
(income_in_thousands>=50000) & (income_in_thous... util_mid_to_high_income_hh                               0.0     0.230     0.230  ...     0.230     0.230     0.230
(income_in_thousands>=100000)                      util_high_income_hh                                      0.0     0.240     0.240  ...     0.240     0.240     0.240
hhsize                                             util_number_of_hh_persons                                0.0    -0.310    -0.310  ...    -0.310    -0.310    -0.310
num_student                                        util_number_of_students_in_hh                            0.0     0.210     0.210  ...     0.210     0.210     0.210
(num_age_0_4 > 0)                                  util_presence_of_kids_between_0_and_4_including...       0.0     0.740     0.740  ...     0.740     0.740     0.740
num_age_5_15                                       util_num_kids_between_5_and_15_including_years_old       0.0     0.080     0.080  ...     0.080     0.080     0.080
(num_age_5_15 > 0)                                 util_presence_of_kids_between_5_and_15_includin...       0.0     0.260     0.260  ...     0.260     0.260     0.260
num_adult                                          util_number_of_adults_16_years_old_                      0.0     0.030     0.030  ...     0.030     0.030     0.030
more_cars_than_workers                             util_number_of_cars_number_of_workers                    0.0     0.160     0.160  ...     0.160     0.160     0.160
female                                             util_dummy_for_female                                    0.0     0.220     0.220  ...     0.220     0.220     0.220
tour_mode_is_transit                               util_dummy_for_all_stops_made_by_transit                 0.0    -0.700    -0.700  ...    -0.700    -0.700    -0.700
tour_mode_is_non_motorized                         util_dummy_for_walking_to_all_stops                      0.0    -1.540    -1.540  ...    -1.540    -1.540    -1.540
num_work_tours                                     util_number_of_work_tours_undertaken_by_the_person       0.0    -0.150    -0.150  ...    -0.150    -0.150    -0.150
num_univ_tours                                     util_number_of_university_tours_tours_undertake...       0.0    -0.480    -0.480  ...    -0.480    -0.480    -0.480
num_school_tours                                   util_number_of_school_tours_tours_undertaken_by...       0.0    -1.550    -1.550  ...    -1.550    -1.550    -1.550
num_escort_tours                                   util_number_of_escort_tours_tours_undertaken_by...       0.0     0.200     0.200  ...     0.200     0.200     0.200
num_hh_shop_tours                                  util_number_of_shop_tours_undertaken_by_the_hou...       0.0    -0.050    -0.050  ...    -0.050    -0.050    -0.050
(start>5) & (start<8)                              util_am_peak_departure_between_6am_and_7_am_inc...       0.0    -1.930    -1.930  ...    -1.930    -1.930    -1.930
(end > 18)                                         util_evening_arrival_19_00_interacted_with_retu...       0.0     0.310     0.310  ...     0.310     0.310     0.310
(duration > 10)                                    util_dummy_for_the_duration_of_the_tour_being_e...       0.0     0.600     0.600  ...     0.600     0.600     0.600
(distance_in_miles < 20)                           util_dummy_for_distance_less_than_20_miles               0.0    -0.220    -0.220  ...    -0.220    -0.220    -0.220
distance_in_miles                                  util_dummy_for_distance_in_miles                         0.0     0.010     0.010  ...     0.010     0.010     0.010
tour_mode_is_drive_transit                         util_no_stops_if_tour_mode_is_drivetransit               0.0  -999.000  -999.000  ...  -999.000  -999.000  -999.000
1                                                  util_alternative_specific_constant_for_outbound...       0.0     0.000     0.000  ...    -3.934    -3.934    -3.934
1 # (2)                                            util_alternative_specific_constant_for_return_s...       0.0    -0.445    -1.775  ...    -0.445    -1.775    -2.139
1 # (3)                                            util_alternative_specific_constant_for_the_tota...       0.0     0.000     0.000  ...     0.000     0.695     0.695
num_atwork_subtours                                util_number_of_subtours_in_the_tour                      0.0     0.190     0.190  ...     0.190     0.190     0.190

[28 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
10828426      264107      work                1              1         1  ...                  0                  0  7.188751  7.322742                      0
10834207      264248      work                1              1         1  ...                  0                  0  7.379336  7.150924                      0
13271288      323689      work                1              1         1  ...                  0                  0  7.306522  7.693086                      0
13286130      324051      work                1              1         1  ...                  0                  0  8.247303  8.422746                      0
13286171      324052      work                1              1         1  ...                  0                  0  7.016154  7.511301                      0
...              ...       ...              ...            ...       ...  ...                ...                ...       ...       ...                    ...
308016475    7512596      work                1              1         1  ...                  0                  0  7.981950  7.415630                      0
308050751    7513432      work                1              1         1  ...                  0                  0  7.981950  8.016915                      0
308055425    7513546      work                1              1         1  ...                  0                  0  7.546934  7.511301                      0
308055753    7513554      work                1              1         1  ...                  0                  0  7.981950  7.415630                      0
308056204    7513565      work                1              1         1  ...                  0                  0  7.981950  7.616518                      0

[69 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in  3out_3in
tour_id                                                                                                                                                                  
10828426        0.0   -3.0783   -4.4083   -4.7723   -3.4663   -3.9113   -5.2413   -5.6053   -5.2463   -5.6913   -7.0213   -6.6903   -6.5673   -7.0123   -7.6473   -8.0113
10834207        0.0   -3.0755   -4.4055   -4.7695   -3.4635   -3.9085   -5.2385   -5.6025   -5.2435   -5.6885   -7.0185   -6.6875   -6.5645   -7.0095   -7.6445   -8.0085
13271288        0.0   -1.8393   -3.1693   -3.5333   -2.2273   -2.6723   -4.0023   -4.3663   -4.0073   -4.4523   -5.7823   -5.4513   -5.3283   -5.7733   -6.4083   -6.7723
13286130        0.0   -4.2404   -5.5704   -5.9344   -4.6284   -5.0734   -6.4034   -6.7674   -6.4084   -6.8534   -8.1834   -7.8524   -7.7294   -8.1744   -8.8094   -9.1734
13286171        0.0   -3.5884   -4.9184   -5.2824   -3.9764   -4.4214   -5.7514   -6.1154   -5.7564   -6.2014   -7.5314   -7.2004   -7.0774   -7.5224   -8.1574   -8.5214
...             ...       ...       ...       ...       ...       ...       ...       ...       ...       ...       ...       ...       ...       ...       ...       ...
308016475       0.0   -3.7397   -5.0697   -5.4337   -4.1277   -4.5727   -5.9027   -6.2667   -5.9077   -6.3527   -7.6827   -7.3517   -7.2287   -7.6737   -8.3087   -8.6727
308050751       0.0   -2.2525   -3.5825   -3.9465   -2.6405   -3.0855   -4.4155   -4.7795   -4.4205   -4.8655   -6.1955   -5.8645   -5.7415   -6.1865   -6.8215   -7.1855
308055425       0.0   -3.7151   -5.0451   -5.4091   -4.1031   -4.5481   -5.8781   -6.2421   -5.8831   -6.3281   -7.6581   -7.3271   -7.2041   -7.6491   -8.2841   -8.6481
308055753       0.0   -1.8097   -3.1397   -3.5037   -2.1977   -2.6427   -3.9727   -4.3367   -3.9777   -4.4227   -5.7527   -5.4217   -5.2987   -5.7437   -6.3787   -6.7427
308056204       0.0   -2.0336   -3.3636   -3.7276   -2.4216   -2.8666   -4.1966   -4.5606   -4.2016   -4.6466   -5.9766   -5.6456   -5.5226   -5.9676   -6.6026   -6.9666

[69 rows x 16 columns]
[00:37.46] INFO: stop_frequency running segment school with 17 chooser rows
[00:37.48] INFO: stop_frequency.school.simple_simulate Running adaptive_chunked_choosers with 17 choosers
[00:37.48] INFO: Running chunk 1 of 1 with 17 of 17 choosers
before eval utilities                                                                                0out_0in  0out_1in  0out_2in  0out_3in  ...  3out_0in  3out_1in  3out_2in  3out_3in
Expression                 Label                                                                                       ...                                        
hhsize                     util_number_of_hh_persons                                0.0   -0.5060   -0.5060   -0.5060  ...   -0.5060   -0.5060   -0.5060   -0.5060
(num_age_5_15 > 0)         util_presence_of_kids_between_5_and_15_includin...       0.0    0.3299    0.3299    0.3299  ...    0.3299    0.3299    0.3299    0.3299
more_cars_than_workers     util_number_of_cars_number_of_workers                    0.0    0.5331    0.5331    0.5331  ...    0.5331    0.5331    0.5331    0.5331
female                     util_dummy_for_female                                    0.0    0.4099    0.4099    0.4099  ...    0.4099    0.4099    0.4099    0.4099
tour_mode_is_transit       util_dummy_for_all_stops_made_by_transit                 0.0   -0.7000   -0.7000   -0.7000  ...   -0.7000   -0.7000   -0.7000   -0.7000
tour_mode_is_non_motorized util_dummy_for_walking_to_all_stops                      0.0   -1.8163   -1.8163   -1.8163  ...   -1.8163   -1.8163   -1.8163   -1.8163
num_escort_tours           util_number_of_escort_tours_tours_undertaken_by...       0.0    1.2365    1.2365    1.2365  ...    1.2365    1.2365    1.2365    1.2365
(end > 16)                 util_arrival_later_than_17_00_                           0.0    1.8377    1.8377    1.8377  ...    1.8377    1.8377    1.8377    1.8377
(duration > 8)             util_dummy_for_the_duration_of_the_tour_being_e...       0.0    0.9549    0.9549    0.9549  ...    0.9549    0.9549    0.9549    0.9549
distance_in_miles          util_dummy_for_distance_in_miles                         0.0    0.0438    0.0438    0.0438  ...    0.0438    0.0438    0.0438    0.0438
tour_mode_is_drive_transit util_no_stops_if_tour_mode_is_drivetransit               0.0 -999.0000 -999.0000 -999.0000  ... -999.0000 -999.0000 -999.0000 -999.0000
1                          util_alternative_specific_constant_for_outbound...       0.0    0.0000    0.0000    0.0000  ...   -5.8500   -5.8500   -5.8500   -5.8500
1 # (2)                    util_alternative_specific_constant_for_return_s...       0.0   -1.2060   -2.6720   -3.3640  ...    0.0000   -1.2060   -2.6720   -3.3640
1 # (3)                    util_alternative_specific_constant_for_the_tota...       0.0    0.0000    0.0000    0.0000  ...    0.0000    0.7010    1.1350    1.1350

[14 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
15826359      386008    school                1              1         1  ...                  0                  0  7.415630  7.415630                      0
15828573      386062    school                1              1         1  ...                  0                  0  7.188751  7.767894                      0
17156153      418442    school                1              1         1  ...                  0                  0  0.000000  0.000000                      0
24423157      595686    school                1              1         1  ...                  0                  0  7.428997  7.546934                      0
26416003      644292    school                1              1         1  ...                  0                  0  7.607878  7.149057                      0
26423629      644478    school                1              1         1  ...                  0                  0  6.945206  6.590820                      1
80305829     1958678    school                1              1         1  ...                  0                  0  7.188751  7.296646                      0
88521450     2159059    school                1              1         1  ...                  0                  0  7.494197  7.494197                      1
91019949     2219998    school                1              1         1  ...                  0                  0  7.016154  7.546934                      0
100798613    2458502    school                1              1         1  ...                  0                  0  7.981950  7.981950                      0
100798654    2458503    school                1              1         1  ...                  0                  0  7.981950  7.981950                      0
105234731    2566700    school                1              1         1  ...                  0                  0  7.616518  7.616518                      0
105234772    2566701    school                1              1         1  ...                  0                  0  7.616518  8.050369                      0
105234813    2566702    school                1              1         1  ...                  0                  0  7.180366  7.068341                      0
117968798    2877287    school                1              1         1  ...                  0                  0  7.567826  7.567826                      0
159535295    3891104    school                1              1         1  ...                  0                  0  7.016154  6.661200                      1
171036451    4171620    school                1              1         1  ...                  0                  0  7.016154  7.546934                      0

[17 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  ...  2out_1in   2out_2in   2out_3in   3out_0in   3out_1in   3out_2in   3out_3in
tour_id                                                                          ...                                                                            
15826359        0.0 -2.753954 -4.219954 -4.911954 -3.670954 -4.876954 -6.342954  ... -6.551954  -7.316954  -7.574954  -7.397954  -7.902954  -8.934954  -9.626954
15828573        0.0 -2.326208 -3.792208 -4.484208 -3.243208 -4.449208 -5.915208  ... -6.124208  -6.889208  -7.147208  -6.970208  -7.475208  -8.507208  -9.199208
17156153        0.0 -1.675702 -3.141702 -3.833702 -2.592702 -3.798702 -5.264702  ... -5.473702  -6.238702  -6.496702  -6.319702  -6.824702  -7.856702  -8.548702
24423157        0.0 -1.698202 -3.164202 -3.856202 -2.615202 -3.821202 -5.287202  ... -5.496202  -6.261202  -6.519202  -6.342202  -6.847202  -7.879202  -8.571202
26416003        0.0 -1.218732 -2.684732 -3.376732 -2.135732 -3.341732 -4.807732  ... -5.016732  -5.781732  -6.039732  -5.862732  -6.367732  -7.399732  -8.091732
26423629        0.0 -1.191138 -2.657138 -3.349138 -2.108138 -3.314138 -4.780138  ... -4.989138  -5.754138  -6.012138  -5.835138  -6.340138  -7.372138  -8.064138
80305829        0.0 -2.315258 -3.781258 -4.473258 -3.232258 -4.438258 -5.904258  ... -6.113258  -6.878258  -7.136258  -6.959258  -7.464258  -8.496258  -9.188258
88521450        0.0 -3.997126 -5.463126 -6.155126 -4.914126 -6.120126 -7.586126  ... -7.795126  -8.560126  -8.818126  -8.641126  -9.146126 -10.178126 -10.870126
91019949        0.0 -3.033218 -4.499218 -5.191218 -3.950218 -5.156218 -6.622218  ... -6.831218  -7.596218  -7.854218  -7.677218  -8.182218  -9.214218  -9.906218
100798613       0.0 -0.975544 -2.441544 -3.133544 -1.892544 -3.098544 -4.564544  ... -4.773544  -5.538544  -5.796544  -5.619544  -6.124544  -7.156544  -7.848544
100798654       0.0 -4.178044 -5.644044 -6.336044 -5.095044 -6.301044 -7.767044  ... -7.976044  -8.741044  -8.999044  -8.822044  -9.327044 -10.359044 -11.051044
105234731       0.0 -5.216706 -6.682706 -7.374706 -6.133706 -7.339706 -8.805706  ... -9.014706  -9.779706 -10.037706  -9.860706 -10.365706 -11.397706 -12.089706
105234772       0.0 -4.794542 -6.260542 -6.952542 -5.711542 -6.917542 -8.383542  ... -8.592542  -9.357542  -9.615542  -9.438542  -9.943542 -10.975542 -11.667542
105234813       0.0 -2.234236 -3.700236 -4.392236 -3.151236 -4.357236 -5.823236  ... -6.032236  -6.797236  -7.055236  -6.878236  -7.383236  -8.415236  -9.107236
117968798       0.0 -5.309302 -6.775302 -7.467302 -6.226302 -7.432302 -8.898302  ... -9.107302  -9.872302 -10.130302  -9.953302 -10.458302 -11.490302 -12.182302
159535295       0.0 -3.070010 -4.536010 -5.228010 -3.987010 -5.193010 -6.659010  ... -6.868010  -7.633010  -7.891010  -7.714010  -8.219010  -9.251010  -9.943010
171036451       0.0 -5.989218 -7.455218 -8.147218 -6.906218 -8.112218 -9.578218  ... -9.787218 -10.552218 -10.810218 -10.633218 -11.138218 -12.170218 -12.862218

[17 rows x 16 columns]
[00:37.68] INFO: stop_frequency running segment univ with 9 chooser rows
[00:37.70] INFO: stop_frequency.univ.simple_simulate Running adaptive_chunked_choosers with 9 choosers
[00:37.70] INFO: Running chunk 1 of 1 with 9 of 9 choosers
before eval utilities                                                                                0out_0in  0out_1in  0out_2in  0out_3in  ...  3out_0in  3out_1in  3out_2in  3out_3in
Expression                 Label                                                                                       ...                                        
hhsize                     util_number_of_hh_persons                                0.0   -0.2827   -0.2827   -0.2827  ...   -0.2827   -0.2827   -0.2827   -0.2827
(num_age_5_15 > 0)         util_presence_of_kids_between_5_and_15_includin...       0.0    0.6823    0.6823    0.6823  ...    0.6823    0.6823    0.6823    0.6823
auto_ownership             util_number_of_vehicles                                  0.0    0.1703    0.1703    0.1703  ...    0.1703    0.1703    0.1703    0.1703
female                     util_dummy_for_female                                    0.0    0.7349    0.7349    0.7349  ...    0.7349    0.7349    0.7349    0.7349
tour_mode_is_transit       util_dummy_for_all_stops_made_by_transit                 0.0   -0.7000   -0.7000   -0.7000  ...   -0.7000   -0.7000   -0.7000   -0.7000
num_escort_tours           util_number_of_escort_tours_tours_undertaken_by...       0.0    0.9018    0.9018    0.9018  ...    0.9018    0.9018    0.9018    0.9018
(end > 16)                 util_arrival_later_than_17_00_                           0.0    0.3890    0.3890    0.3890  ...    0.3890    0.3890    0.3890    0.3890
(duration > 8)             util_dummy_for_the_duration_of_the_tour_being_e...       0.0    0.8434    0.8434    0.8434  ...    0.8434    0.8434    0.8434    0.8434
hhacc # (2)                util_hh_accesibility_for_inbound_tours_interaction       0.0    0.2481    0.2481    0.2481  ...    0.2481    0.2481    0.2481    0.2481
tour_mode_is_drive_transit util_no_stops_if_tour_mode_is_drivetransit               0.0 -999.0000 -999.0000 -999.0000  ... -999.0000 -999.0000 -999.0000 -999.0000
1                          util_alternative_specific_constant_for_outbound...       0.0    0.0000    0.0000    0.0000  ...   -4.9810   -4.9810   -4.9810   -4.9810
1 # (2)                    util_alternative_specific_constant_for_return_s...       0.0   -2.0030   -3.5100   -3.6770  ...    0.0000   -2.0030   -3.5100   -3.6770
1 # (3)                    util_alternative_specific_constant_for_the_tota...       0.0    0.0000    0.0000    0.0000  ...    0.0000    1.2720    1.8710    1.8710

[13 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
13350574      325623    school                1              1         2  ...                  0                  0  0.000000  0.000000                      0
72744896     1774265    school                1              1         1  ...                  0                  0  0.000000  0.000000                      0
76918819     1876068    school                1              1         2  ...                  0                  0  6.661200  7.767894                      0
88521409     2159058    school                1              1         1  ...                  0                  0  0.000000  0.000000                      0
117968675    2877284    school                1              1         2  ...                  0                  0  7.567826  7.567826                      0
130727875    3188484    school                1              1         2  ...                  0                  0  7.616518  8.422746                      0
171036246    4171615    school                1              1         1  ...                  0                  0  8.247303  8.420518                      0
309118137    7539466    school                1              1         1  ...                  0                  0  7.340975  7.945965                      0
309128059    7539708    school                1              1         1  ...                  0                  0  7.085930  7.767894                      0

[9 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in  3out_3in
tour_id                                                                                                                                                                  
13350574        0.0 -1.274200 -2.781200 -2.948200 -1.899200 -3.902200 -5.409200 -4.304200 -3.012200 -5.015200 -5.250200 -4.818200 -4.252200 -4.983200 -5.891200 -6.058200
72744896        0.0 -1.274200 -2.781200 -2.948200 -1.899200 -3.902200 -5.409200 -4.304200 -3.012200 -5.015200 -5.250200 -4.818200 -4.252200 -4.983200 -5.891200 -6.058200
76918819        0.0 -1.056456 -2.563456 -2.730456 -1.681456 -3.684456 -5.191456 -4.086456 -2.794456 -4.797456 -5.032456 -4.600456 -4.034456 -4.765456 -5.673456 -5.840456
88521409        0.0 -1.556900 -3.063900 -3.230900 -2.181900 -4.184900 -5.691900 -4.586900 -3.294900 -5.297900 -5.532900 -5.100900 -4.534900 -5.265900 -6.173900 -6.340900
117968675       0.0 -0.580022 -2.087022 -2.254022 -1.205022 -3.208022 -4.715022 -3.610022 -2.318022 -4.321022 -4.556022 -4.124022 -3.558022 -4.289022 -5.197022 -5.364022
130727875       0.0  0.050058 -1.456942 -1.623942 -0.574942 -2.577942 -4.084942 -2.979942 -1.687942 -3.690942 -3.925942 -3.493942 -2.927942 -3.658942 -4.566942 -4.733942
171036246       0.0 -2.330844 -3.837844 -4.004844 -2.955844 -4.958844 -6.465844 -5.360844 -4.068844 -6.071844 -6.306844 -5.874844 -5.308844 -6.039844 -6.947844 -7.114844
309118137       0.0 -0.429504 -1.936504 -2.103504 -1.054504 -3.057504 -4.564504 -3.459504 -2.167504 -4.170504 -4.405504 -3.973504 -3.407504 -4.138504 -5.046504 -5.213504
309128059       0.0 -0.838681 -2.345681 -2.512681 -1.463681 -3.466681 -4.973681 -3.868681 -2.576681 -4.579681 -4.814681 -4.382681 -3.816681 -4.547681 -5.455681 -5.622681
[00:37.89] INFO: stop_frequency running segment social with 7 chooser rows
[00:37.91] INFO: stop_frequency.social.simple_simulate Running adaptive_chunked_choosers with 7 choosers
[00:37.91] INFO: Running chunk 1 of 1 with 7 of 7 choosers
before eval utilities                                                                                       0out_0in  0out_1in  0out_2in  0out_3in  ...  3out_0in  3out_1in  3out_2in  3out_3in
Expression                        Label                                                                                       ...                                        
auto_ownership                    util_number_of_vehicles                                  0.0    -0.190    -0.190    -0.190  ...    -0.190    -0.190    -0.190    -0.190
tour_mode_is_transit              util_dummy_for_all_stops_made_by_transit                 0.0    -0.700    -0.700    -0.700  ...    -0.700    -0.700    -0.700    -0.700
tour_mode_is_non_motorized        util_dummy_for_walking_to_all_stops                      0.0    -1.730    -1.730    -1.730  ...    -1.730    -1.730    -1.730    -1.730
~is_joint * num_work_tours        util_number_of_work_tours_undertaken_by_the_person       0.0    -0.280    -0.280    -0.280  ...    -0.280    -0.280    -0.280    -0.280
~is_joint * num_shop_tours        util_number_of_shop_tours_undertaken_by_the_person       0.0    -0.240    -0.240    -0.240  ...    -0.240    -0.240    -0.240    -0.240
is_joint * number_of_participants util_number_of_persons_participating_in_the_tou...       0.0     0.000     0.000     0.000  ...    -0.460    -0.460    -0.460    -0.460
composition=='mixed'              util_at_least_one_kid_and_one_adult_participate...       0.0     0.370     0.370     0.370  ...     0.370     0.370     0.370     0.370
(end > 16)                        util_arrival_later_than_17_00_                           0.0    -0.450    -0.450    -0.450  ...    -0.450    -0.450    -0.450    -0.450
(duration > 2)                    util_dummy_for_the_duration_of_the_tour_being_e...       0.0     1.310     1.310     1.310  ...     1.310     1.310     1.310     1.310
distance_in_miles                 util_dummy_for_distance_in_miles                         0.0    -0.010    -0.010    -0.010  ...    -0.010    -0.010    -0.010    -0.010
tour_mode_is_drive_transit        util_no_stops_if_tour_mode_is_drivetransit               0.0  -999.000  -999.000  -999.000  ...  -999.000  -999.000  -999.000  -999.000
~is_joint                         util_alternative_specific_constant_for_outbound...       0.0     0.000     0.000     0.000  ...    -4.552    -4.552    -4.552    -4.552
~is_joint # (2)                   util_alternative_specific_constant_for_return_s...       0.0    -1.120    -2.764    -3.451  ...     0.000    -1.120    -2.764    -3.451
~is_joint # (3)                   util_alternative_specific_constant_for_the_tota...       0.0     0.000     0.000     0.000  ...     0.000     0.496     0.882     0.882
is_joint                          util_alternative_specific_constant_for_outbound...       0.0     0.000     0.000     0.000  ...    -4.998    -4.998    -4.998    -4.998
is_joint # (2)                    util_alternative_specific_constant_for_return_s...       0.0    -1.329    -2.796    -3.379  ...     0.000    -1.329    -2.796    -3.379
is_joint # (3)                    util_alternative_specific_constant_for_the_tota...       0.0     0.000     0.000     0.000  ...     0.000     0.518     1.497     1.497
primary_purpose == 'social'       util_dummy_for_an_outbound_visiting_tour                 0.0     0.000     0.000     0.000  ...    -0.690    -0.690    -0.690    -0.690
primary_purpose == 'social' # (2) util_dummy_for_a_return_visiting_tour                    0.0    -0.640    -0.640    -0.640  ...     0.000    -0.640    -0.640    -0.640
primary_purpose == 'social' # (3) util_dummy_for_a_visiting_tour_with_both_outbou...       0.0     0.000     0.000     0.000  ...     0.000     0.440     0.440     0.440

[20 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
207350894    5057338    social                1              1         4  ...                  0                  0  0.000000  0.000000                      0
1100640        26844    social                1              1         1  ...                  0                  0  7.981950  8.016915                      0
58514990     1427194    social                1              1         2  ...                  0                  0  7.616518  8.142717                      0
80305793     1958677    social                1              1         1  ...                  0                  0  7.149057  7.914738                      0
147451001    3596365    social                1              1         1  ...                  0                  0  0.000000  0.000000                      0
220897868    5387752    social                1              1         3  ...                  0                  0  7.188751  7.764263                      0
308013110    7512514    social                1              1         3  ...                  0                  0  7.546934  7.188751                      0

[7 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in  3out_3in
tour_id                                                                                                                                                                  
207350894       0.0   -2.7329   -4.3769   -5.0639   -2.7439   -4.0639   -5.7079   -5.8989   -4.5369   -5.8569   -7.0049   -7.3059   -6.2149   -7.0389   -8.2969   -8.9839
1100640         0.0   -2.1825   -3.8265   -4.5135   -2.1935   -3.5135   -5.1575   -5.3485   -3.9865   -5.3065   -6.4545   -6.7555   -5.6645   -6.4885   -7.7465   -8.4335
58514990        0.0   -2.8245   -4.4685   -5.1555   -2.8355   -4.1555   -5.7995   -5.9905   -4.6285   -5.9485   -7.0965   -7.3975   -6.3065   -7.1305   -8.3885   -9.0755
80305793        0.0   -2.4755   -4.1195   -4.8065   -2.4865   -3.8065   -5.4505   -5.6415   -4.2795   -5.5995   -6.7475   -7.0485   -5.9575   -6.7815   -8.0395   -8.7265
147451001       0.0   -0.9017   -2.5457   -3.2327   -0.9127   -2.2327   -3.8767   -4.0677   -2.7057   -4.0257   -5.1737   -5.4747   -4.3837   -5.2077   -6.4657   -7.1527
220897868       0.0   -2.9249   -4.5689   -5.2559   -2.9359   -4.2559   -5.8999   -6.0909   -4.7289   -6.0489   -7.1969   -7.4979   -6.4069   -7.2309   -8.4889   -9.1759
308013110       0.0   -2.7053   -4.3493   -5.0363   -2.7163   -4.0363   -5.6803   -5.8713   -4.5093   -5.8293   -6.9773   -7.2783   -6.1873   -7.0113   -8.2693   -8.9563
[00:38.15] INFO: stop_frequency running segment shopping with 31 chooser rows
[00:38.16] INFO: stop_frequency.shopping.simple_simulate Running adaptive_chunked_choosers with 31 choosers
[00:38.16] INFO: Running chunk 1 of 1 with 31 of 31 choosers
before eval utilities                                                                                 0out_0in  0out_1in  0out_2in  0out_3in  ...  3out_0in  3out_1in  3out_2in  3out_3in
Expression                  Label                                                                                       ...                                        
hhsize                      util_number_of_hh_persons                                0.0   -0.1522   -0.1522   -0.1522  ...   -0.1522   -0.1522   -0.1522   -0.1522
num_age_5_15                util_num_kids_between_5_and_15_including_years_old       0.0    0.0482    0.0482    0.0482  ...    0.0482    0.0482    0.0482    0.0482
~is_joint & female          util_dummy_for_female                                    0.0    0.1721    0.1721    0.1721  ...    0.1721    0.1721    0.1721    0.1721
tour_mode_is_transit        util_dummy_for_all_stops_made_by_transit                 0.0   -0.7000   -0.7000   -0.7000  ...   -0.7000   -0.7000   -0.7000   -0.7000
tour_mode_is_non_motorized  util_dummy_for_walking_to_all_stops                      0.0   -1.4908   -1.4908   -1.4908  ...   -1.4908   -1.4908   -1.4908   -1.4908
~is_joint * num_work_tours  util_number_of_work_tours_undertaken_by_the_person       0.0   -0.5480   -0.5480   -0.5480  ...   -0.5480   -0.5480   -0.5480   -0.5480
~is_joint * num_univ_tours  util_number_of_university_tours_tours_undertake...       0.0   -0.6709   -0.6709   -0.6709  ...   -0.6709   -0.6709   -0.6709   -0.6709
~is_joint * num_maint_tours util_number_of_maintenace_tours_tours_undertake...       0.0   -0.1977   -0.1977   -0.1977  ...   -0.1977   -0.1977   -0.1977   -0.1977
num_hh_shop_tours           util_number_of_shop_tours_undertaken_by_the_hou...       0.0   -0.0733   -0.0733   -0.0733  ...   -0.0733   -0.0733   -0.0733   -0.0733
composition=='adults'       util_dummy_for_only_adults_participate_in_the_tour       0.0    0.1902    0.1902    0.1902  ...    0.1902    0.1902    0.1902    0.1902
(duration > 8)              util_dummy_for_the_duration_of_the_tour_being_e...       0.0    0.9056    0.9056    0.9056  ...    0.9056    0.9056    0.9056    0.9056
(distance_in_miles < 5)     util_dummy_for_distance_less_than_5_miles                0.0    0.3768    0.3768    0.3768  ...    0.3768    0.3768    0.3768    0.3768
distance_in_miles           util_dummy_for_distance_in_miles                         0.0    0.0289    0.0289    0.0289  ...    0.0289    0.0289    0.0289    0.0289
tour_mode_is_drive_transit  util_no_stops_if_tour_mode_is_drivetransit               0.0 -999.0000 -999.0000 -999.0000  ... -999.0000 -999.0000 -999.0000 -999.0000
~is_joint                   util_alternative_specific_constant_for_outbound...       0.0    0.0000    0.0000    0.0000  ...   -4.4870   -4.4870   -4.4870   -4.4870
~is_joint # (2)             util_alternative_specific_constant_for_return_s...       0.0   -1.1790   -2.3050   -3.0240  ...    0.0000   -1.1790   -2.3050   -3.0240
~is_joint # (3)             util_alternative_specific_constant_for_the_tota...       0.0    0.0000    0.0000    0.0000  ...    0.0000    0.2520    0.5140    0.5140
is_joint                    util_alternative_specific_constant_for_outbound...       0.0    0.0000    0.0000    0.0000  ...   -4.9980   -4.9980   -4.9980   -4.9980
is_joint # (2)              util_alternative_specific_constant_for_return_s...       0.0   -1.3290   -2.7960   -3.3790  ...    0.0000   -1.3290   -2.7960   -3.3790
is_joint # (3)              util_alternative_specific_constant_for_the_tota...       0.0    0.0000    0.0000    0.0000  ...    0.0000    0.5180    1.4970    1.4970

[20 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
55606652     1356259  shopping                1              1         1  ...                  0                  0  8.108436  7.667142                      1
100798519    2458502  shopping                1              1         1  ...                  0                  0  0.000000  0.000000                      0
132571975    3233462  shopping                2              1         1  ...                  0                  0  6.661200  7.016154                      0
132571976    3233462  shopping                2              2         2  ...                  0                  0  6.661200  7.767894                      0
207350891    5057338  shopping                1              1         1  ...                  0                  0  7.607878  7.180366                      0
309118139    7539466  shopping                1              1         1  ...                  0                  0  7.340975  7.546934                      0
1214658        29625  shopping                1              1         1  ...                  1                  0  7.667142  7.667142                      1
4594657       112064  shopping                1              1         1  ...                  1                  0  8.247303  8.050369                      0
13342745      325432  shopping                1              1         1  ...                  0                  0  0.000000  0.000000                      0
15826320      386007  shopping                1              1         1  ...                  0                  0  7.149057  6.945206                      0
26423590      644477  shopping                1              1         1  ...                  0                  0  0.000000  0.000000                      0
51944614     1266941  shopping                1              1         1  ...                  0                  0  8.108436  8.318059                      0
58424910     1424997  shopping                1              1         1  ...                  0                  0  7.149057  6.945206                      0
58514946     1427193  shopping                1              1         1  ...                  0                  0  7.616518  8.293606                      0
60521773     1476140  shopping                1              1         1  ...                  0                  0  7.607878  7.549557                      0
64479052     1572659  shopping                1              1         1  ...                  0                  0  7.838241  7.713628                      0
64490163     1572930  shopping                1              1         1  ...                  0                  0  7.415630  8.016915                      0
66961807     1633214  shopping                2              1         1  ...                  0                  0  0.000000  0.000000                      1
66961808     1633214  shopping                2              2         2  ...                  0                  0  6.941084  6.997756                      1
120398583    2936550  shopping                1              1         1  ...                  0                  0  7.981950  8.228287                      0
120410678    2936845  shopping                2              1         1  ...                  0                  0  7.348368  6.945206                      0
120410679    2936845  shopping                2              2         2  ...                  0                  0  0.000000  0.000000                      0
125537687    3061894  shopping                1              1         1  ...                  0                  0  0.000000  0.000000                      1
136471362    3328569  shopping                1              1         1  ...                  0                  0  7.501424  7.814365                      0
143283635    3494722  shopping                1              1         1  ...                  0                  0  7.068341  7.149057                      0
147450957    3596364  shopping                1              1         1  ...                  0                  0  7.149057  6.941084                      0
159535256    3891103  shopping                1              1         1  ...                  0                  0  8.247303  8.247303                      0
171036412    4171619  shopping                1              1         1  ...                  0                  0  8.247303  8.137361                      0
171036535    4171622  shopping                1              1         1  ...                  0                  0  8.247303  7.088693                      1
305589966    7453413  shopping                1              1         1  ...                  0                  0  7.494197  7.088693                      1
308013107    7512514  shopping                1              1         1  ...                  0                  0  7.981950  7.838241                      0

[31 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in  3out_3in
tour_id                                                                                                                                                                  
55606652        0.0 -2.794223 -3.920223 -4.639223 -2.954223 -4.133223 -5.259223 -5.726223 -4.725223 -5.904223 -6.778223 -7.235223 -6.102223 -7.029223 -7.893223 -8.612223
100798519       0.0 -1.447260 -2.914260 -3.497260 -1.901260 -3.230260 -4.697260 -4.762260 -4.185260 -5.514260 -6.463260 -6.067260 -5.116260 -5.927260 -6.415260 -6.998260
132571975       0.0 -2.012093 -3.138093 -3.857093 -2.172093 -3.351093 -4.477093 -4.944093 -3.943093 -5.122093 -5.996093 -6.453093 -5.320093 -6.247093 -7.111093 -7.830093
132571976       0.0 -2.006602 -3.132602 -3.851602 -2.166602 -3.345602 -4.471602 -4.938602 -3.937602 -5.116602 -5.990602 -6.447602 -5.314602 -6.241602 -7.105602 -7.824602
207350891       0.0 -2.370911 -3.496911 -4.215911 -2.530911 -3.709911 -4.835911 -5.302911 -4.301911 -5.480911 -6.354911 -6.811911 -5.678911 -6.605911 -7.469911 -8.188911
309118139       0.0 -1.453689 -2.579689 -3.298689 -1.613689 -2.792689 -3.918689 -4.385689 -3.384689 -4.563689 -5.437689 -5.894689 -4.761689 -5.688689 -6.552689 -7.271689
1214658         0.0 -2.510119 -3.636119 -4.355119 -2.670119 -3.849119 -4.975119 -5.442119 -4.441119 -5.620119 -6.494119 -6.951119 -5.818119 -6.745119 -7.609119 -8.328119
4594657         0.0 -2.321835 -3.447835 -4.166835 -2.481835 -3.660835 -4.786835 -5.253835 -4.252835 -5.431835 -6.305835 -6.762835 -5.629835 -6.556835 -7.420835 -8.139835
13342745        0.0 -0.924963 -2.050963 -2.769963 -1.084963 -2.263963 -3.389963 -3.856963 -2.855963 -4.034963 -4.908963 -5.365963 -4.232963 -5.159963 -6.023963 -6.742963
15826320        0.0 -1.536014 -2.662014 -3.381014 -1.696014 -2.875014 -4.001014 -4.468014 -3.467014 -4.646014 -5.520014 -5.977014 -4.844014 -5.771014 -6.635014 -7.354014
26423590        0.0 -1.033587 -2.159587 -2.878587 -1.193587 -2.372587 -3.498587 -3.965587 -2.964587 -4.143587 -5.017587 -5.474587 -4.341587 -5.268587 -6.132587 -6.851587
51944614        0.0 -2.422947 -3.548947 -4.267947 -2.582947 -3.761947 -4.887947 -5.354947 -4.353947 -5.532947 -6.406947 -6.863947 -5.730947 -6.657947 -7.521947 -8.240947
58424910        0.0 -1.584214 -2.710214 -3.429214 -1.744214 -2.923214 -4.049214 -4.516214 -3.515214 -4.694214 -5.568214 -6.025214 -4.892214 -5.819214 -6.683214 -7.402214
58514946        0.0 -2.570234 -3.696234 -4.415234 -2.730234 -3.909234 -5.035234 -5.502234 -4.501234 -5.680234 -6.554234 -7.011234 -5.878234 -6.805234 -7.669234 -8.388234
60521773        0.0 -1.621206 -2.747206 -3.466206 -1.781206 -2.960206 -4.086206 -4.553206 -3.552206 -4.731206 -5.605206 -6.062206 -4.929206 -5.856206 -6.720206 -7.439206
64479052        0.0 -1.508099 -2.634099 -3.353099 -1.668099 -2.847099 -3.973099 -4.440099 -3.439099 -4.618099 -5.492099 -5.949099 -4.816099 -5.743099 -6.607099 -7.326099
64490163        0.0 -2.622092 -3.748092 -4.467092 -2.782092 -3.961092 -5.087092 -5.554092 -4.553092 -5.732092 -6.606092 -7.063092 -5.930092 -6.857092 -7.721092 -8.440092
66961807        0.0 -1.084763 -2.210763 -2.929763 -1.244763 -2.423763 -3.549763 -4.016763 -3.015763 -4.194763 -5.068763 -5.525763 -4.392763 -5.319763 -6.183763 -6.902763
66961808        0.0 -1.800080 -2.926080 -3.645080 -1.960080 -3.139080 -4.265080 -4.732080 -3.731080 -4.910080 -5.784080 -6.241080 -5.108080 -6.035080 -6.899080 -7.618080
120398583       0.0 -2.257494 -3.383494 -4.102494 -2.417494 -3.596494 -4.722494 -5.189494 -4.188494 -5.367494 -6.241494 -6.698494 -5.565494 -6.492494 -7.356494 -8.075494
120410678       0.0 -1.621743 -2.747743 -3.466743 -1.781743 -2.960743 -4.086743 -4.553743 -3.552743 -4.731743 -5.605743 -6.062743 -4.929743 -5.856743 -6.720743 -7.439743
120410679       0.0 -0.921743 -2.047743 -2.766743 -1.081743 -2.260743 -3.386743 -3.853743 -2.852743 -4.031743 -4.905743 -5.362743 -4.229743 -5.156743 -6.020743 -6.739743
125537687       0.0 -1.048222 -2.174222 -2.893222 -1.208222 -2.387222 -3.513222 -3.980222 -2.979222 -4.158222 -5.032222 -5.489222 -4.356222 -5.283222 -6.147222 -6.866222
136471362       0.0 -1.783480 -2.909480 -3.628480 -1.943480 -3.122480 -4.248480 -4.715480 -3.714480 -4.893480 -5.767480 -6.224480 -5.091480 -6.018480 -6.882480 -7.601480
143283635       0.0 -0.874990 -2.000990 -2.719990 -1.034990 -2.213990 -3.339990 -3.806990 -2.805990 -3.984990 -4.858990 -5.315990 -4.182990 -5.109990 -5.973990 -6.692990
147450957       0.0 -1.532257 -2.658257 -3.377257 -1.692257 -2.871257 -3.997257 -4.464257 -3.463257 -4.642257 -5.516257 -5.973257 -4.840257 -5.767257 -6.631257 -7.350257
159535256       0.0 -2.696487 -3.822487 -4.541487 -2.856487 -4.035487 -5.161487 -5.628487 -4.627487 -5.806487 -6.680487 -7.137487 -6.004487 -6.931487 -7.795487 -8.514487
171036412       0.0 -3.475961 -4.601961 -5.320961 -3.635961 -4.814961 -5.940961 -6.407961 -5.406961 -6.585961 -7.459961 -7.916961 -6.783961 -7.710961 -8.574961 -9.293961
171036535       0.0 -3.444460 -4.570460 -5.289460 -3.604460 -4.783460 -5.909460 -6.376460 -5.375460 -6.554460 -7.428460 -7.885460 -6.752460 -7.679460 -8.543460 -9.262460
305589966       0.0 -2.425837 -3.551837 -4.270837 -2.585837 -3.764837 -4.890837 -5.357837 -4.356837 -5.535837 -6.409837 -6.866837 -5.733837 -6.660837 -7.524837 -8.243837
308013107       0.0 -2.432773 -3.558773 -4.277773 -2.592773 -3.771773 -4.897773 -5.364773 -4.363773 -5.542773 -6.416773 -6.873773 -5.740773 -6.667773 -7.531773 -8.250773
[00:38.39] INFO: stop_frequency running segment eatout with 16 chooser rows
[00:38.41] INFO: stop_frequency.eatout.simple_simulate Running adaptive_chunked_choosers with 16 choosers
[00:38.41] INFO: Running chunk 1 of 1 with 16 of 16 choosers
before eval utilities                                                                                       0out_0in  0out_1in  0out_2in  0out_3in  ...  3out_0in  3out_1in  3out_2in  3out_3in
Expression                        Label                                                                                       ...                                        
auto_ownership                    util_number_of_vehicles                                  0.0    -0.190    -0.190    -0.190  ...    -0.190    -0.190    -0.190    -0.190
tour_mode_is_transit              util_dummy_for_all_stops_made_by_transit                 0.0    -0.700    -0.700    -0.700  ...    -0.700    -0.700    -0.700    -0.700
tour_mode_is_non_motorized        util_dummy_for_walking_to_all_stops                      0.0    -1.730    -1.730    -1.730  ...    -1.730    -1.730    -1.730    -1.730
~is_joint * num_work_tours        util_number_of_work_tours_undertaken_by_the_person       0.0    -0.280    -0.280    -0.280  ...    -0.280    -0.280    -0.280    -0.280
~is_joint * num_shop_tours        util_number_of_shop_tours_undertaken_by_the_person       0.0    -0.240    -0.240    -0.240  ...    -0.240    -0.240    -0.240    -0.240
is_joint * number_of_participants util_number_of_persons_participating_in_the_tou...       0.0     0.000     0.000     0.000  ...    -0.460    -0.460    -0.460    -0.460
composition=='mixed'              util_at_least_one_kid_and_one_adult_participate...       0.0     0.370     0.370     0.370  ...     0.000     0.370     0.370     0.370
(end > 16)                        util_arrival_later_than_17_00_                           0.0    -0.450    -0.450    -0.450  ...    -0.450    -0.450    -0.450    -0.450
(duration > 2)                    util_dummy_for_the_duration_of_the_tour_being_e...       0.0     1.310     1.310     1.310  ...     1.310     1.310     1.310     1.310
distance_in_miles                 util_dummy_for_distance_in_miles                         0.0    -0.010    -0.010    -0.010  ...    -0.010    -0.010    -0.010    -0.010
tour_mode_is_drive_transit        util_no_stops_if_tour_mode_is_drivetransit               0.0  -999.000  -999.000  -999.000  ...  -999.000  -999.000  -999.000  -999.000
~is_joint                         util_alternative_specific_constant_for_outbound...       0.0     0.000     0.000     0.000  ...    -5.255    -5.255    -5.255    -5.255
~is_joint # (2)                   util_alternative_specific_constant_for_return_s...       0.0    -1.761    -3.697    -4.717  ...     0.000    -1.761    -3.697    -4.717
~is_joint # (3)                   util_alternative_specific_constant_for_the_tota...       0.0     0.000     0.000     0.000  ...     0.000     0.940     2.026     2.026
is_joint                          util_alternative_specific_constant_for_outbound...       0.0     0.000     0.000     0.000  ...    -4.998    -4.998    -4.998    -4.998
is_joint # (2)                    util_alternative_specific_constant_for_return_s...       0.0    -1.329    -2.796    -3.379  ...     0.000    -1.329    -2.796    -3.379
is_joint # (3)                    util_alternative_specific_constant_for_the_tota...       0.0     0.000     0.000     0.000  ...     0.000     0.518     1.497     1.497
primary_purpose == 'social'       util_dummy_for_an_outbound_visiting_tour                 0.0     0.000     0.000     0.000  ...    -0.690    -0.690    -0.690    -0.690
primary_purpose == 'social' # (2) util_dummy_for_a_return_visiting_tour                    0.0    -0.640    -0.640    -0.640  ...     0.000    -0.640    -0.640    -0.640
primary_purpose == 'social' # (3) util_dummy_for_a_visiting_tour_with_both_outbou...       0.0     0.000     0.000     0.000  ...     0.000     0.440     0.440     0.440

[20 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
15826334      386008    eatout                1              1         1  ...                  0                  0  7.188751  7.016154                      0
207350864    5057338    eatout                1              1         3  ...                  0                  0  7.607878  7.306522                      0
220958279    5389226    eatout                1              1         1  ...                  0                  0  8.247303  8.422746                      0
221029612    5390966    eatout                1              1         1  ...                  0                  0  7.667142  7.838241                      0
1085604        26478    eatout                1              1         2  ...                  0                  1  7.981950  8.422746                      0
1094132        26686    eatout                1              1         2  ...                  0                  1  7.981950  8.318059                      0
4412754       107628    eatout                1              1         1  ...                  0                  0  7.838241  7.838241                      0
15826293      386007    eatout                1              1         2  ...                  0                  0  7.415630  7.981950                      0
51813592     1263746    eatout                1              1         1  ...                  0                  0  7.981950  7.637908                      0
66959115     1633149    eatout                1              1         1  ...                  0                  0  8.108436  8.422746                      0
76904608     1875722    eatout                1              1         1  ...                  0                  0  8.247303  8.293606                      0
120410774    2936848    eatout                1              1         2  ...                  0                  0  0.000000  0.000000                      0
136471335    3328569    eatout                1              1         2  ...                  0                  0  7.501424  7.691842                      0
147450930    3596364    eatout                1              1         2  ...                  0                  0  7.415630  8.318059                      0
220876723    5387237    eatout                1              1         1  ...                  0                  0  7.415630  7.596636                      1
308013080    7512514    eatout                1              1         2  ...                  0                  0  7.981950  8.422746                      0

[16 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in  3out_3in
tour_id                                                                                                                                                                  
15826334        0.0   -1.8084   -3.7444   -4.7644   -2.2374   -3.9984   -5.9344   -6.0144   -4.5634   -6.3244   -7.3204   -7.2544   -5.3024   -6.1234   -6.9734   -7.9934
207350864       0.0   -3.4404   -5.3764   -6.3964   -3.8694   -5.6304   -7.5664   -7.6464   -6.1954   -7.9564   -8.9524   -8.8864   -6.9344   -7.7554   -8.6054   -9.6254
220958279       0.0   -3.5136   -4.9806   -5.5636   -4.8876   -6.2166   -7.6836   -7.7486   -7.1716   -8.5006   -9.4496   -9.0536   -8.1026   -8.9136   -9.4016   -9.9846
221029612       0.0   -4.4270   -6.3630   -7.3830   -4.8560   -6.6170   -8.5530   -8.6330   -7.1820   -8.9430   -9.9390   -9.8730   -7.9210   -8.7420   -9.5920  -10.6120
1085604         0.0   -3.5001   -5.4361   -6.4561   -3.9291   -5.6901   -7.6261   -7.7061   -6.2551   -8.0161   -9.0121   -8.9461   -6.9941   -7.8151   -8.6651   -9.6851
1094132         0.0   -3.9470   -5.8830   -6.9030   -4.3760   -6.1370   -8.0730   -8.1530   -6.7020   -8.4630   -9.4590   -9.3930   -7.4410   -8.2620   -9.1120  -10.1320
4412754         0.0   -3.4920   -5.4280   -6.4480   -3.9210   -5.6820   -7.6180   -7.6980   -6.2470   -8.0080   -9.0040   -8.9380   -6.9860   -7.8070   -8.6570   -9.6770
15826293        0.0   -2.6163   -4.5523   -5.5723   -3.0453   -4.8063   -6.7423   -6.8223   -5.3713   -7.1323   -8.1283   -8.0623   -6.1103   -6.9313   -7.7813   -8.8013
51813592        0.0   -2.1953   -4.1313   -5.1513   -2.6243   -4.3853   -6.3213   -6.4013   -4.9503   -6.7113   -7.7073   -7.6413   -5.6893   -6.5103   -7.3603   -8.3803
66959115        0.0   -2.3786   -4.3146   -5.3346   -2.8076   -4.5686   -6.5046   -6.5846   -5.1336   -6.8946   -7.8906   -7.8246   -5.8726   -6.6936   -7.5436   -8.5636
76904608        0.0   -2.8243   -4.7603   -5.7803   -3.2533   -5.0143   -6.9503   -7.0303   -5.5793   -7.3403   -8.3363   -8.2703   -6.3183   -7.1393   -7.9893   -9.0093
120410774       0.0   -1.9575   -3.8935   -4.9135   -2.3865   -4.1475   -6.0835   -6.1635   -4.7125   -6.4735   -7.4695   -7.4035   -5.4515   -6.2725   -7.1225   -8.1425
136471335       0.0   -2.7101   -4.6461   -5.6661   -3.1391   -4.9001   -6.8361   -6.9161   -5.4651   -7.2261   -8.2221   -8.1561   -6.2041   -7.0251   -7.8751   -8.8951
147450930       0.0   -3.7414   -5.6774   -6.6974   -4.1704   -5.9314   -7.8674   -7.9474   -6.4964   -8.2574   -9.2534   -9.1874   -7.2354   -8.0564   -8.9064   -9.9264
220876723       0.0   -2.1957   -4.1317   -5.1517   -2.6247   -4.3857   -6.3217   -6.4017   -4.9507   -6.7117   -7.7077   -7.6417   -5.6897   -6.5107   -7.3607   -8.3807
308013080       0.0   -2.4301   -4.3661   -5.3861   -2.8591   -4.6201   -6.5561   -6.6361   -5.1851   -6.9461   -7.9421   -7.8761   -5.9241   -6.7451   -7.5951   -8.6151
[00:38.66] INFO: stop_frequency running segment escort with 3 chooser rows
[00:38.67] INFO: stop_frequency.escort.simple_simulate Running adaptive_chunked_choosers with 3 choosers
[00:38.67] INFO: Running chunk 1 of 1 with 3 of 3 choosers
before eval utilities                                                                                0out_0in  0out_1in  0out_2in  0out_3in  ...  3out_0in  3out_1in  3out_2in  3out_3in
Expression                 Label                                                                                       ...                                        
hhsize                     util_number_of_hh_persons                                0.0    -0.240    -0.240    -0.240  ...    -0.240    -0.240    -0.240    -0.240
num_student                util_number_of_students_in_hh                            0.0     0.190     0.190     0.190  ...     0.190     0.190     0.190     0.190
tour_mode_is_transit       util_dummy_for_all_stops_made_by_transit                 0.0    -0.700    -0.700    -0.700  ...    -0.700    -0.700    -0.700    -0.700
tour_mode_is_non_motorized util_dummy_for_walking_to_all_stops                      0.0    -1.910    -1.910    -1.910  ...    -1.910    -1.910    -1.910    -1.910
num_work_tours             util_number_of_work_tours_undertaken_by_the_person       0.0    -0.290    -0.290    -0.290  ...    -0.290    -0.290    -0.290    -0.290
num_escort_tours           util_number_of_escort_tours_tours_undertaken_by...       0.0    -0.150    -0.150    -0.150  ...    -0.150    -0.150    -0.150    -0.150
(duration > 8)             util_dummy_for_the_duration_of_the_tour_being_e...       0.0     0.590     0.590     0.590  ...     0.590     0.590     0.590     0.590
(distance_in_miles < 5)    util_dummy_for_distance_less_than_5_miles                0.0     0.320     0.320     0.320  ...     0.320     0.320     0.320     0.320
distance_in_miles          util_dummy_for_distance_in_miles                         0.0     0.010     0.010     0.010  ...     0.010     0.010     0.010     0.010
tour_mode_is_drive_transit util_no_stops_if_tour_mode_is_drivetransit               0.0  -999.000  -999.000  -999.000  ...  -999.000  -999.000  -999.000  -999.000
~is_joint                  util_alternative_specific_constant_for_outbound...       0.0     0.000     0.000     0.000  ...    -4.758    -4.758    -4.758    -4.758
~is_joint # (2)            util_alternative_specific_constant_for_return_s...       0.0    -0.968    -2.410    -3.024  ...     0.000    -0.968    -2.410    -3.024
~is_joint # (3)            util_alternative_specific_constant_for_the_tota...       0.0     0.000     0.000     0.000  ...     0.000     0.000    -1.807    -1.807
is_joint                   util_alternative_specific_constant_for_outbound...       0.0     0.000     0.000     0.000  ...    -4.998    -4.998    -4.998    -4.998
is_joint # (2)             util_alternative_specific_constant_for_return_s...       0.0    -1.329    -2.796    -3.379  ...     0.000    -1.329    -2.796    -3.379
is_joint # (3)             util_alternative_specific_constant_for_the_tota...       0.0     0.000     0.000     0.000  ...     0.000     0.518     1.497     1.497

[16 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
17156090      418441    escort                1              1         1  ...                  0                  0  0.000000  0.000000                      0
26423525      644476    escort                1              1         1  ...                  0                  0  0.000000  0.000000                      0
100798550    2458501    escort                1              1         1  ...                  0                  0  7.546934  7.016154                      0

[3 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in  3out_3in
tour_id                                                                                                                                                                  
17156090        0.0   -1.3682   -2.8102   -3.4242   -2.5732   -3.5412   -4.9832   -5.5972   -4.6942   -5.6622   -7.1042   -9.5252   -5.1582   -6.1262   -9.3752   -9.9892
26423525        0.0   -1.6067   -3.0487   -3.6627   -2.8117   -3.7797   -5.2217   -5.8357   -4.9327   -5.9007   -7.3427   -9.7637   -5.3967   -6.3647   -9.6137  -10.2277
100798550       0.0   -2.0646   -3.5066   -4.1206   -3.2696   -4.2376   -5.6796   -6.2936   -5.3906   -6.3586   -7.8006  -10.2216   -5.8546   -6.8226  -10.0716  -10.6856
[00:38.86] INFO: stop_frequency running segment othmaint with 20 chooser rows
[00:38.88] INFO: stop_frequency.othmaint.simple_simulate Running adaptive_chunked_choosers with 20 choosers
[00:38.88] INFO: Running chunk 1 of 1 with 20 of 20 choosers
before eval utilities                                                                                                        0out_0in  0out_1in  0out_2in  ...  3out_1in  3out_2in  3out_3in
Expression                                         Label                                                                             ...                              
(income_in_thousands>19999) & (income_in_thousa... util_middle_to_low_income_hh_                            0.0    0.1700    0.1700  ...    0.1700    0.1700    0.1700
(income_in_thousands>=50000) & (income_in_thous... util_mid_to_high_income_hh                               0.0    0.2300    0.2300  ...    0.2300    0.2300    0.2300
(income_in_thousands>=100000)                      util_high_income_hh                                      0.0    0.2400    0.2400  ...    0.2400    0.2400    0.2400
hhsize                                             util_number_of_hh_persons                                0.0   -0.3100   -0.3100  ...   -0.3100   -0.3100   -0.3100
num_student                                        util_number_of_students_in_hh                            0.0    0.2100    0.2100  ...    0.2100    0.2100    0.2100
(num_age_0_4 > 0)                                  util_presence_of_kids_between_0_and_4_including...       0.0    0.7400    0.7400  ...    0.7400    0.7400    0.7400
~is_joint & female                                 util_dummy_for_female                                    0.0    0.3012    0.3012  ...    0.3012    0.3012    0.3012
tour_mode_is_transit                               util_dummy_for_all_stops_made_by_transit                 0.0   -0.7000   -0.7000  ...   -0.7000   -0.7000   -0.7000
tour_mode_is_non_motorized                         util_dummy_for_walking_to_all_stops                      0.0   -1.4329   -1.4329  ...   -1.4329   -1.4329   -1.4329
~is_joint * num_work_tours                         util_number_of_work_tours_undertaken_by_the_person       0.0   -0.3640   -0.3640  ...   -0.3640   -0.3640   -0.3640
~is_joint * num_univ_tours                         util_number_of_university_tours_tours_undertake...       0.0   -0.6252   -0.6252  ...   -0.6252   -0.6252   -0.6252
~is_joint * num_school_tours                       util_number_of_shool_tours_tours_undertaken_by_...       0.0   -1.4135   -1.4135  ...   -1.4135   -1.4135   -1.4135
~is_joint * num_shop_tours                         util_number_of_shop_tours_undertaken_by_the_person       0.0   -0.1428   -0.1428  ...   -0.1428   -0.1428   -0.1428
num_hh_maint_tours                                 util_number_of_maintenace_tours_undertaken_by_t...       0.0   -0.0468   -0.0468  ...   -0.0468   -0.0468   -0.0468
is_joint * number_of_participants # (2)            util_number_of_persons_participating_in_the_tou...       0.0    0.4904    0.4904  ...    0.4904    0.4904    0.4904
(duration > 8)                                     util_dummy_for_the_duration_of_the_tour_being_e...       0.0    0.5134    0.5134  ...    0.5134    0.5134    0.5134
(distance_in_miles < 20)                           util_dummy_for_distance_less_than_20_miles_              0.0   -0.4080   -0.4080  ...   -0.4080   -0.4080   -0.4080
distance_in_miles                                  util_dummy_for_distance_in_miles                         0.0    0.0273    0.0273  ...    0.0273    0.0273    0.0273
tour_mode_is_drive_transit                         util_no_stops_if_tour_mode_is_drivetransit               0.0 -999.0000 -999.0000  ... -999.0000 -999.0000 -999.0000
~is_joint                                          util_alternative_specific_constant_for_outbound...       0.0    0.0000    0.0000  ...   -5.4260   -5.4260   -5.4260
~is_joint # (2)                                    util_alternative_specific_constant_for_return_s...       0.0   -0.5850   -1.4800  ...   -0.5850   -1.4800   -2.4620
~is_joint # (3)                                    util_alternative_specific_constant_for_the_tota...       0.0    0.0000    0.0000  ...    0.4140    0.4880    0.4880
is_joint                                           util_alternative_specific_constant_for_outbound...       0.0    0.0000    0.0000  ...   -4.9980   -4.9980   -4.9980
is_joint # (2)                                     util_alternative_specific_constant_for_return_s...       0.0   -1.3290   -2.7960  ...   -1.3290   -2.7960   -3.3790
is_joint # (3)                                     util_alternative_specific_constant_for_the_tota...       0.0    0.0000    0.0000  ...    0.5180    1.4970    1.4970

[25 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
13321297      324909  othmaint                1              1         1  ...                  0                  0  0.000000  0.000000                      0
17156109      418441  othmaint                1              1         2  ...                  0                  0  7.379336  7.455702                      0
26415959      644291  othmaint                1              1         1  ...                  0                  0  0.000000  0.000000                      0
26423544      644476  othmaint                1              1         2  ...                  0                  0  8.247303  8.422746                      0
60521727     1476139  othmaint                1              1         1  ...                  0                  0  8.016915  7.713628                      0
91019946     2219998  othmaint                1              1         1  ...                  0                  0  0.000000  0.000000                      0
125537723    3061895  othmaint                1              1         1  ...                  0                  0  7.713628  8.016915                      0
197775705    4823797  othmaint                1              1         1  ...                  0                  0  0.000000  0.000000                      0
207350886    5057338  othmaint                1              1         2  ...                  0                  0  8.016915  8.016915                      0
1085626        26478  othmaint                1              1         1  ...                  0                  1  7.981950  7.567826                      0
1094154        26686  othmaint                1              1         1  ...                  0                  1  7.981950  7.415630                      0
15828529      386061  othmaint                1              1         1  ...                  0                  0  7.415630  7.713628                      0
55616077     1356489  othmaint                1              1         1  ...                  0                  0  8.108436  7.088693                      1
58514982     1427194  othmaint                1              1         1  ...                  0                  0  7.616518  8.293606                      0
64490158     1572930  othmaint                1              1         2  ...                  0                  0  7.415630  7.415630                      0
66961802     1633214  othmaint                1              1         3  ...                  0                  0  0.000000  0.000000                      1
100798528    2458500  othmaint                1              1         1  ...                  0                  0  7.981950  8.016915                      0
143283671    3494723  othmaint                1              1         1  ...                  0                  0  0.000000  0.000000                      0
171036284    4171616  othmaint                1              1         1  ...                  0                  0  8.247303  8.050369                      0
220859115    5386807  othmaint                1              1         1  ...                  0                  0  8.016915  8.050369                      0

[20 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in   3out_3in
tour_id                                                                                                                                                                   
13321297        0.0 -1.943694 -2.838694 -3.820694 -3.119694 -3.704694 -4.599694 -5.167694 -5.019694 -5.604694 -6.085694 -6.993694 -6.784694 -6.955694 -7.776694  -8.758694
17156109        0.0 -1.412423 -2.307423 -3.289423 -2.588423 -3.173423 -4.068423 -4.636423 -4.488423 -5.073423 -5.554423 -6.462423 -6.253423 -6.424423 -7.245423  -8.227423
26415959        0.0 -1.749592 -2.644592 -3.626592 -2.925592 -3.510592 -4.405592 -4.973592 -4.825592 -5.410592 -5.891592 -6.799592 -6.590592 -6.761592 -7.582592  -8.564592
26423544        0.0 -3.497342 -4.392342 -5.374342 -4.673342 -5.258342 -6.153342 -6.721342 -6.573342 -7.158342 -7.639342 -8.547342 -8.338342 -8.509342 -9.330342 -10.312342
60521727        0.0 -3.377140 -4.272140 -5.254140 -4.553140 -5.138140 -6.033140 -6.601140 -6.453140 -7.038140 -7.519140 -8.427140 -8.218140 -8.389140 -9.210140 -10.192140
91019946        0.0 -3.078179 -3.973179 -4.955179 -4.254179 -4.839179 -5.734179 -6.302179 -6.154179 -6.739179 -7.220179 -8.128179 -7.919179 -8.090179 -8.911179  -9.893179
125537723       0.0 -3.076486 -3.971486 -4.953486 -4.252486 -4.837486 -5.732486 -6.300486 -6.152486 -6.737486 -7.218486 -8.126486 -7.917486 -8.088486 -8.909486  -9.891486
197775705       0.0 -1.639427 -2.534427 -3.516427 -2.815427 -3.400427 -4.295427 -4.863427 -4.715427 -5.300427 -5.781427 -6.689427 -6.480427 -6.651427 -7.472427  -8.454427
207350886       0.0 -3.238605 -4.133605 -5.115605 -4.414605 -4.999605 -5.894605 -6.462605 -6.314605 -6.899605 -7.380605 -8.288605 -8.079605 -8.250605 -9.071605 -10.053605
1085626         0.0 -2.762771 -3.657771 -4.639771 -3.938771 -4.523771 -5.418771 -5.986771 -5.838771 -6.423771 -6.904771 -7.812771 -7.603771 -7.774771 -8.595771  -9.577771
1094154         0.0 -2.768231 -3.663231 -4.645231 -3.944231 -4.529231 -5.424231 -5.992231 -5.844231 -6.429231 -6.910231 -7.818231 -7.609231 -7.780231 -8.601231  -9.583231
15828529        0.0 -2.790309 -3.685309 -4.667309 -3.966309 -4.551309 -5.446309 -6.014309 -5.866309 -6.451309 -6.932309 -7.840309 -7.631309 -7.802309 -8.623309  -9.605309
55616077        0.0 -2.710511 -3.605511 -4.587511 -3.886511 -4.471511 -5.366511 -5.934511 -5.786511 -6.371511 -6.852511 -7.760511 -7.551511 -7.722511 -8.543511  -9.525511
58514982        0.0 -2.719038 -3.614038 -4.596038 -3.895038 -4.480038 -5.375038 -5.943038 -5.795038 -6.380038 -6.861038 -7.769038 -7.560038 -7.731038 -8.552038  -9.534038
64490158        0.0 -2.874059 -3.769059 -4.751059 -4.050059 -4.635059 -5.530059 -6.098059 -5.950059 -6.535059 -7.016059 -7.924059 -7.715059 -7.886059 -8.707059  -9.689059
66961802        0.0 -1.549834 -2.444834 -3.426834 -2.725834 -3.310834 -4.205834 -4.773834 -4.625834 -5.210834 -5.691834 -6.599834 -6.390834 -6.561834 -7.382834  -8.364834
100798528       0.0 -3.239075 -4.134075 -5.116075 -4.415075 -5.000075 -5.895075 -6.463075 -6.315075 -6.900075 -7.381075 -8.289075 -8.080075 -8.251075 -9.072075 -10.054075
143283671       0.0 -1.309070 -2.204070 -3.186070 -2.485070 -3.070070 -3.965070 -4.533070 -4.385070 -4.970070 -5.451070 -6.359070 -6.150070 -6.321070 -7.142070  -8.124070
171036284       0.0 -3.822695 -4.717695 -5.699695 -4.998695 -5.583695 -6.478695 -7.046695 -6.898695 -7.483695 -7.964695 -8.872695 -8.663695 -8.834695 -9.655695 -10.637695
220859115       0.0 -2.299295 -3.194295 -4.176295 -3.475295 -4.060295 -4.955295 -5.523295 -5.375295 -5.960295 -6.441295 -7.349295 -7.140295 -7.311295 -8.132295  -9.114295
[00:39.12] INFO: stop_frequency running segment othdiscr with 21 chooser rows
[00:39.14] INFO: stop_frequency.othdiscr.simple_simulate Running adaptive_chunked_choosers with 21 choosers
[00:39.14] INFO: Running chunk 1 of 1 with 21 of 21 choosers
before eval utilities                                                                                  0out_0in  0out_1in  0out_2in  0out_3in  ...  3out_0in  3out_1in  3out_2in  3out_3in
Expression                   Label                                                                                       ...                                        
tour_mode_is_transit         util_dummy_for_all_stops_made_by_transit                 0.0   -0.7000   -0.7000   -0.7000  ...   -0.7000   -0.7000   -0.7000   -0.7000
tour_mode_is_non_motorized   util_dummy_for_walking_to_all_stops                      0.0   -2.4578   -2.4578   -2.4578  ...   -2.4578   -2.4578   -2.4578   -2.4578
~is_joint * num_work_tours   util_number_of_work_tours_undertaken_by_the_person       0.0   -0.6153   -0.6153   -0.6153  ...   -0.6153   -0.6153   -0.6153   -0.6153
~is_joint * num_school_tours util_number_of_shool_tours_tours_undertaken_by_...       0.0   -0.8176   -0.8176   -0.8176  ...   -0.8176   -0.8176   -0.8176   -0.8176
~is_joint * num_shop_tours   util_number_of_shop_tours_undertaken_by_the_person       0.0   -0.6290   -0.6290   -0.6290  ...   -0.6290   -0.6290   -0.6290   -0.6290
~is_joint * num_maint_tours  util_number_of_maintenace_tours_tours_undertake...       0.0   -0.3715   -0.3715   -0.3715  ...   -0.3715   -0.3715   -0.3715   -0.3715
(end > 16)                   util_arrival_later_than_17_00_                           0.0   -0.6383   -0.6383   -0.6383  ...   -0.6383   -0.6383   -0.6383   -0.6383
(duration > 8)               util_dummy_for_the_duration_of_the_tour_being_e...       0.0    0.8335    0.8335    0.8335  ...    0.8335    0.8335    0.8335    0.8335
(distance_in_miles < 10)     util_dummy_for_distance_less_than_10_miles_              0.0    0.3756    0.3756    0.3756  ...    0.3756    0.3756    0.3756    0.3756
distance_in_miles            util_dummy_for_distance_in_miles                         0.0   -0.0225   -0.0225   -0.0225  ...   -0.0225   -0.0225   -0.0225   -0.0225
tour_mode_is_drive_transit   util_no_stops_if_tour_mode_is_drivetransit               0.0 -999.0000 -999.0000 -999.0000  ... -999.0000 -999.0000 -999.0000 -999.0000
~is_joint                    util_alternative_specific_constant_for_outbound...       0.0    0.0000    0.0000    0.0000  ...   -4.6230   -4.6230   -4.6230   -4.6230
~is_joint # (2)              util_alternative_specific_constant_for_return_s...       0.0   -0.9210   -2.3360   -2.9270  ...    0.0000   -0.9210   -2.3360   -2.9270
~is_joint # (3)              util_alternative_specific_constant_for_the_tota...       0.0    0.0000    0.0000    0.0000  ...    0.0000    0.8630    0.9390    0.9390
is_joint                     util_alternative_specific_constant_for_outbound...       0.0    0.0000    0.0000    0.0000  ...   -4.9980   -4.9980   -4.9980   -4.9980
is_joint # (2)               util_alternative_specific_constant_for_return_s...       0.0   -1.3290   -2.7960   -3.3790  ...    0.0000   -1.3290   -2.7960   -3.3790
is_joint # (3)               util_alternative_specific_constant_for_the_tota...       0.0    0.0000    0.0000    0.0000  ...    0.0000    0.5180    1.4970    1.4970

[17 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
26423541      644476  othdiscr                1              1         3  ...                  0                  0  8.247303  8.247303                      0
125537720    3061895  othdiscr                1              1         2  ...                  0                  0  7.322742  7.188751                      0
130727777    3188483  othdiscr                1              1         1  ...                  0                  0  7.616518  7.713628                      0
132571967    3233462  othdiscr                1              1         3  ...                  0                  0  6.661200  7.982914                      0
143309047    3495342  othdiscr                1              1         1  ...                  0                  0  0.000000  0.000000                      0
159535289    3891104  othdiscr                2              1         1  ...                  0                  0  8.247303  8.142717                      0
159535290    3891104  othdiscr                2              2         2  ...                  0                  0  7.016154  6.088624                      1
197775702    4823797  othdiscr                1              1         2  ...                  0                  0  8.293606  8.247303                      0
299527165    7305540  othdiscr                1              1         1  ...                  0                  0  7.494197  8.420518                      0
1136791        27726  othdiscr                1              1         1  ...                  0                  0  7.567826  7.415630                      0
4594649       112064  othdiscr                1              1         2  ...                  1                  0  0.000000  0.000000                      1
10828453      264108  othdiscr                1              1         1  ...                  0                  0  7.415630  7.088693                      1
51901843     1265898  othdiscr                1              1         1  ...                  0                  0  6.661200  6.636653                      1
120398575    2936550  othdiscr                1              1         2  ...                  0                  0  7.981950  7.838241                      0
120410793    2936848  othdiscr                1              1         1  ...                  0                  0  0.000000  0.000000                      0
143283668    3494723  othdiscr                1              1         2  ...                  0                  0  7.838241  8.293606                      0
220859112    5386807  othdiscr                1              1         2  ...                  0                  0  8.016915  8.420518                      0
220897857    5387752  othdiscr                2              1         1  ...                  0                  0  7.149057  7.427060                      0
220897858    5387752  othdiscr                2              2         2  ...                  0                  0  0.000000  0.000000                      0
308464222    7523517  othdiscr                1              1         1  ...                  0                  0  7.641910  7.188751                      0
308469593    7523648  othdiscr                1              1         1  ...                  0                  0  0.000000  0.000000                      0

[21 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in   3out_3in
tour_id                                                                                                                                                                   
26423541        0.0 -4.632125 -6.047125 -6.638125 -5.292125 -6.213125 -7.628125 -7.356125 -7.034125 -7.955125 -8.507125 -9.022125 -8.334125 -8.392125 -9.731125 -10.322125
125537720       0.0 -2.910100 -4.325100 -4.916100 -3.570100 -4.491100 -5.906100 -5.634100 -5.312100 -6.233100 -6.785100 -7.300100 -6.612100 -6.670100 -8.009100  -8.600100
130727777       0.0 -3.422000 -4.889000 -5.472000 -3.876000 -5.205000 -6.672000 -6.737000 -6.160000 -7.489000 -8.438000 -8.042000 -7.091000 -7.902000 -8.390000  -8.973000
132571967       0.0 -3.138275 -4.553275 -5.144275 -3.798275 -4.719275 -6.134275 -5.862275 -5.540275 -6.461275 -7.013275 -7.528275 -6.840275 -6.898275 -8.237275  -8.828275
143309047       0.0 -1.835225 -3.250225 -3.841225 -2.495225 -3.416225 -4.831225 -4.559225 -4.237225 -5.158225 -5.710225 -6.225225 -5.537225 -5.595225 -6.934225  -7.525225
159535289       0.0 -3.835650 -5.250650 -5.841650 -4.495650 -5.416650 -6.831650 -6.559650 -6.237650 -7.158650 -7.710650 -8.225650 -7.537650 -7.595650 -8.934650  -9.525650
159535290       0.0 -2.737300 -4.152300 -4.743300 -3.397300 -4.318300 -5.733300 -5.461300 -5.139300 -6.060300 -6.612300 -7.127300 -6.439300 -6.497300 -7.836300  -8.427300
197775702       0.0 -4.638875 -6.053875 -6.644875 -5.298875 -6.219875 -7.634875 -7.362875 -7.040875 -7.961875 -8.513875 -9.028875 -8.340875 -8.398875 -9.737875 -10.328875
299527165       0.0 -4.282225 -5.697225 -6.288225 -4.942225 -5.863225 -7.278225 -7.006225 -6.684225 -7.605225 -8.157225 -8.672225 -7.984225 -8.042225 -9.381225  -9.972225
1136791         0.0 -3.014225 -4.429225 -5.020225 -3.674225 -4.595225 -6.010225 -5.738225 -5.416225 -6.337225 -6.889225 -7.404225 -6.716225 -6.774225 -8.113225  -8.704225
4594649         0.0 -1.196000 -2.611000 -3.202000 -1.856000 -2.777000 -4.192000 -3.920000 -3.598000 -4.519000 -5.071000 -5.586000 -4.898000 -4.956000 -6.295000  -6.886000
10828453        0.0 -3.027050 -4.442050 -5.033050 -3.687050 -4.608050 -6.023050 -5.751050 -5.429050 -6.350050 -6.902050 -7.417050 -6.729050 -6.787050 -8.126050  -8.717050
51901843        0.0 -1.083950 -2.498950 -3.089950 -1.743950 -2.664950 -4.079950 -3.807950 -3.485950 -4.406950 -4.958950 -5.473950 -4.785950 -4.843950 -6.182950  -6.773950
120398575       0.0 -4.280175 -5.695175 -6.286175 -4.940175 -5.861175 -7.276175 -7.004175 -6.682175 -7.603175 -8.155175 -8.670175 -7.982175 -8.040175 -9.379175  -9.970175
120410793       0.0 -0.550575 -1.965575 -2.556575 -1.210575 -2.131575 -3.546575 -3.274575 -2.952575 -3.873575 -4.425575 -4.940575 -4.252575 -4.310575 -5.649575  -6.240575
143283668       0.0 -3.398775 -4.813775 -5.404775 -4.058775 -4.979775 -6.394775 -6.122775 -5.800775 -6.721775 -7.273775 -7.788775 -7.100775 -7.158775 -8.497775  -9.088775
220859112       0.0 -3.392025 -4.807025 -5.398025 -4.052025 -4.973025 -6.388025 -6.116025 -5.794025 -6.715025 -7.267025 -7.782025 -7.094025 -7.152025 -8.491025  -9.082025
220897857       0.0 -1.277575 -2.692575 -3.283575 -1.937575 -2.858575 -4.273575 -4.001575 -3.679575 -4.600575 -5.152575 -5.667575 -4.979575 -5.037575 -6.376575  -6.967575
220897858       0.0 -1.223075 -2.638075 -3.229075 -1.883075 -2.804075 -4.219075 -3.947075 -3.625075 -4.546075 -5.098075 -5.613075 -4.925075 -4.983075 -6.322075  -6.913075
308464222       0.0 -1.264750 -2.679750 -3.270750 -1.924750 -2.845750 -4.260750 -3.988750 -3.666750 -4.587750 -5.139750 -5.654750 -4.966750 -5.024750 -6.363750  -6.954750
308469593       0.0 -0.556425 -1.971425 -2.562425 -1.216425 -2.137425 -3.552425 -3.280425 -2.958425 -3.879425 -4.431425 -4.946425 -4.258425 -4.316425 -5.655425  -6.246425
[00:39.35] INFO: stop_frequency running segment atwork with 9 chooser rows
[00:39.37] INFO: stop_frequency.atwork.simple_simulate Running adaptive_chunked_choosers with 9 choosers
[00:39.37] INFO: Running chunk 1 of 1 with 9 of 9 choosers
before eval utilities                                                                               0out_0in  0out_1in  0out_2in  0out_3in  ...  3out_0in  3out_1in  3out_2in  3out_3in
Expression                Label                                                                                       ...                                        
income_in_thousands<50000 util_middle_to_low_income_hh_                            0.0     0.450     0.450      0.45  ...     0.450     0.450     0.450     0.450
num_eatout_tours          util_number_of_eating_tours_tours_undertaken_by...       0.0    -0.280    -0.280     -0.28  ...    -0.280    -0.280    -0.280    -0.280
start<12                  util_subtour_departure_less_than_or_equal_to_11am        0.0     0.310     0.310      0.31  ...     0.310     0.310     0.310     0.310
end>16                    util_subtour_return_time_greater_or_equal_to_2pm         0.0     0.340     0.340      0.34  ...     0.340     0.340     0.340     0.340
end-start                 util_subtour_duration_in_hours_integer_                  0.0     0.560     0.560      0.56  ...     0.560     0.560     0.560     0.560
destination_area_type >5  util_dummy_for_subtour_origin_tour_destination_...       0.0     0.270     0.270      0.27  ...     0.270     0.270     0.270     0.270
pracc                     util_primary_destination_accessibility_log_of_it_        0.0     0.180     0.180      0.18  ...     0.180     0.180     0.180     0.180
distance_in_miles         util_subtour_distance_in_miles_from_tour_destin...       0.0     0.020     0.020      0.02  ...     0.020     0.020     0.020     0.020
1                         util_alternative_specific_constant_for_outbound...       0.0     0.000     0.000      0.00  ...    -7.361    -7.361    -7.361    -7.361
1 # (2)                   util_alternative_specific_constant_for_return_s...       0.0    -3.671    -5.388     -6.21  ...     0.000    -3.671    -5.388    -6.210
1 # (3)                   util_alternative_specific_constant_for_the_tota...       0.0     0.000     0.000      0.00  ...     0.000     2.127     2.127     2.127

[11 rows x 16 columns]            person_id tour_type  tour_type_count  tour_type_num  tour_num  ...  num_hh_shop_tours num_hh_maint_tours     hhacc     pracc  destination_area_type
tour_id                                                                   ...                                                                                 
13286136      324052       eat                1              1         1  ...                  0                  0  8.247303  7.616518                      0
66896301     1631617       eat                1              1         1  ...                  0                  0  7.415630  8.142717                      0
66923525     1632281       eat                1              1         1  ...                  0                  0  8.420518  7.981950                      0
91019881     2219997       eat                1              1         1  ...                  0                  0  8.247303  8.228287                      0
143309067    3495343       eat                1              1         1  ...                  0                  0  7.567826  8.247303                      0
171036547    4171623       eat                1              1         1  ...                  0                  0  8.247303  8.016915                      0
220897896    5387753     maint                1              1         1  ...                  0                  0  7.415630  8.000487                      0
220958270    5389226       eat                1              1         1  ...                  0                  0  8.247303  8.142717                      0
307996473    7512109       eat                1              1         1  ...                  0                  0  7.981950  7.981950                      0

[9 rows x 175 columns] {'TRANSIT_MODES': ['WALK_LOC', 'WALK_LRF', 'WALK_EXP', 'WALK_HVY', 'WALK_COM', 'DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'DRIVE_TO_TRANSIT_MODES': ['DRIVE_LOC', 'DRIVE_LRF', 'DRIVE_EXP', 'DRIVE_HVY', 'DRIVE_COM'], 'NONMOTORIZED_MODES': ['WALK', 'BIKE'], 'SHOP_TOUR': 'shopping', 'MAINT_TOUR': 'othmaint', 'SCHOOL_TOUR': 'school', 'EATOUT_TOUR': 'eatout', 'SOCIAL_TOUR': 'social', 'num_atwork_subtours_map': {'no_subtours': 0, 'eat': 1, 'business1': 1, 'maint': 1, 'business2': 2, 'eat_business': 2}}
before logit.utils_to_probs 
            0out_0in  0out_1in  0out_2in  0out_3in  1out_0in  1out_1in  1out_2in  1out_3in  2out_0in  2out_1in  2out_2in  2out_3in  3out_0in  3out_1in  3out_2in  3out_3in
tour_id                                                                                                                                                                  
13286136        0.0 -1.528227 -3.245227 -4.067227 -1.753227 -5.424227 -7.141227 -5.836227 -3.566227 -7.237227 -6.827227 -7.649227 -5.218227 -6.762227 -8.479227 -9.301227
66896301        0.0 -1.750511 -3.467511 -4.289511 -1.975511 -5.646511 -7.363511 -6.058511 -3.788511 -7.459511 -7.049511 -7.871511 -5.440511 -6.984511 -8.701511 -9.523511
66923525        0.0 -1.204849 -2.921849 -3.743849 -1.429849 -5.100849 -6.817849 -5.512849 -3.242849 -6.913849 -6.503849 -7.325849 -4.894849 -6.438849 -8.155849 -8.977849
91019881        0.0 -1.719108 -3.436108 -4.258108 -1.944108 -5.615108 -7.332108 -6.027108 -3.757108 -7.428108 -7.018108 -7.840108 -5.409108 -6.953108 -8.670108 -9.492108
143309067       0.0 -1.701685 -3.418685 -4.240685 -1.926685 -5.597685 -7.314685 -6.009685 -3.739685 -7.410685 -7.000685 -7.822685 -5.391685 -6.935685 -8.652685 -9.474685
171036547       0.0 -1.449755 -3.166755 -3.988755 -1.674755 -5.345755 -7.062755 -5.757755 -3.487755 -7.158755 -6.748755 -7.570755 -5.139755 -6.683755 -8.400755 -9.222755
220897896       0.0 -1.211512 -2.928512 -3.750512 -1.436512 -5.107512 -6.824512 -5.519512 -3.249512 -6.920512 -6.510512 -7.332512 -4.901512 -6.445512 -8.162512 -8.984512
220958270       0.0 -1.750511 -3.467511 -4.289511 -1.975511 -5.646511 -7.363511 -6.058511 -3.788511 -7.459511 -7.049511 -7.871511 -5.440511 -6.984511 -8.701511 -9.523511
307996473       0.0 -1.453049 -3.170049 -3.992049 -1.678049 -5.349049 -7.066049 -5.761049 -3.491049 -7.162049 -6.752049 -7.574049 -5.143049 -6.687049 -8.404049 -9.226049
[00:39.55] INFO: stop_frequency top 10 value counts:
0out_0in    157
0out_1in     14
1out_0in     10
0out_2in      8
1out_1in      4
3out_0in      3
1out_3in      2
3out_1in      1
2out_2in      1
2out_0in      1
dtype: int64
Time to execute step 'stop_frequency': 2.53 s
Total time to execute iteration 1 with iteration value None: 2.53 s
[00:39.68] INFO: #run_model running step trip_purpose
Running step 'trip_purpose'
[00:39.68] INFO: assign purpose to 202 last outbound trips
[00:39.68] INFO: assign purpose to 202 last inbound trips
[00:39.68] INFO: assign purpose to 78 intermediate trips
[00:39.71] INFO: trip_purpose Running adaptive_chunked_choosers with 78 choosers
[00:39.71] INFO: Running chunk 1 of 1 with 78 of 78 choosers
Time to execute step 'trip_purpose': 0.05 s
Total time to execute iteration 1 with iteration value None: 0.05 s
[00:39.80] INFO: #run_model running step trip_destination
Running step 'trip_destination'
[00:39.82] INFO: Running trip_destination with 482 trips
[00:39.88] INFO: Running trip_destination.trip_num_1 with 53 trips
[00:39.88] INFO: choose_trip_destination trip_destination.trip_num_1.atwork with 5 trips
[00:39.89] INFO: trip_destination.trip_num_1.atwork.sample.interaction_sample Running adaptive_chunked_choosers with 5 choosers
[00:39.89] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:39.89] INFO: Running eval_interaction_utilities on 125 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:39.91] INFO: Time to execute trip_destination.trip_num_1.atwork.trip_destination_sample : 0.031 seconds (0.0 minutes)
[00:39.91] INFO: Running trip_destination.trip_num_1.atwork.compute_logsums with 68 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:40.06] INFO: trip_destination.trip_num_1.atwork.compute_logsums.od Running adaptive_chunked_choosers with 68 choosers
[00:40.06] INFO: Running chunk 1 of 1 with 68 of 68 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:40.54] INFO: trip_destination.trip_num_1.atwork.compute_logsums.dp Running adaptive_chunked_choosers with 68 choosers
[00:40.54] INFO: Running chunk 1 of 1 with 68 of 68 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:40.90] INFO: Time to execute trip_destination.trip_num_1.atwork.compute_logsums : 0.988 seconds (0.0 minutes)
[00:40.92] INFO: Running trip_destination_simulate with 5 trips
[00:40.92] INFO: trip_destination.trip_num_1.atwork.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 5 choosers and 68 alternatives
[00:40.92] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:40.92] INFO: Running eval_interaction_utilities on 68 rows
get item DIST
get item DIST
[00:40.96] INFO: Time to execute trip_destination.trip_num_1.atwork.trip_destination_simulate : 0.057 seconds (0.0 minutes)
[00:40.96] INFO: choose_trip_destination trip_destination.trip_num_1.eatout with 1 trips
[00:40.97] INFO: trip_destination.trip_num_1.eatout.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:40.97] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:40.98] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:41.06] INFO: Time to execute trip_destination.trip_num_1.eatout.trip_destination_sample : 0.101 seconds (0.0 minutes)
[00:41.06] INFO: Running trip_destination.trip_num_1.eatout.compute_logsums with 15 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:41.23] INFO: trip_destination.trip_num_1.eatout.compute_logsums.od Running adaptive_chunked_choosers with 15 choosers
[00:41.23] INFO: Running chunk 1 of 1 with 15 of 15 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:41.77] INFO: trip_destination.trip_num_1.eatout.compute_logsums.dp Running adaptive_chunked_choosers with 15 choosers
[00:41.78] INFO: Running chunk 1 of 1 with 15 of 15 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:42.18] INFO: Time to execute trip_destination.trip_num_1.eatout.compute_logsums : 1.124 seconds (0.0 minutes)
[00:42.19] INFO: Running trip_destination_simulate with 1 trips
[00:42.19] INFO: trip_destination.trip_num_1.eatout.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 15 alternatives
[00:42.19] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:42.20] INFO: Running eval_interaction_utilities on 15 rows
get item DIST
get item DIST
[00:42.23] INFO: Time to execute trip_destination.trip_num_1.eatout.trip_destination_simulate : 0.045 seconds (0.0 minutes)
[00:42.23] INFO: choose_trip_destination trip_destination.trip_num_1.othdiscr with 1 trips
[00:42.23] INFO: trip_destination.trip_num_1.othdiscr.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:42.23] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:42.24] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:42.26] INFO: Time to execute trip_destination.trip_num_1.othdiscr.trip_destination_sample : 0.03 seconds (0.0 minutes)
[00:42.26] INFO: Running trip_destination.trip_num_1.othdiscr.compute_logsums with 15 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:42.41] INFO: trip_destination.trip_num_1.othdiscr.compute_logsums.od Running adaptive_chunked_choosers with 15 choosers
[00:42.41] INFO: Running chunk 1 of 1 with 15 of 15 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:42.81] INFO: trip_destination.trip_num_1.othdiscr.compute_logsums.dp Running adaptive_chunked_choosers with 15 choosers
[00:42.81] INFO: Running chunk 1 of 1 with 15 of 15 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:43.13] INFO: Time to execute trip_destination.trip_num_1.othdiscr.compute_logsums : 0.876 seconds (0.0 minutes)
[00:43.14] INFO: Running trip_destination_simulate with 1 trips
[00:43.14] INFO: trip_destination.trip_num_1.othdiscr.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 15 alternatives
[00:43.14] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:43.14] INFO: Running eval_interaction_utilities on 15 rows
get item DIST
get item DIST
[00:43.16] INFO: Time to execute trip_destination.trip_num_1.othdiscr.trip_destination_simulate : 0.026 seconds (0.0 minutes)
[00:43.16] INFO: choose_trip_destination trip_destination.trip_num_1.othmaint with 3 trips
[00:43.17] INFO: trip_destination.trip_num_1.othmaint.sample.interaction_sample Running adaptive_chunked_choosers with 3 choosers
[00:43.17] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:43.17] INFO: Running eval_interaction_utilities on 75 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:43.19] INFO: Time to execute trip_destination.trip_num_1.othmaint.trip_destination_sample : 0.032 seconds (0.0 minutes)
[00:43.19] INFO: Running trip_destination.trip_num_1.othmaint.compute_logsums with 45 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:43.35] INFO: trip_destination.trip_num_1.othmaint.compute_logsums.od Running adaptive_chunked_choosers with 45 choosers
[00:43.35] INFO: Running chunk 1 of 1 with 45 of 45 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:43.82] INFO: trip_destination.trip_num_1.othmaint.compute_logsums.dp Running adaptive_chunked_choosers with 45 choosers
[00:43.82] INFO: Running chunk 1 of 1 with 45 of 45 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:44.18] INFO: Time to execute trip_destination.trip_num_1.othmaint.compute_logsums : 0.984 seconds (0.0 minutes)
[00:44.19] INFO: Running trip_destination_simulate with 3 trips
[00:44.19] INFO: trip_destination.trip_num_1.othmaint.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 3 choosers and 45 alternatives
[00:44.19] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:44.20] INFO: Running eval_interaction_utilities on 45 rows
get item DIST
get item DIST
[00:44.23] INFO: Time to execute trip_destination.trip_num_1.othmaint.trip_destination_simulate : 0.057 seconds (0.0 minutes)
[00:44.23] INFO: choose_trip_destination trip_destination.trip_num_1.school with 3 trips
[00:44.25] INFO: trip_destination.trip_num_1.school.sample.interaction_sample Running adaptive_chunked_choosers with 3 choosers
[00:44.25] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:44.25] INFO: Running eval_interaction_utilities on 75 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:44.27] INFO: Time to execute trip_destination.trip_num_1.school.trip_destination_sample : 0.04 seconds (0.0 minutes)
[00:44.27] INFO: Running trip_destination.trip_num_1.school.compute_logsums with 42 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:44.43] INFO: trip_destination.trip_num_1.school.compute_logsums.od Running adaptive_chunked_choosers with 42 choosers
[00:44.43] INFO: Running chunk 1 of 1 with 42 of 42 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:44.94] INFO: trip_destination.trip_num_1.school.compute_logsums.dp Running adaptive_chunked_choosers with 42 choosers
[00:44.94] INFO: Running chunk 1 of 1 with 42 of 42 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:45.32] INFO: Time to execute trip_destination.trip_num_1.school.compute_logsums : 1.043 seconds (0.0 minutes)
[00:45.33] INFO: Running trip_destination_simulate with 3 trips
[00:45.33] INFO: trip_destination.trip_num_1.school.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 3 choosers and 42 alternatives
[00:45.33] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:45.34] INFO: Running eval_interaction_utilities on 42 rows
get item DIST
get item DIST
[00:45.37] INFO: Time to execute trip_destination.trip_num_1.school.trip_destination_simulate : 0.057 seconds (0.0 minutes)
[00:45.37] INFO: choose_trip_destination trip_destination.trip_num_1.shopping with 10 trips
[00:45.39] INFO: trip_destination.trip_num_1.shopping.sample.interaction_sample Running adaptive_chunked_choosers with 10 choosers
[00:45.39] INFO: Running chunk 1 of 1 with 10 of 10 choosers
[00:45.39] INFO: Running eval_interaction_utilities on 250 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:45.41] INFO: Time to execute trip_destination.trip_num_1.shopping.trip_destination_sample : 0.041 seconds (0.0 minutes)
[00:45.42] INFO: Running trip_destination.trip_num_1.shopping.compute_logsums with 124 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:45.56] INFO: trip_destination.trip_num_1.shopping.compute_logsums.od Running adaptive_chunked_choosers with 124 choosers
[00:45.56] INFO: Running chunk 1 of 1 with 124 of 124 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:46.04] INFO: trip_destination.trip_num_1.shopping.compute_logsums.dp Running adaptive_chunked_choosers with 124 choosers
[00:46.04] INFO: Running chunk 1 of 1 with 124 of 124 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:46.40] INFO: Time to execute trip_destination.trip_num_1.shopping.compute_logsums : 0.989 seconds (0.0 minutes)
[00:46.42] INFO: Running trip_destination_simulate with 10 trips
[00:46.42] INFO: trip_destination.trip_num_1.shopping.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 10 choosers and 124 alternatives
[00:46.42] INFO: Running chunk 1 of 1 with 10 of 10 choosers
[00:46.43] INFO: Running eval_interaction_utilities on 124 rows
get item DIST
get item DIST
[00:46.46] INFO: Time to execute trip_destination.trip_num_1.shopping.trip_destination_simulate : 0.055 seconds (0.0 minutes)
[00:46.46] INFO: choose_trip_destination trip_destination.trip_num_1.social with 3 trips
[00:46.47] INFO: trip_destination.trip_num_1.social.sample.interaction_sample Running adaptive_chunked_choosers with 3 choosers
[00:46.47] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:46.48] INFO: Running eval_interaction_utilities on 75 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:46.50] INFO: Time to execute trip_destination.trip_num_1.social.trip_destination_sample : 0.041 seconds (0.0 minutes)
[00:46.50] INFO: Running trip_destination.trip_num_1.social.compute_logsums with 42 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:46.65] INFO: trip_destination.trip_num_1.social.compute_logsums.od Running adaptive_chunked_choosers with 42 choosers
[00:46.65] INFO: Running chunk 1 of 1 with 42 of 42 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:47.12] INFO: trip_destination.trip_num_1.social.compute_logsums.dp Running adaptive_chunked_choosers with 42 choosers
[00:47.12] INFO: Running chunk 1 of 1 with 42 of 42 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:47.50] INFO: Time to execute trip_destination.trip_num_1.social.compute_logsums : 0.996 seconds (0.0 minutes)
[00:47.51] INFO: Running trip_destination_simulate with 3 trips
[00:47.51] INFO: trip_destination.trip_num_1.social.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 3 choosers and 42 alternatives
[00:47.51] INFO: Running chunk 1 of 1 with 3 of 3 choosers
[00:47.52] INFO: Running eval_interaction_utilities on 42 rows
get item DIST
get item DIST
[00:47.55] INFO: Time to execute trip_destination.trip_num_1.social.trip_destination_simulate : 0.054 seconds (0.0 minutes)
[00:47.55] INFO: choose_trip_destination trip_destination.trip_num_1.univ with 5 trips
[00:47.56] INFO: trip_destination.trip_num_1.univ.sample.interaction_sample Running adaptive_chunked_choosers with 5 choosers
[00:47.56] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:47.57] INFO: Running eval_interaction_utilities on 125 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:47.59] INFO: Time to execute trip_destination.trip_num_1.univ.trip_destination_sample : 0.04 seconds (0.0 minutes)
[00:47.59] INFO: Running trip_destination.trip_num_1.univ.compute_logsums with 74 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:47.74] INFO: trip_destination.trip_num_1.univ.compute_logsums.od Running adaptive_chunked_choosers with 74 choosers
[00:47.74] INFO: Running chunk 1 of 1 with 74 of 74 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:48.21] INFO: trip_destination.trip_num_1.univ.compute_logsums.dp Running adaptive_chunked_choosers with 74 choosers
[00:48.21] INFO: Running chunk 1 of 1 with 74 of 74 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:48.56] INFO: Time to execute trip_destination.trip_num_1.univ.compute_logsums : 0.972 seconds (0.0 minutes)
[00:48.58] INFO: Running trip_destination_simulate with 5 trips
[00:48.58] INFO: trip_destination.trip_num_1.univ.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 5 choosers and 74 alternatives
[00:48.58] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:48.58] INFO: Running eval_interaction_utilities on 74 rows
get item DIST
get item DIST
[00:48.62] INFO: Time to execute trip_destination.trip_num_1.univ.trip_destination_simulate : 0.053 seconds (0.0 minutes)
[00:48.62] INFO: choose_trip_destination trip_destination.trip_num_1.work with 22 trips
[00:48.63] INFO: trip_destination.trip_num_1.work.sample.interaction_sample Running adaptive_chunked_choosers with 22 choosers
[00:48.63] INFO: Running chunk 1 of 1 with 22 of 22 choosers
[00:48.64] INFO: Running eval_interaction_utilities on 550 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:48.66] INFO: Time to execute trip_destination.trip_num_1.work.trip_destination_sample : 0.043 seconds (0.0 minutes)
[00:48.66] INFO: Running trip_destination.trip_num_1.work.compute_logsums with 287 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:48.81] INFO: trip_destination.trip_num_1.work.compute_logsums.od Running adaptive_chunked_choosers with 287 choosers
[00:48.81] INFO: Running chunk 1 of 1 with 287 of 287 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:49.35] INFO: trip_destination.trip_num_1.work.compute_logsums.dp Running adaptive_chunked_choosers with 287 choosers
[00:49.35] INFO: Running chunk 1 of 1 with 287 of 287 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:49.72] INFO: Time to execute trip_destination.trip_num_1.work.compute_logsums : 1.055 seconds (0.0 minutes)
[00:49.73] INFO: Running trip_destination_simulate with 22 trips
[00:49.73] INFO: trip_destination.trip_num_1.work.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 22 choosers and 287 alternatives
[00:49.73] INFO: Running chunk 1 of 1 with 22 of 22 choosers
[00:49.74] INFO: Running eval_interaction_utilities on 287 rows
get item DIST
get item DIST
[00:49.77] INFO: Time to execute trip_destination.trip_num_1.work.trip_destination_simulate : 0.058 seconds (0.0 minutes)
[00:49.81] INFO: Running trip_destination.trip_num_2 with 18 trips
[00:49.81] INFO: choose_trip_destination trip_destination.trip_num_2.atwork with 2 trips
[00:49.81] INFO: trip_destination.trip_num_2.atwork.sample.interaction_sample Running adaptive_chunked_choosers with 2 choosers
[00:49.81] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:49.82] INFO: Running eval_interaction_utilities on 50 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:49.84] INFO: Time to execute trip_destination.trip_num_2.atwork.trip_destination_sample : 0.028 seconds (0.0 minutes)
[00:49.84] INFO: Running trip_destination.trip_num_2.atwork.compute_logsums with 23 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:49.99] INFO: trip_destination.trip_num_2.atwork.compute_logsums.od Running adaptive_chunked_choosers with 23 choosers
[00:49.99] INFO: Running chunk 1 of 1 with 23 of 23 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:50.39] INFO: trip_destination.trip_num_2.atwork.compute_logsums.dp Running adaptive_chunked_choosers with 23 choosers
[00:50.39] INFO: Running chunk 1 of 1 with 23 of 23 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:50.70] INFO: Time to execute trip_destination.trip_num_2.atwork.compute_logsums : 0.866 seconds (0.0 minutes)
[00:50.71] INFO: Running trip_destination_simulate with 2 trips
[00:50.71] INFO: trip_destination.trip_num_2.atwork.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 2 choosers and 23 alternatives
[00:50.71] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:50.71] INFO: Running eval_interaction_utilities on 23 rows
get item DIST
get item DIST
[00:50.73] INFO: Time to execute trip_destination.trip_num_2.atwork.trip_destination_simulate : 0.024 seconds (0.0 minutes)
[00:50.73] INFO: choose_trip_destination trip_destination.trip_num_2.othmaint with 2 trips
[00:50.73] INFO: trip_destination.trip_num_2.othmaint.sample.interaction_sample Running adaptive_chunked_choosers with 2 choosers
[00:50.74] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:50.74] INFO: Running eval_interaction_utilities on 50 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:50.76] INFO: Time to execute trip_destination.trip_num_2.othmaint.trip_destination_sample : 0.032 seconds (0.0 minutes)
[00:50.76] INFO: Running trip_destination.trip_num_2.othmaint.compute_logsums with 23 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:50.91] INFO: trip_destination.trip_num_2.othmaint.compute_logsums.od Running adaptive_chunked_choosers with 23 choosers
[00:50.91] INFO: Running chunk 1 of 1 with 23 of 23 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:51.31] INFO: trip_destination.trip_num_2.othmaint.compute_logsums.dp Running adaptive_chunked_choosers with 23 choosers
[00:51.31] INFO: Running chunk 1 of 1 with 23 of 23 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:51.65] INFO: Time to execute trip_destination.trip_num_2.othmaint.compute_logsums : 0.885 seconds (0.0 minutes)
[00:51.65] INFO: Running trip_destination_simulate with 2 trips
[00:51.65] INFO: trip_destination.trip_num_2.othmaint.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 2 choosers and 23 alternatives
[00:51.65] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:51.66] INFO: Running eval_interaction_utilities on 23 rows
get item DIST
get item DIST
[00:51.67] INFO: Time to execute trip_destination.trip_num_2.othmaint.trip_destination_simulate : 0.027 seconds (0.0 minutes)
[00:51.67] INFO: choose_trip_destination trip_destination.trip_num_2.shopping with 5 trips
[00:51.68] INFO: trip_destination.trip_num_2.shopping.sample.interaction_sample Running adaptive_chunked_choosers with 5 choosers
[00:51.68] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:51.68] INFO: Running eval_interaction_utilities on 125 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:51.70] INFO: Time to execute trip_destination.trip_num_2.shopping.trip_destination_sample : 0.032 seconds (0.0 minutes)
[00:51.71] INFO: Running trip_destination.trip_num_2.shopping.compute_logsums with 69 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:51.86] INFO: trip_destination.trip_num_2.shopping.compute_logsums.od Running adaptive_chunked_choosers with 69 choosers
[00:51.86] INFO: Running chunk 1 of 1 with 69 of 69 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:52.33] INFO: trip_destination.trip_num_2.shopping.compute_logsums.dp Running adaptive_chunked_choosers with 69 choosers
[00:52.33] INFO: Running chunk 1 of 1 with 69 of 69 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:52.68] INFO: Time to execute trip_destination.trip_num_2.shopping.compute_logsums : 0.978 seconds (0.0 minutes)
[00:52.70] INFO: Running trip_destination_simulate with 5 trips
[00:52.70] INFO: trip_destination.trip_num_2.shopping.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 5 choosers and 69 alternatives
[00:52.70] INFO: Running chunk 1 of 1 with 5 of 5 choosers
[00:52.70] INFO: Running eval_interaction_utilities on 69 rows
get item DIST
get item DIST
[00:52.74] INFO: Time to execute trip_destination.trip_num_2.shopping.trip_destination_simulate : 0.055 seconds (0.0 minutes)
[00:52.74] INFO: choose_trip_destination trip_destination.trip_num_2.social with 1 trips
[00:52.75] INFO: trip_destination.trip_num_2.social.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:52.76] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:52.76] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:52.78] INFO: Time to execute trip_destination.trip_num_2.social.trip_destination_sample : 0.042 seconds (0.0 minutes)
[00:52.78] INFO: Running trip_destination.trip_num_2.social.compute_logsums with 10 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:52.93] INFO: trip_destination.trip_num_2.social.compute_logsums.od Running adaptive_chunked_choosers with 10 choosers
[00:52.93] INFO: Running chunk 1 of 1 with 10 of 10 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:53.34] INFO: trip_destination.trip_num_2.social.compute_logsums.dp Running adaptive_chunked_choosers with 10 choosers
[00:53.34] INFO: Running chunk 1 of 1 with 10 of 10 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:53.76] INFO: Time to execute trip_destination.trip_num_2.social.compute_logsums : 0.983 seconds (0.0 minutes)
[00:53.77] INFO: Running trip_destination_simulate with 1 trips
[00:53.77] INFO: trip_destination.trip_num_2.social.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 10 alternatives
[00:53.77] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:53.78] INFO: Running eval_interaction_utilities on 10 rows
get item DIST
get item DIST
[00:53.79] INFO: Time to execute trip_destination.trip_num_2.social.trip_destination_simulate : 0.03 seconds (0.0 minutes)
[00:53.79] INFO: choose_trip_destination trip_destination.trip_num_2.univ with 2 trips
[00:53.80] INFO: trip_destination.trip_num_2.univ.sample.interaction_sample Running adaptive_chunked_choosers with 2 choosers
[00:53.80] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:53.81] INFO: Running eval_interaction_utilities on 50 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:53.83] INFO: Time to execute trip_destination.trip_num_2.univ.trip_destination_sample : 0.034 seconds (0.0 minutes)
[00:53.83] INFO: Running trip_destination.trip_num_2.univ.compute_logsums with 27 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:53.99] INFO: trip_destination.trip_num_2.univ.compute_logsums.od Running adaptive_chunked_choosers with 27 choosers
[00:53.99] INFO: Running chunk 1 of 1 with 27 of 27 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:54.43] INFO: trip_destination.trip_num_2.univ.compute_logsums.dp Running adaptive_chunked_choosers with 27 choosers
[00:54.43] INFO: Running chunk 1 of 1 with 27 of 27 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:54.77] INFO: Time to execute trip_destination.trip_num_2.univ.compute_logsums : 0.937 seconds (0.0 minutes)
[00:54.77] INFO: Running trip_destination_simulate with 2 trips
[00:54.77] INFO: trip_destination.trip_num_2.univ.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 2 choosers and 27 alternatives
[00:54.77] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:54.78] INFO: Running eval_interaction_utilities on 27 rows
get item DIST
get item DIST
[00:54.79] INFO: Time to execute trip_destination.trip_num_2.univ.trip_destination_simulate : 0.026 seconds (0.0 minutes)
[00:54.79] INFO: choose_trip_destination trip_destination.trip_num_2.work with 6 trips
[00:54.80] INFO: trip_destination.trip_num_2.work.sample.interaction_sample Running adaptive_chunked_choosers with 6 choosers
[00:54.80] INFO: Running chunk 1 of 1 with 6 of 6 choosers
[00:54.80] INFO: Running eval_interaction_utilities on 150 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:54.82] INFO: Time to execute trip_destination.trip_num_2.work.trip_destination_sample : 0.033 seconds (0.0 minutes)
[00:54.82] INFO: Running trip_destination.trip_num_2.work.compute_logsums with 81 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:54.98] INFO: trip_destination.trip_num_2.work.compute_logsums.od Running adaptive_chunked_choosers with 81 choosers
[00:54.98] INFO: Running chunk 1 of 1 with 81 of 81 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:55.49] INFO: trip_destination.trip_num_2.work.compute_logsums.dp Running adaptive_chunked_choosers with 81 choosers
[00:55.49] INFO: Running chunk 1 of 1 with 81 of 81 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:55.84] INFO: Time to execute trip_destination.trip_num_2.work.compute_logsums : 1.014 seconds (0.0 minutes)
[00:55.85] INFO: Running trip_destination_simulate with 6 trips
[00:55.85] INFO: trip_destination.trip_num_2.work.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 6 choosers and 81 alternatives
[00:55.85] INFO: Running chunk 1 of 1 with 6 of 6 choosers
[00:55.86] INFO: Running eval_interaction_utilities on 81 rows
get item DIST
get item DIST
[00:55.89] INFO: Time to execute trip_destination.trip_num_2.work.trip_destination_simulate : 0.056 seconds (0.0 minutes)
[00:55.93] INFO: Running trip_destination.trip_num_3 with 7 trips
[00:55.93] INFO: choose_trip_destination trip_destination.trip_num_3.atwork with 2 trips
[00:55.94] INFO: trip_destination.trip_num_3.atwork.sample.interaction_sample Running adaptive_chunked_choosers with 2 choosers
[00:55.94] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:55.94] INFO: Running eval_interaction_utilities on 50 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:55.96] INFO: Time to execute trip_destination.trip_num_3.atwork.trip_destination_sample : 0.028 seconds (0.0 minutes)
[00:55.96] INFO: Running trip_destination.trip_num_3.atwork.compute_logsums with 29 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:56.11] INFO: trip_destination.trip_num_3.atwork.compute_logsums.od Running adaptive_chunked_choosers with 29 choosers
[00:56.11] INFO: Running chunk 1 of 1 with 29 of 29 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:56.53] INFO: trip_destination.trip_num_3.atwork.compute_logsums.dp Running adaptive_chunked_choosers with 29 choosers
[00:56.53] INFO: Running chunk 1 of 1 with 29 of 29 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:56.85] INFO: Time to execute trip_destination.trip_num_3.atwork.compute_logsums : 0.89 seconds (0.0 minutes)
[00:56.86] INFO: Running trip_destination_simulate with 2 trips
[00:56.86] INFO: trip_destination.trip_num_3.atwork.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 2 choosers and 29 alternatives
[00:56.86] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:56.86] INFO: Running eval_interaction_utilities on 29 rows
get item DIST
get item DIST
[00:56.88] INFO: Time to execute trip_destination.trip_num_3.atwork.trip_destination_simulate : 0.025 seconds (0.0 minutes)
[00:56.88] INFO: choose_trip_destination trip_destination.trip_num_3.othmaint with 1 trips
[00:56.88] INFO: trip_destination.trip_num_3.othmaint.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:56.88] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:56.89] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:56.91] INFO: Time to execute trip_destination.trip_num_3.othmaint.trip_destination_sample : 0.032 seconds (0.0 minutes)
[00:56.91] INFO: Running trip_destination.trip_num_3.othmaint.compute_logsums with 12 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:57.06] INFO: trip_destination.trip_num_3.othmaint.compute_logsums.od Running adaptive_chunked_choosers with 12 choosers
[00:57.06] INFO: Running chunk 1 of 1 with 12 of 12 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:57.48] INFO: trip_destination.trip_num_3.othmaint.compute_logsums.dp Running adaptive_chunked_choosers with 12 choosers
[00:57.48] INFO: Running chunk 1 of 1 with 12 of 12 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:57.86] INFO: Time to execute trip_destination.trip_num_3.othmaint.compute_logsums : 0.955 seconds (0.0 minutes)
[00:57.87] INFO: Running trip_destination_simulate with 1 trips
[00:57.87] INFO: trip_destination.trip_num_3.othmaint.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 12 alternatives
[00:57.87] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:57.88] INFO: Running eval_interaction_utilities on 12 rows
get item DIST
get item DIST
[00:57.90] INFO: Time to execute trip_destination.trip_num_3.othmaint.trip_destination_simulate : 0.031 seconds (0.0 minutes)
[00:57.90] INFO: choose_trip_destination trip_destination.trip_num_3.social with 1 trips
[00:57.90] INFO: trip_destination.trip_num_3.social.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:57.90] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:57.91] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:57.93] INFO: Time to execute trip_destination.trip_num_3.social.trip_destination_sample : 0.036 seconds (0.0 minutes)
[00:57.93] INFO: Running trip_destination.trip_num_3.social.compute_logsums with 17 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:58.08] INFO: trip_destination.trip_num_3.social.compute_logsums.od Running adaptive_chunked_choosers with 17 choosers
[00:58.08] INFO: Running chunk 1 of 1 with 17 of 17 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:58.49] INFO: trip_destination.trip_num_3.social.compute_logsums.dp Running adaptive_chunked_choosers with 17 choosers
[00:58.49] INFO: Running chunk 1 of 1 with 17 of 17 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:58.82] INFO: Time to execute trip_destination.trip_num_3.social.compute_logsums : 0.887 seconds (0.0 minutes)
[00:58.83] INFO: Running trip_destination_simulate with 1 trips
[00:58.83] INFO: trip_destination.trip_num_3.social.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 17 alternatives
[00:58.83] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:58.83] INFO: Running eval_interaction_utilities on 17 rows
get item DIST
get item DIST
[00:58.85] INFO: Time to execute trip_destination.trip_num_3.social.trip_destination_simulate : 0.029 seconds (0.0 minutes)
[00:58.85] INFO: choose_trip_destination trip_destination.trip_num_3.univ with 1 trips
[00:58.85] INFO: trip_destination.trip_num_3.univ.sample.interaction_sample Running adaptive_chunked_choosers with 1 choosers
[00:58.85] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:58.86] INFO: Running eval_interaction_utilities on 25 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:58.88] INFO: Time to execute trip_destination.trip_num_3.univ.trip_destination_sample : 0.033 seconds (0.0 minutes)
[00:58.88] INFO: Running trip_destination.trip_num_3.univ.compute_logsums with 14 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[00:59.04] INFO: trip_destination.trip_num_3.univ.compute_logsums.od Running adaptive_chunked_choosers with 14 choosers
[00:59.04] INFO: Running chunk 1 of 1 with 14 of 14 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[00:59.49] INFO: trip_destination.trip_num_3.univ.compute_logsums.dp Running adaptive_chunked_choosers with 14 choosers
[00:59.49] INFO: Running chunk 1 of 1 with 14 of 14 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[00:59.85] INFO: Time to execute trip_destination.trip_num_3.univ.compute_logsums : 0.969 seconds (0.0 minutes)
[00:59.86] INFO: Running trip_destination_simulate with 1 trips
[00:59.86] INFO: trip_destination.trip_num_3.univ.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 1 choosers and 14 alternatives
[00:59.86] INFO: Running chunk 1 of 1 with 1 of 1 choosers
[00:59.86] INFO: Running eval_interaction_utilities on 14 rows
get item DIST
get item DIST
[00:59.87] INFO: Time to execute trip_destination.trip_num_3.univ.trip_destination_simulate : 0.026 seconds (0.0 minutes)
[00:59.87] INFO: choose_trip_destination trip_destination.trip_num_3.work with 2 trips
[00:59.88] INFO: trip_destination.trip_num_3.work.sample.interaction_sample Running adaptive_chunked_choosers with 2 choosers
[00:59.88] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[00:59.89] INFO: Running eval_interaction_utilities on 50 rows
get item DIST
get item DIST
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
[00:59.91] INFO: Time to execute trip_destination.trip_num_3.work.trip_destination_sample : 0.032 seconds (0.0 minutes)
[00:59.91] INFO: Running trip_destination.trip_num_3.work.compute_logsums with 28 samples
get item DIST
get item DISTWALK
get item DISTBIKE
[01:00.05] INFO: trip_destination.trip_num_3.work.compute_logsums.od Running adaptive_chunked_choosers with 28 choosers
[01:00.05] INFO: Running chunk 1 of 1 with 28 of 28 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
get item DIST
get item DISTWALK
get item DISTBIKE
[01:00.50] INFO: trip_destination.trip_num_3.work.compute_logsums.dp Running adaptive_chunked_choosers with 28 choosers
[01:00.50] INFO: Running chunk 1 of 1 with 28 of 28 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:00.83] INFO: Time to execute trip_destination.trip_num_3.work.compute_logsums : 0.926 seconds (0.0 minutes)
[01:00.84] INFO: Running trip_destination_simulate with 2 trips
[01:00.84] INFO: trip_destination.trip_num_3.work.trip_dest_simulate.interaction_sample_simulate Running adaptive_chunked_choosers_and_alts with 2 choosers and 28 alternatives
[01:00.84] INFO: Running chunk 1 of 1 with 2 of 2 choosers
[01:00.84] INFO: Running eval_interaction_utilities on 28 rows
get item DIST
get item DIST
[01:00.86] INFO: Time to execute trip_destination.trip_num_3.work.trip_destination_simulate : 0.027 seconds (0.0 minutes)
Time to execute step 'trip_destination': 21.07 s
Total time to execute iteration 1 with iteration value None: 21.07 s
[01:00.94] INFO: #run_model running step trip_purpose_and_destination
Running step 'trip_purpose_and_destination'
[01:00.96] INFO: trip_purpose_and_destination - no failed trips from prior model run.
Time to execute step 'trip_purpose_and_destination': 0.02 s
Total time to execute iteration 1 with iteration value None: 0.02 s
[01:01.04] INFO: #run_model running step trip_scheduling
Running step 'trip_scheduling'
[01:01.06] INFO: trip_scheduling Running chunk 1 of 1 with 202 of 202 choosers
[01:01.06] INFO: trip_scheduling.i1 scheduling 482 trips within chunk 1
[01:01.16] INFO: trip_scheduling.i1 6 failed
[01:01.16] INFO: trip_scheduling.i2 scheduling 18 trips within chunk 1
[01:01.24] INFO: trip_scheduling.i2 7 failed
[01:01.24] INFO: trip_scheduling.i3 scheduling 11 trips within chunk 1
[01:01.32] INFO: trip_scheduling.i3 7 failed
[01:01.32] INFO: trip_scheduling.i4 scheduling 11 trips within chunk 1
[01:01.40] INFO: trip_scheduling.i4 7 failed
[01:01.40] INFO: trip_scheduling.i5 scheduling 11 trips within chunk 1
[01:01.48] INFO: trip_scheduling.i5 7 failed
[01:01.48] INFO: trip_scheduling.i6 scheduling 11 trips within chunk 1
[01:01.56] INFO: trip_scheduling.i6 7 failed
[01:01.56] INFO: trip_scheduling.i7 scheduling 11 trips within chunk 1
[01:01.64] INFO: trip_scheduling.i7 7 failed
[01:01.64] INFO: trip_scheduling.i8 scheduling 11 trips within chunk 1
[01:01.72] INFO: trip_scheduling.i8 7 failed
[01:01.72] INFO: trip_scheduling.i9 scheduling 11 trips within chunk 1
[01:01.80] INFO: trip_scheduling.i9 7 failed
[01:01.80] INFO: trip_scheduling.i10 scheduling 11 trips within chunk 1
[01:01.88] INFO: trip_scheduling.i10 7 failed
[01:01.89] INFO: trip_scheduling.i11 scheduling 11 trips within chunk 1
[01:01.98] INFO: trip_scheduling.i11 7 failed
[01:01.98] INFO: trip_scheduling.i12 scheduling 11 trips within chunk 1
[01:02.07] INFO: trip_scheduling.i12 7 failed
[01:02.07] INFO: trip_scheduling.i13 scheduling 11 trips within chunk 1
[01:02.17] INFO: trip_scheduling.i13 7 failed
[01:02.17] INFO: trip_scheduling.i14 scheduling 11 trips within chunk 1
[01:02.25] INFO: trip_scheduling.i14 7 failed
[01:02.25] INFO: trip_scheduling.i15 scheduling 11 trips within chunk 1
[01:02.33] INFO: trip_scheduling.i15 7 failed
[01:02.33] INFO: trip_scheduling.i16 scheduling 11 trips within chunk 1
[01:02.41] INFO: trip_scheduling.i16 7 failed
[01:02.41] INFO: trip_scheduling.i17 scheduling 11 trips within chunk 1
[01:02.49] INFO: trip_scheduling.i17 7 failed
[01:02.49] INFO: trip_scheduling.i18 scheduling 11 trips within chunk 1
[01:02.57] INFO: trip_scheduling.i18 7 failed
[01:02.57] INFO: trip_scheduling.i19 scheduling 11 trips within chunk 1
[01:02.65] INFO: trip_scheduling.i19 7 failed
[01:02.65] INFO: trip_scheduling.i20 scheduling 11 trips within chunk 1
[01:02.73] INFO: trip_scheduling.i20 7 failed
[01:02.73] INFO: trip_scheduling.i21 scheduling 11 trips within chunk 1
[01:02.80] INFO: trip_scheduling.i21 7 failed
[01:02.81] INFO: trip_scheduling.i22 scheduling 11 trips within chunk 1
[01:02.90] INFO: trip_scheduling.i22 7 failed
[01:02.90] INFO: trip_scheduling.i23 scheduling 11 trips within chunk 1
[01:02.99] INFO: trip_scheduling.i23 7 failed
[01:03.00] INFO: trip_scheduling.i24 scheduling 11 trips within chunk 1
[01:03.09] INFO: trip_scheduling.i24 7 failed
[01:03.09] INFO: trip_scheduling.i25 scheduling 11 trips within chunk 1
[01:03.18] INFO: trip_scheduling.i25 7 failed
[01:03.18] INFO: trip_scheduling.i26 scheduling 11 trips within chunk 1
[01:03.26] INFO: trip_scheduling.i26 7 failed
[01:03.26] INFO: trip_scheduling.i27 scheduling 11 trips within chunk 1
[01:03.34] INFO: trip_scheduling.i27 7 failed
[01:03.34] INFO: trip_scheduling.i28 scheduling 11 trips within chunk 1
[01:03.42] INFO: trip_scheduling.i28 7 failed
[01:03.42] INFO: trip_scheduling.i29 scheduling 11 trips within chunk 1
[01:03.50] INFO: trip_scheduling.i29 7 failed
[01:03.50] INFO: trip_scheduling.i30 scheduling 11 trips within chunk 1
[01:03.58] INFO: trip_scheduling.i30 7 failed
[01:03.58] INFO: trip_scheduling.i31 scheduling 11 trips within chunk 1
[01:03.66] INFO: trip_scheduling.i31 7 failed
[01:03.66] INFO: trip_scheduling.i32 scheduling 11 trips within chunk 1
[01:03.74] INFO: trip_scheduling.i32 7 failed
[01:03.74] INFO: trip_scheduling.i33 scheduling 11 trips within chunk 1
[01:03.82] INFO: trip_scheduling.i33 7 failed
[01:03.82] INFO: trip_scheduling.i34 scheduling 11 trips within chunk 1
[01:03.90] INFO: trip_scheduling.i34 7 failed
[01:03.90] INFO: trip_scheduling.i35 scheduling 11 trips within chunk 1
[01:03.98] INFO: trip_scheduling.i35 7 failed
[01:03.98] INFO: trip_scheduling.i36 scheduling 11 trips within chunk 1
[01:04.06] INFO: trip_scheduling.i36 7 failed
[01:04.06] INFO: trip_scheduling.i37 scheduling 11 trips within chunk 1
[01:04.14] INFO: trip_scheduling.i37 7 failed
[01:04.14] INFO: trip_scheduling.i38 scheduling 11 trips within chunk 1
[01:04.22] INFO: trip_scheduling.i38 7 failed
[01:04.22] INFO: trip_scheduling.i39 scheduling 11 trips within chunk 1
[01:04.30] INFO: trip_scheduling.i39 7 failed
[01:04.30] INFO: trip_scheduling.i40 scheduling 11 trips within chunk 1
[01:04.38] INFO: trip_scheduling.i40 7 failed
[01:04.38] INFO: trip_scheduling.i41 scheduling 11 trips within chunk 1
[01:04.47] INFO: trip_scheduling.i41 7 failed
[01:04.47] INFO: trip_scheduling.i42 scheduling 11 trips within chunk 1
[01:04.59] INFO: trip_scheduling.i42 7 failed
[01:04.59] INFO: trip_scheduling.i43 scheduling 11 trips within chunk 1
[01:04.69] INFO: trip_scheduling.i43 7 failed
[01:04.69] INFO: trip_scheduling.i44 scheduling 11 trips within chunk 1
[01:04.77] INFO: trip_scheduling.i44 7 failed
[01:04.77] INFO: trip_scheduling.i45 scheduling 11 trips within chunk 1
[01:04.85] INFO: trip_scheduling.i45 7 failed
[01:04.85] INFO: trip_scheduling.i46 scheduling 11 trips within chunk 1
[01:04.93] INFO: trip_scheduling.i46 7 failed
[01:04.93] INFO: trip_scheduling.i47 scheduling 11 trips within chunk 1
[01:05.01] INFO: trip_scheduling.i47 7 failed
[01:05.01] INFO: trip_scheduling.i48 scheduling 11 trips within chunk 1
[01:05.09] INFO: trip_scheduling.i48 7 failed
[01:05.09] INFO: trip_scheduling.i49 scheduling 11 trips within chunk 1
[01:05.17] INFO: trip_scheduling.i49 7 failed
[01:05.18] INFO: trip_scheduling.i50 scheduling 11 trips within chunk 1
[01:05.25] INFO: trip_scheduling.i50 7 failed
[01:05.25] INFO: trip_scheduling.i51 scheduling 11 trips within chunk 1
[01:05.33] INFO: trip_scheduling.i51 7 failed
[01:05.33] INFO: trip_scheduling.i52 scheduling 11 trips within chunk 1
[01:05.41] INFO: trip_scheduling.i52 7 failed
[01:05.42] INFO: trip_scheduling.i53 scheduling 11 trips within chunk 1
[01:05.50] INFO: trip_scheduling.i53 7 failed
[01:05.50] INFO: trip_scheduling.i54 scheduling 11 trips within chunk 1
[01:05.61] INFO: trip_scheduling.i54 7 failed
[01:05.61] INFO: trip_scheduling.i55 scheduling 11 trips within chunk 1
[01:05.70] INFO: trip_scheduling.i55 7 failed
[01:05.70] INFO: trip_scheduling.i56 scheduling 11 trips within chunk 1
[01:05.79] INFO: trip_scheduling.i56 7 failed
[01:05.79] INFO: trip_scheduling.i57 scheduling 11 trips within chunk 1
[01:05.88] INFO: trip_scheduling.i57 7 failed
[01:05.88] INFO: trip_scheduling.i58 scheduling 11 trips within chunk 1
[01:05.95] INFO: trip_scheduling.i58 7 failed
[01:05.96] INFO: trip_scheduling.i59 scheduling 11 trips within chunk 1
[01:06.03] INFO: trip_scheduling.i59 7 failed
[01:06.04] INFO: trip_scheduling.i60 scheduling 11 trips within chunk 1
[01:06.11] INFO: trip_scheduling.i60 7 failed
[01:06.12] INFO: trip_scheduling.i61 scheduling 11 trips within chunk 1
[01:06.19] INFO: trip_scheduling.i61 7 failed
[01:06.19] INFO: trip_scheduling.i62 scheduling 11 trips within chunk 1
[01:06.27] INFO: trip_scheduling.i62 7 failed
[01:06.27] INFO: trip_scheduling.i63 scheduling 11 trips within chunk 1
[01:06.35] INFO: trip_scheduling.i63 7 failed
[01:06.35] INFO: trip_scheduling.i64 scheduling 11 trips within chunk 1
[01:06.43] INFO: trip_scheduling.i64 7 failed
[01:06.43] INFO: trip_scheduling.i65 scheduling 11 trips within chunk 1
[01:06.52] INFO: trip_scheduling.i65 7 failed
[01:06.52] INFO: trip_scheduling.i66 scheduling 11 trips within chunk 1
[01:06.60] INFO: trip_scheduling.i66 7 failed
[01:06.60] INFO: trip_scheduling.i67 scheduling 11 trips within chunk 1
[01:06.69] INFO: trip_scheduling.i67 7 failed
[01:06.69] INFO: trip_scheduling.i68 scheduling 11 trips within chunk 1
[01:06.78] INFO: trip_scheduling.i68 7 failed
[01:06.78] INFO: trip_scheduling.i69 scheduling 11 trips within chunk 1
[01:06.87] INFO: trip_scheduling.i69 7 failed
[01:06.87] INFO: trip_scheduling.i70 scheduling 11 trips within chunk 1
[01:06.95] INFO: trip_scheduling.i70 7 failed
[01:06.95] INFO: trip_scheduling.i71 scheduling 11 trips within chunk 1
[01:07.03] INFO: trip_scheduling.i71 7 failed
[01:07.04] INFO: trip_scheduling.i72 scheduling 11 trips within chunk 1
[01:07.11] INFO: trip_scheduling.i72 7 failed
[01:07.11] INFO: trip_scheduling.i73 scheduling 11 trips within chunk 1
[01:07.19] INFO: trip_scheduling.i73 7 failed
[01:07.19] INFO: trip_scheduling.i74 scheduling 11 trips within chunk 1
[01:07.27] INFO: trip_scheduling.i74 7 failed
[01:07.27] INFO: trip_scheduling.i75 scheduling 11 trips within chunk 1
[01:07.35] INFO: trip_scheduling.i75 7 failed
[01:07.35] INFO: trip_scheduling.i76 scheduling 11 trips within chunk 1
[01:07.43] INFO: trip_scheduling.i76 7 failed
[01:07.43] INFO: trip_scheduling.i77 scheduling 11 trips within chunk 1
[01:07.51] INFO: trip_scheduling.i77 7 failed
[01:07.51] INFO: trip_scheduling.i78 scheduling 11 trips within chunk 1
[01:07.59] INFO: trip_scheduling.i78 7 failed
[01:07.59] INFO: trip_scheduling.i79 scheduling 11 trips within chunk 1
[01:07.67] INFO: trip_scheduling.i79 7 failed
[01:07.67] INFO: trip_scheduling.i80 scheduling 11 trips within chunk 1
[01:07.75] INFO: trip_scheduling.i80 7 failed
[01:07.75] INFO: trip_scheduling.i81 scheduling 11 trips within chunk 1
[01:07.83] INFO: trip_scheduling.i81 7 failed
[01:07.83] INFO: trip_scheduling.i82 scheduling 11 trips within chunk 1
[01:07.91] INFO: trip_scheduling.i82 7 failed
[01:07.91] INFO: trip_scheduling.i83 scheduling 11 trips within chunk 1
[01:07.98] INFO: trip_scheduling.i83 7 failed
[01:07.98] INFO: trip_scheduling.i84 scheduling 11 trips within chunk 1
[01:08.06] INFO: trip_scheduling.i84 7 failed
[01:08.06] INFO: trip_scheduling.i85 scheduling 11 trips within chunk 1
[01:08.14] INFO: trip_scheduling.i85 7 failed
[01:08.14] INFO: trip_scheduling.i86 scheduling 11 trips within chunk 1
[01:08.22] INFO: trip_scheduling.i86 7 failed
[01:08.22] INFO: trip_scheduling.i87 scheduling 11 trips within chunk 1
[01:08.30] INFO: trip_scheduling.i87 7 failed
[01:08.30] INFO: trip_scheduling.i88 scheduling 11 trips within chunk 1
[01:08.38] INFO: trip_scheduling.i88 7 failed
[01:08.38] INFO: trip_scheduling.i89 scheduling 11 trips within chunk 1
[01:08.46] INFO: trip_scheduling.i89 7 failed
[01:08.46] INFO: trip_scheduling.i90 scheduling 11 trips within chunk 1
[01:08.54] INFO: trip_scheduling.i90 7 failed
[01:08.54] INFO: trip_scheduling.i91 scheduling 11 trips within chunk 1
[01:08.62] INFO: trip_scheduling.i91 7 failed
[01:08.62] INFO: trip_scheduling.i92 scheduling 11 trips within chunk 1
[01:08.69] INFO: trip_scheduling.i92 7 failed
[01:08.70] INFO: trip_scheduling.i93 scheduling 11 trips within chunk 1
[01:08.77] INFO: trip_scheduling.i93 7 failed
[01:08.77] INFO: trip_scheduling.i94 scheduling 11 trips within chunk 1
[01:08.85] INFO: trip_scheduling.i94 7 failed
[01:08.85] INFO: trip_scheduling.i95 scheduling 11 trips within chunk 1
[01:08.93] INFO: trip_scheduling.i95 7 failed
[01:08.93] INFO: trip_scheduling.i96 scheduling 11 trips within chunk 1
[01:09.01] INFO: trip_scheduling.i96 7 failed
[01:09.01] INFO: trip_scheduling.i97 scheduling 11 trips within chunk 1
[01:09.10] INFO: trip_scheduling.i97 7 failed
[01:09.10] INFO: trip_scheduling.i98 scheduling 11 trips within chunk 1
[01:09.19] INFO: trip_scheduling.i98 7 failed
[01:09.20] INFO: trip_scheduling.i99 scheduling 11 trips within chunk 1
[01:09.29] INFO: trip_scheduling.i99 7 failed
[01:09.29] INFO: trip_scheduling.i100 scheduling 11 trips within chunk 1
[01:09.31] WARNING: trip_scheduling.i100.outbound.num_2 coercing 0 depart choices to most initial
[01:09.33] INFO: dumping trip_scheduling.i100.outbound.num_3.failed_choosers
[01:09.33] WARNING: trip_scheduling.i100.outbound.num_3 coercing 1 depart choices to most initial
[01:09.34] INFO: dumping trip_scheduling.i100.outbound.num_4.failed_choosers
[01:09.34] WARNING: trip_scheduling.i100.outbound.num_4 coercing 1 depart choices to most initial
[01:09.36] INFO: dumping trip_scheduling.i100.inbound.num_1.failed_choosers
[01:09.36] WARNING: trip_scheduling.i100.inbound.num_1 coercing 2 depart choices to most initial
[01:09.38] INFO: dumping trip_scheduling.i100.inbound.num_2.failed_choosers
[01:09.38] WARNING: trip_scheduling.i100.inbound.num_2 coercing 2 depart choices to most initial
[01:09.39] INFO: dumping trip_scheduling.i100.inbound.num_3.failed_choosers
[01:09.39] WARNING: trip_scheduling.i100.inbound.num_3 coercing 1 depart choices to most initial
[01:09.39] INFO: trip_scheduling.i100 0 failed
Time to execute step 'trip_scheduling': 8.35 s
Total time to execute iteration 1 with iteration value None: 8.35 s
[01:09.47] INFO: #run_model running step trip_mode_choice
Running step 'trip_mode_choice'
[01:09.49] INFO: Running trip_mode_choice with 482 trips
[01:09.51] INFO: primary_purpose top 10 value counts:
work        168
shopping     77
othmaint     46
othdiscr     43
school       37
eatout       33
atwork       27
univ         26
social       19
escort        6
Name: primary_purpose, dtype: int64
[01:09.52] INFO: trip_mode_choice tour_type 'atwork' (27 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:09.65] INFO: trip_mode_choice.atwork.simple_simulate Running adaptive_chunked_choosers with 27 choosers
[01:09.65] INFO: Running chunk 1 of 1 with 27 of 27 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:10.02] WARNING: slice_canonically: no rows in trip_mode_choice.atwork.trip_mode with household_id == [982875]
[01:10.02] INFO: trip_mode_choice tour_type 'eatout' (33 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:10.16] INFO: trip_mode_choice.eatout.simple_simulate Running adaptive_chunked_choosers with 33 choosers
[01:10.16] INFO: Running chunk 1 of 1 with 33 of 33 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:10.57] INFO: trip_mode_choice tour_type 'escort' (6 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:10.70] INFO: trip_mode_choice.escort.simple_simulate Running adaptive_chunked_choosers with 6 choosers
[01:10.70] INFO: Running chunk 1 of 1 with 6 of 6 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:11.04] WARNING: slice_canonically: no rows in trip_mode_choice.escort.trip_mode with household_id == [982875]
[01:11.04] INFO: trip_mode_choice tour_type 'othdiscr' (43 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:11.17] INFO: trip_mode_choice.othdiscr.simple_simulate Running adaptive_chunked_choosers with 43 choosers
[01:11.17] INFO: Running chunk 1 of 1 with 43 of 43 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:11.58] WARNING: slice_canonically: no rows in trip_mode_choice.othdiscr.trip_mode with household_id == [982875]
[01:11.58] INFO: trip_mode_choice tour_type 'othmaint' (46 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:11.72] INFO: trip_mode_choice.othmaint.simple_simulate Running adaptive_chunked_choosers with 46 choosers
[01:11.72] INFO: Running chunk 1 of 1 with 46 of 46 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:12.12] WARNING: slice_canonically: no rows in trip_mode_choice.othmaint.trip_mode with household_id == [982875]
[01:12.12] INFO: trip_mode_choice tour_type 'school' (37 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:12.26] INFO: trip_mode_choice.school.simple_simulate Running adaptive_chunked_choosers with 37 choosers
[01:12.26] INFO: Running chunk 1 of 1 with 37 of 37 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:12.62] WARNING: slice_canonically: no rows in trip_mode_choice.school.trip_mode with household_id == [982875]
[01:12.62] INFO: trip_mode_choice tour_type 'shopping' (77 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:12.75] INFO: trip_mode_choice.shopping.simple_simulate Running adaptive_chunked_choosers with 77 choosers
[01:12.75] INFO: Running chunk 1 of 1 with 77 of 77 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:13.15] WARNING: slice_canonically: no rows in trip_mode_choice.shopping.trip_mode with household_id == [982875]
[01:13.15] INFO: trip_mode_choice tour_type 'social' (19 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:13.32] INFO: trip_mode_choice.social.simple_simulate Running adaptive_chunked_choosers with 19 choosers
[01:13.32] INFO: Running chunk 1 of 1 with 19 of 19 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:13.72] WARNING: slice_canonically: no rows in trip_mode_choice.social.trip_mode with household_id == [982875]
[01:13.72] INFO: trip_mode_choice tour_type 'univ' (26 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:13.86] INFO: trip_mode_choice.univ.simple_simulate Running adaptive_chunked_choosers with 26 choosers
[01:13.86] INFO: Running chunk 1 of 1 with 26 of 26 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:14.26] WARNING: slice_canonically: no rows in trip_mode_choice.univ.trip_mode with household_id == [982875]
[01:14.26] INFO: trip_mode_choice tour_type 'work' (168 trips)
get item DIST
get item DISTWALK
get item DISTBIKE
[01:14.44] INFO: trip_mode_choice.work.simple_simulate Running adaptive_chunked_choosers with 168 choosers
[01:14.44] INFO: Running chunk 1 of 1 with 168 of 168 choosers
get item DISTWALK
get item DISTWALK
get item DISTBIKE
get item DISTBIKE
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DIST
get item DISTWALK
get item DISTBIKE
[01:14.99] INFO: trip_modes top 10 value counts:
WALK              211
WALK_LRF           80
WALK_LOC           64
TNC_SINGLE         38
DRIVEALONEFREE     27
BIKE               18
TAXI               15
WALK_HVY           14
SHARED2FREE         6
SHARED3FREE         6
Name: tour_mode, dtype: int64
[01:14.99] INFO: trip_mode_choice choices top 10 value counts:
WALK              304
WALK_LOC           85
WALK_LRF           53
BIKE               17
TNC_SHARED          4
TNC_SINGLE          4
SHARED2FREE         4
WALK_HVY            4
DRIVEALONEFREE      4
TAXI                2
Name: trip_mode, dtype: int64
Time to execute step 'trip_mode_choice': 5.53 s
Total time to execute iteration 1 with iteration value None: 5.53 s
[01:15.08] INFO: #run_model running step write_data_dictionary
Running step 'write_data_dictionary'
Time to execute step 'write_data_dictionary': 0.38 s
Total time to execute iteration 1 with iteration value None: 0.38 s
[01:15.53] INFO: #run_model running step track_skim_usage
Running step 'track_skim_usage'
Time to execute step 'track_skim_usage': 0.00 s
Total time to execute iteration 1 with iteration value None: 0.00 s
[01:15.59] INFO: #run_model running step write_trip_matrices
Running step 'write_trip_matrices'
get item DIST
[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.72] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/core/util.py:365: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  df[new_columns] = df2[new_columns]

[01:15.73] INFO: adding 'sample_rate' from households to trips table
[01:15.73] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/abm/models/trip_matrices.py:226: PerformanceWarning: DataFrame is highly fragmented.  This is usually the result of calling `frame.insert` many times, which has poor performance.  Consider joining all columns at once using pd.concat(axis=1) instead. To get a de-fragmented frame, use `newframe = frame.copy()`
  trips_df[hh_weight_col] = trips_df.household_id.map(household_weights)

[01:15.73] INFO: aggregating trips one zone...
[01:15.74] WARNING: /home/jin/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/abm/models/trip_matrices.py:63: FutureWarning: The default value of numeric_only in DataFrameGroupBy.sum is deprecated. In a future version, numeric_only will default to False. Either specify numeric_only or select only columns which should be valid for the function.
  aggregate_trips = trips_df.groupby(["origin", "destination"], sort=False).sum()

[01:15.74] INFO: opening output/trips_ea.omx
[01:15.76] INFO: adding zone_id mapping for 25 zones to trips_ea.omx
[01:15.76] INFO: closing output/trips_ea.omx
[01:15.77] INFO: opening output/trips_am.omx
[01:15.79] INFO: adding zone_id mapping for 25 zones to trips_am.omx
[01:15.79] INFO: closing output/trips_am.omx
[01:15.79] INFO: opening output/trips_md.omx
[01:15.81] INFO: adding zone_id mapping for 25 zones to trips_md.omx
[01:15.81] INFO: closing output/trips_md.omx
[01:15.82] INFO: opening output/trips_pm.omx
[01:15.83] INFO: adding zone_id mapping for 25 zones to trips_pm.omx
[01:15.83] INFO: closing output/trips_pm.omx
[01:15.84] INFO: opening output/trips_ev.omx
[01:15.86] INFO: adding zone_id mapping for 25 zones to trips_ev.omx
[01:15.86] INFO: closing output/trips_ev.omx
Time to execute step 'write_trip_matrices': 0.27 s
Total time to execute iteration 1 with iteration value None: 0.27 s
[01:15.93] INFO: #run_model running step write_tables
Running step 'write_tables'
Time to execute step 'write_tables': 0.02 s
Total time to execute iteration 1 with iteration value None: 0.02 s
[01:16.02] INFO: #run_model running step summarize
Running step 'summarize'
get item DISTBIKE
get item DISTWALK
Time to execute step 'summarize': 0.32 s
Total time to execute iteration 1 with iteration value None: 0.32 s
[01:16.40] INFO: Time to execute run_model (34 models) : 75.211 seconds (1.3 minutes)
[01:16.41] INFO: MainProcess high water mark rss: 235_941_888 (235.9 MB) timestamp: 20/09/2023 16:22:44 label:pipeline.run_model trip_scheduling finished
[01:16.41] INFO: MainProcess high water mark uss: 234_770_432 (234.8 MB) timestamp: 20/09/2023 16:22:51 label:pipeline.run_model summarize finished
[01:16.41] INFO: Time to execute all models : 75.26 seconds (1.3 minutes)
(ASIM_DEV_new) jin@rush:~/AVAIL/code/activitysim/activitysim/activitysim_new/activitysim/activitysim/examples/test_prototype_mtc_new$ 
