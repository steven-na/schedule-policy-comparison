---
created: 2026-03-26
tags:
  - programming
fs_location: ~/projects/programming/cpp/job-schedule-comp
---
# Scheduling Policy Comparison Program
---
## Project Overview
### Problem
It can be difficult to tell in which circumstances some schedulers (eg. Round Robin, SJF, STCF, MLFQ, Lottery, Stride) will perform poorly or excel.
### Goal
A CLI tool that will output data for the performance of a scheduler or set of schedulers based on a given workload. The data will then be able to be fed into a spreadsheet application or parsed with python (pandas, matplotlib). The user will be able to pass in a JSON file and output directory and the software will run the declared workload for each scheduler
### Target Users
Engineers interested in choosing a scheduler for their use case.

---
## Scope
### Schedulers to Compare
First in First Out, Shortest Job First, Shortest Time-to-Completion First, Round Robin, Multi Level Feedback Queue, Lottery, Stride, Completely Fair Scheduler (CFS will be implemented last out of everything in the project). In the workload file, configs for each scheduler will be laid out (RR quantum, MLFQ queue count/quanta, lottery seed, etc)
### Comparison Criteria
High level criteria will include averages of turnaround time ($T_{turnaround}=T_{completion}-T_{arrival}$), response time ($T_{response}=T_{first run}-T_{arrival}$), fairness ($F=T_{firstcomplete}/T_{secondcomplete}$), detailed logs of the runs will be provided for analysis as well (CSV of every timestep for each scheduler and the information for each job mapped to the timesteps)
### Out of scope
Multi-processor schedulers, Real time comparisons/running (system benchmark)

---
## Requirements
### Functional
Must support reading jobs and scheduler information from JSON file, this includes declaring schedulers (Including multiple configs of the same scheduler) and workloads with optional values (no ticket count for non lottery benchmarks). Output will be, atleast, a csv with timestamps per line, metrics json, workload details json for correlating data during analysis
### Non-Functional
Must run under 1 second per scheduler with 1000 jobs that average 1000 tick duration, Must support all arguments that will be documented (-i=input file -o=outputdir --multithread (if it becomes a performance necessity)), reproducible results (other than randomized schedulers)

---
## Architecture
### Language/Framework
C++ built binary, using custom implemented data structures that may need to be written per-scheduler (job queue for RR, multi-level queue for MLFQ, job linked list for lottery, stride map for stride), if performance necessitates multithreading will be implemented which should be quite simple due to low coupling between each scheduler simulation (This document will be revised in that case)
### High-level Components
Main testing interface that each scheduler can slot into in a modular manner where they are tested on a per-tick basis, configurable implementation for each scheduler, job implementation that holds state for each job (id, remaining time, max runtime, current runtime, min downtime, tickets (for lottery scheduler)), workload loader (load jobs from file), output data builder
### Simulator Class
- The simulator will hold a master copy of the job list
- each tick, the sim will decide whether to give or relinquish jobs to/from the scheduler.
- The sim will update each job after each tick
- The sim will hold the main loop logic
### Scheduler Base Class
- The base class will only have a few basic virtual methods so that it can interface with the testing apparatus easily.
- The jobs will be held by shared pointers or some other sharing functionality so that the simulator can update each job at the end of a tick
- `add()` will tell the scheduler to add a new job to its structure (make sure to check if already in)
- `remove()` will tell the scheduler to remove a job from its structure (make sure to check if not in)
- `choose()` will return a pointer to the chosen job 
- `update()` will be used to implement any updating of the structure a scheduler needs to do (RR might move to the next pointer in the list if the time is met)
### Simulation Process and details
1. Workloads are loaded into the simulator
	- They are represented by a struct with members:
		1. **id**: for jobs with identical arrival time, `id` will be the decider, lower id first.
		2. **remaining time**: the total amount of ticks the job will run for.
		3. **arrival time**: job cant be chosen until `tick` >= `arrival time`.
		4. **max runtime**: max time before i/o must be performed.
		5. **current runtime**: total time running since start or last i/o was completed, do not reset on context switch.
		6. **minimum downtime**: how long the job must stop for i/o until it can start again.
		7. **current downtime**: how long until the job can run again, `0` means it can run, when `current runtime` = `max runtime`, set `current runtime` = `0` and `current downtime` = `max downtime`.
		8. **lottery tickets**: optional for lottery.
		9. **stride length**: optional for stride.
		10. **niceness**: optional for CFS.
2. The simulation begins
	1. The simulator updates the jobs available to the scheduler based on arrival time and i/o status with `Scheduler::remove()` and `Scheduler::add()`
	2. A job is chosen by the scheduler
	3. The job's remaining time is decremented
	4. Update job states and record info
		- If job is completed, `remove()` it and document
		- Downtime (if > 0)
		- current runtime (if selected)
	5. Update scheduler state (stride table, queue, etc)
	6. increment tick count
	7. repeat
3. Repeat for each scheduler
### Input JSON
The input file will hold 2 objects: A scheduler list and a workload list. The scheduler list will have schedulers, each called out by a name for the scheduler, a display name, and configuration for the scheduler. The workload object will hold a list of jobs. Jobs can have many fields, some required, some mandatory. 
- **Required:**
	- id (Unique, checked by input handler)
	- job length
	- arrival time
- **Optional:**
	- I/O config (max runtime, min downtime, all assumed -1 (disabled) unless specified)
	- lottery tickets (lottery scheduler)
	- stride length (stride scheduler)
	- niceness (CFS scheduler)
### Data Model
Main metrics for assessing schedulers and detailed run output that includes timestamps (time, job id that is running, is job complete?), job list (id, total time the job takes, max run time \[simulate i/o call], min downtime \[simulates i/o time])
### Input/Output
JSON Workload file in (workload fil will hold the jobs and scheduler info with configurations for each scheduler) -> CSV and simple stats out

---
## Comparison Engine
### How schedulers will be tested
Each scheduler will be given the same input of workloads based on the input file, they will simulate the scheduling and running until each one is done
### Metrics to collect
Turnaround time, response time, fairness, timestamps
### Benchmarking methodology
Each run should be deterministic, randomized schedulers will be given a seed by the input or one will be provided at runtime and documented in the output json, no needs for reruns on the same workload
### Environment Constraints
Output should remain similar between testing environments

---
## UI/Reporting
### How results are presented
CLI will output a simple table ranking tested schedulers, data analysis functionality will be extended with tools better suited for the task (Excel, Python)
### Visualization
Bar charts can be used to show job runtime data based on timestamps, tables and charts can be used to rank schedulers based on metrics
### Export Formats
CSV for timestamps, JSON output files for job info and metrics/scheduler info (JSON job info will hold each job finish time, arrival time, first run, etc AND metrics/sinfo will hold the scheduler info and their related metrics from the run) these are different schemas than the input file

---
## Development Plan
- [x] **Phase 1**: Testing interface with update loop logic and a base class for the schedulers, implement simplest scheduler (FIFO), simple workload handling, hard coded (1 week)
- [/] **Phase 2**: input json handling, multi-scheduler testing interface (0.5 weeks)
- [ ] **Phase 3**: Build out simple schedulers (RR, SJF, Lottery) and workload functionality (i/o sim, arrival time) (1 week)
- [ ] **Phase 4**: Build out complex schedulers (STCF, Stride) and workload functionality that pertains to each (1 week)
- [ ] **Phase 5**: Implement MLFQ and configuration for it (2 weeks)
- [ ] **Phase 6**: Implement data collection (log each event in loop) and analysis (average of metrics and comparison of each scheduler) (1.5 weeks)
- [ ] **Phase 7**: CLI polish/document interface, Data mutation into CSV and JSON Files (1 week) (Project is done now)
	- [ ] Optimise if necessary.
		- Ideas: Priority queue for NotArrivedJobs instead of vector.
- [ ] **Phase 8**: Implement CFS itself. A project by itself (2-3 weeks)

---
## Testing
### Testing strategy
- Tests for each scheduler (input 2 jobs and make sure it selects the expected one, or run until the job is finished and make sure the selections match the criteria)
- Tests for metrics (input certain data and validate that calculations are correct)
- Test I/O downtime logic, make sure jobs are always removed when down and always added when they go up
- Tests for comparisons (RR should beat SJF in response time always (in multi job situations))
### Edge cases
- Invalid user input (Dont do run, give info on what exactly was wrong, this includes empty required inputs (workload, scheduler info, etc))
- zero job runtime (This should be treated as invalid input as it is useless and will only mess up data)
- same arrival time (solution: sort by id)
- all jobs are same length (this will be handled per scheduler and is a valid test case)
- large downtimes (these are a valid test case and will be accepted)
### Validation approach
Compare results to expected outputs, real benchmarks

---
## Risks & Open questions
### Known unknowns
Is the I/O simulation (downtime) complex enough to yield interesting results? Algorithm performance with large workloads. Difficulties reading/writing to json/csv. Is CSF realistic to implement with this setup? Input JSON Schema may be too complicated. Jobs and schedulers may be passed a multiple files? also increases modularity of testing.